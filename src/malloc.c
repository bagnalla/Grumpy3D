/**
 * @file malloc.c
 * @author Alexander Bagnall
 * @date March 2, 2016
 * @brief File containing implementations of malloc(), free(), calloc(), and realloc().
 *
 * With these memory allocation functions, all dynamic memory is allocated in heap which is implemented as a doubly-linked list.
 * They can be used as a general-purpose replacement for the equivalent standard library functions (or so I think - may be unsafe in some way).
 * This implementation tends to perform better than the standard implementation for medium/large allocations but worse for small allocations.
 * The memory overhead for block metadata is also larger than the standard implementation which is most noticeable when doing a lot of small allocations.
 * The heap size is managed via the glibc sbrk() function.
 * Thread safety is guaranteed via a pthread mutex.
 * Beware though, errors will not result in a nice exception like bad_alloc.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/param.h>
#include <pthread.h>

#define HEAP_GROWTH_INCREMENT 4 // # of pages
 
/** @struct block_meta
 *  @brief The structure at the beginning of every block node in the list (whether free or allocated).
 *  @var block_meta::length
 *  Length of the data portion of the block (doesn't include sizeof this structure).
 *  @var block_meta::prev
 *  A pointer to the previous block. NULL if the current block is the first block.
 *  @var block_meta::next
 *  A pointer to the next block. NULL if the current block has been allocated (not free).
 *  If the current block is free, next points to either the next free block or end_brk if the current block is the last free block.
 *  @var x
 *  Reserved. Required for the struct to be long word aligned on a 32-bit system.
 */
typedef struct block_meta {
  size_t length;
  struct block_meta *prev;
  struct block_meta *next;
  int x;
} block_meta;

/*
 * block_meta could be reduced in size so that the next pointer resides
 * in the first few bytes of the data section of a block, but the
 * current implementation sets next to NULL on data blocks so it can
 * easily check whether a block is free or not. I could do some hacky
 * stuff like using one of the bits in the length field as an "is free"
 * flag and only having 31 bit length but I'm keeping it simple for now.
*/

/** 
 * @brief Round \p n to the nearest multiple of \p f.
 * @param n The number to be rounded up.
 * @param f The factor to be used.
 * @return Returns \p n rounded up to the nearest multiple of \p f.
 */
size_t round_up_multof(size_t n, size_t f)
{
    return (n + (f - 1)) / f * f;
}

static int initialized = 0;
static size_t page_size;
static void *start_brk;
static void *end_brk;
static block_meta *free_blocks;
static block_meta *last_free_block;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutexattr_t attr;

/** 
 * @brief Initialize the heap. Called on first use of malloc.
 */
static void init_heap()
{
    // initialize recursive mutex
    //pthread_mutexattr_init(&attr);
    //pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    //pthread_mutex_init(&mutex, &attr);

    // get system page size
    page_size = sysconf(_SC_PAGESIZE);

    // get initial addresses for everything
    start_brk = free_blocks = last_free_block = sbrk(page_size);
    end_brk = start_brk + page_size;
    
    // start with one big free block
    free_blocks->length = page_size - sizeof(block_meta);
    free_blocks->prev = NULL;
    free_blocks->next = end_brk;
}

/** 
 * @brief Grow the size of the heap.
 * @param amount The minimum number of bytes to increase by.
 * @return Returns the number of pages the heap increased by.
 */
static size_t grow_heap(size_t amount)
{
    size_t num_pages = round_up_multof(amount, page_size) / page_size;
    num_pages = round_up_multof(num_pages, HEAP_GROWTH_INCREMENT);
    end_brk = sbrk(num_pages * page_size) + num_pages * page_size;
    return num_pages;
}

/** 
 * @brief Create a free block in the heap.
 * @param loc A pointer to the beginning of the new block.
 * @param prev_block A pointer to the beginning of the free block preceding the new block.
 * @param next_block A pointer to the beginning of the free block succeeding the new block.
 * @param size The size of the new block (including the metadata structure).
 * @return Returns a pointer to the beginning of the new block (equal to \p loc).
 */
static void* create_free_block(block_meta *loc,
                                block_meta *prev_block,
                                block_meta *next_block,
                                size_t size)
{
    // length
    loc->length = size - sizeof(block_meta);

    // next block
    loc->next = next_block;
    if (next_block != end_brk)
        next_block->prev = loc;
    else
        last_free_block = loc;

    // previous block
    loc->prev = prev_block;
    if (prev_block != NULL)
    {
        prev_block->next = loc;
    }

    return loc;
}

/** 
 * @brief Create a data block in the heap.
 * @param loc A pointer to the beginning of the new block.
 * @param size The size of the new block (including the metadata structure).
 * @param length The length of the free block that is being replaced or partially replaced by the new block.
 * @param prev_free_block A pointer to the beginning of the free block preceding the new block.
 * @param next_free_block A pointer to the beginning of the free block succeeding the new block.
 * @return Returns a pointer to the beginning of the data section of the new block.
 */
static void* create_data_block(block_meta *loc, size_t size,
                                size_t length,
                                block_meta *prev_free_block,
                                block_meta *next_free_block)
{
    // set size of data block
    loc->length = size;

    // get data pointer to return
    void *start_data = loc + 1; // 1 * sizeof(block_meta) since loc is type block_meta

    // if enough space, create new block in remaining contiguous space
    block_meta *new_free_block = NULL;
    if (length - size >= sizeof(block_meta) + 8)
    {
        new_free_block = create_free_block(start_data + size, prev_free_block, next_free_block, length - size);
    }
    // else still need to update next/prev fields of nearest free blocks
    else
    {
        // just give the leftover data to the data block
        loc->length = length;

        if (loc->prev != NULL)
        {
            loc->prev->next = loc->next;

            if (loc == last_free_block)
                last_free_block = loc->prev;
        }
        if (loc->next != end_brk)
            loc->next->prev = loc->prev;
    }

    // mark as data block
    loc->next = NULL;

    // if this was first block, need to change free_blocks pointer
    if (loc == free_blocks)
    {
        // if new block was made, set free_blocks to point to it
        if (new_free_block != NULL)
            free_blocks = new_free_block;
        // else set free_blocks to point to next block
        else
        {
            free_blocks = next_free_block;

            // if it is the end of the heap, grow the heap and create a new block in the new region
            if (free_blocks == end_brk)
            {
                grow_heap(1);
                create_free_block(free_blocks, NULL, end_brk, (char*)end_brk - (char*)free_blocks);
            }

            free_blocks->prev = NULL;
        }
    }

    // return data pointer
    return start_data;
}

/** 
 * @brief Allocate space on the heap for use by a program.
 * @param size The minimum number of bytes to allocate.
 * @return Returns a pointer to the allocated memory.
 */
void* malloc(size_t size)
{
    pthread_mutex_lock(&mutex);
    
    if (!initialized)
    {
        initialized = 1;
        init_heap();
    }

    if (!size)
    {
        pthread_mutex_unlock(&mutex);
        return NULL;
    }

    size = round_up_multof(size, 8);

    // begin at free_blocks
    block_meta *cursor = free_blocks;
    block_meta *prev_free_block = NULL;

    while (cursor != end_brk)
    {
        // length of current block
        size_t length = cursor->length;

        // location of next block (could be end_brk)
        block_meta *next_free_block = cursor->next;

        // if block is big enough
        if (length >= size)
        {
            void *ptr = create_data_block(cursor, size, length, prev_free_block, next_free_block);
            pthread_mutex_unlock(&mutex);
            return ptr;
        }

        // otherwise advance cursor
        prev_free_block = cursor;
        cursor = cursor->next;
    }

    // if we made it this far, a suitable free block was not found
    // so the size of the heap must be increased

    // if the last free block was at the end of the heap, expand it to the new end
    if (prev_free_block != NULL && (char*)prev_free_block + sizeof(block_meta) + prev_free_block->length == end_brk)
    {
        // length of last free block
        size_t length = prev_free_block->length;
        // amount we need to expand by
        size_t required_space = size + sizeof(block_meta) - length;
        // grow heap, get number of pages it grew by
        size_t num_of_pages_grown = grow_heap(required_space);

        // new length increases by new pages * page size
        length += num_of_pages_grown * page_size;
        // copy length into the length field of the last free block
        prev_free_block->length = length;
        // copy new end_brk location into next block field of the last free block
        prev_free_block->next = end_brk;

        // create new data block starting at the last free block and return the data pointer
        void *ptr = create_data_block(prev_free_block, size, length, prev_free_block->prev, end_brk);
        pthread_mutex_unlock(&mutex);
        return ptr;
    }
    // else create new block in the new region
    else
    {
        // grow heap, get number of pages it grew by
        size_t new_block_size_pages = grow_heap(size + sizeof(block_meta));
        // length of the new free block
        size_t length = new_block_size_pages * page_size - sizeof(block_meta);
        // create new free block
        create_free_block(cursor, prev_free_block, end_brk, new_block_size_pages * page_size);

        // create new data block starting at new free block and return the data pointer
        void *ptr = create_data_block(cursor, size, length, prev_free_block, end_brk);
        pthread_mutex_unlock(&mutex);
        return ptr;
    }
    
    pthread_mutex_unlock(&mutex);
}

/** 
 * @brief Deallocate space in the heap that has been allocated by malloc().
 * @param ptr A pointer to the allocated memory.
 */
void free(void *ptr)
{
    if (ptr == NULL)
        return;
        
    pthread_mutex_lock(&mutex);

    // block we are freeing
    block_meta *block = ptr - sizeof(block_meta);

    //if (block->next != NULL) // shouldn't happen, this means its not a data block
    //    return;

    // if this block is after the last free block
    if (block > last_free_block)
    {
        // if this block is immediately after last_free_block, merge
        if ((char*)last_free_block + sizeof(block_meta) + last_free_block->length == (char*)block)
        {
            last_free_block->length += block->length + sizeof(block_meta);
        }
        // else this is the new last_free_block
        else
        {
            last_free_block->next = block;
            block->prev = last_free_block;
            block->next = end_brk;

            last_free_block = block;
        }
    }
    // else if this block is before first free block
    else if (block < free_blocks)
    {
        // if this block is immediately before free_blocks, merge
        if ((char*)block + sizeof(block_meta) + block->length == (char*)free_blocks)
        {
            block->length += free_blocks->length + sizeof(block_meta);
            block->next = free_blocks->next;
            if (free_blocks->next != end_brk)
                free_blocks->next->prev = block;
        }
        // else connect this block and free_blocks
        else
        {
            block->next = free_blocks;
            free_blocks->prev = block;
        }

        block->prev = NULL;

        // this is the new free_blocks
        free_blocks = block;
    }
    // else this block is in the middle somewhere
    else
    {
        // next_block immediately after this one (could be free or not)
        block_meta *next_block = (block_meta*)((char*)block + sizeof(block_meta) + block->length);
        block_meta *prev_block;

        // if next adjacent block is free, merge with it
        if (next_block->next != NULL)
        {
            block->length += next_block->length + sizeof(block_meta);
            block->next = next_block->next;
            if (next_block->next != end_brk)
                next_block->next->prev = block;
            else
                last_free_block = block;

            // get prev_block before changing it so we know our prev_block
            prev_block = next_block->prev;
        }
        // else find next free block and connect to it
        else
        {
            if ((size_t)block < ((size_t)start_brk + (size_t)end_brk) / 2)
            {
                prev_block = free_blocks;
                next_block = free_blocks->next;
                while (next_block < block)
                {
                    prev_block = prev_block->next;
                    next_block = next_block->next;
                }
            }
            else
            {
                next_block = last_free_block;
                prev_block = last_free_block->prev;
                while (prev_block > block)
                {
                    next_block = next_block->prev;
                    prev_block = prev_block->prev;
                }
            }

            block->next = next_block;
            next_block->prev = block;
        }

        // if this block is adjacent to prev_block, merge
        if ((char*) prev_block + sizeof(block_meta) + prev_block->length == (char*)block)
        {
            prev_block->length += block->length + sizeof(block_meta);
            prev_block->next = block->next;
            if (block->next != end_brk)
                block->next->prev = prev_block;
            else
                last_free_block = prev_block;
        }
        // else connect this block to prev_block
        else
        {
            prev_block->next = block;
            block->prev = prev_block;
        }
    }
    
    pthread_mutex_unlock(&mutex);
}

/** 
 * @brief Allocates memory for an array of \p nmemb elements and zero-initializes the allocated memory.
 * @param nmemb The number of array elements.
 * @param size The size of each array element.
 * @return Returns a pointer to the allocated memory. If \p nmemb or \p size is 0, NULL is returned.
 */
void *calloc(size_t nmemb, size_t size)
{
    size_t real_size = nmemb * size;

    if (!real_size)
        return NULL;
        
    //pthread_mutex_lock(&mutex);

    void * volatile ptr = malloc(real_size);

    memset(ptr, 0, real_size);
    
    //pthread_mutex_unlock(&mutex);

    return ptr;
}

// not sure if extra mutex is required in realloc
/** 
 * @brief Changes the size of the memory block pointed to by \p ptr.
 * @param ptr A pointer to memory allocated by malloc().
 * @param size The new block data size.
 * @return Returns a pointer to the resized block. It is guaranteed to be different from the original pointer.
 * @note If \p ptr is NULL, this function is equivalent to malloc()
 * @note If \p ptr is not NULL and \p size is 0, this function is equivalent to free()
 */
void *realloc(void *ptr, size_t size)
{
    size = round_up_multof(size, 8);

    // if ptr is NULL, equivalent to malloc(size)
    if (ptr == NULL)
        return malloc(size);

    // if ptr not NULL and size is 0, equivalent to free(ptr)
    if (!size)
    {
        free(ptr);
        return NULL;
    }
    
    //pthread_mutex_lock(&mutex);

    void * volatile new_ptr = malloc(size);

    // length field of ptr's data block
    size_t old_size = * (size_t*) ((char*)ptr - sizeof(block_meta));

    size_t min_size = old_size < size ? old_size : size; // min

    memcpy(new_ptr, ptr, min_size);

    free(ptr);
    
    //pthread_mutex_unlock(&mutex);

    return new_ptr;
}
