#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>

#define HEAP_GROWTH_INCREMENT 4 // # of pages

extern int count;
int malloc_count = 0;
int free_count = 0;

typedef struct block_meta {
  size_t length;
  struct block_meta *prev;
  struct block_meta *next;
} block_meta;

void safe_print(const char *s, size_t size)
{
	char buffer [size];
	sprintf (buffer, s);
	write(1, buffer, size);
}

size_t round_up_multof_8(size_t n)
{
	return (n + 7) / 8 * 8;
}

size_t round_up_multof(size_t n, size_t f)
{
	return (n + (f - 1)) / f * f;
}

int initialized = 0;
size_t page_size;
void *start_brk;
void *end_brk;
block_meta *free_blocks;
block_meta *last_free_block;

void init_heap()
{
	initialized = 1;
	
	//printf("initializing malloc for first-time use...\n");
	
	page_size = sysconf(_SC_PAGESIZE);
	
	start_brk = free_blocks = last_free_block = sbrk(page_size);
	end_brk = start_brk + page_size;
	
	free_blocks->length = page_size - sizeof(block_meta);
	free_blocks->prev = NULL;
	free_blocks->next = end_brk;
	
	//printf("system page size: %d\n", page_size);
	//printf("heap begins at location %d and ends at location %d\n", start_brk, end_brk);
}

size_t grow_heap(size_t amount)
{
	size_t num_pages = round_up_multof(amount, page_size) / page_size;
	num_pages = round_up_multof(num_pages, HEAP_GROWTH_INCREMENT);
	end_brk = sbrk(num_pages * page_size) + num_pages * page_size;
	return num_pages;
}

void* create_free_block(block_meta *loc, block_meta *prev_block, block_meta *next_block, size_t size)
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

// returns data pointer
void* create_data_block(block_meta *loc, size_t size, size_t length, block_meta *prev_free_block, block_meta *next_free_block)
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
				create_free_block(free_blocks, NULL, end_brk, end_brk - (void*) free_blocks);
			}
			
			free_blocks->prev = NULL;
		}
	}
	
	//printf("creating data block, data pointer: %d\n", start_data);
	
	// return data pointer
	return start_data;
}

void* malloc(size_t size)
{
	if (!initialized)
		init_heap();
		
	if (!size)
		return NULL;
		
	malloc_count++;
		
	size = round_up_multof_8(size);
	
	//printf("allocating for size %d\n", (int)size);
	
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
			return create_data_block(cursor, size, length, prev_free_block, next_free_block);
		}
		
		// otherwise advance cursor
		prev_free_block = cursor;
		cursor = cursor->next;
	}
	
	// if we made it this far, a suitable free block was not found
	// so the size of the heap must be increased
	
	// if the last free block was at the end of the heap, expand it to the new end
	if (prev_free_block != NULL && (void*) prev_free_block + sizeof(block_meta) + prev_free_block->length == end_brk)
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
		return create_data_block(prev_free_block, size, length, prev_free_block->prev, end_brk);
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
		return create_data_block(cursor, size, length, prev_free_block, end_brk);
	}
}

void free(void *ptr)
{
	if (ptr == NULL)
		return;
	
	free_count++;
		
	//safe_print("freeing block\n", 14);
	//printf("freeing block at data pointer %d. malloc count: %d\n", ptr, malloc_count - free_count);
	
	// block we are freeing
	block_meta *block = ptr - sizeof(block_meta);
	
	//if (block->next != NULL) // shouldn't happen, this means its not a data block
	//	return;
	
	// if this block is after the last free block
	if (block > last_free_block)
	{
		// if this block is immediately after last_free_block, merge
		if ((void*) last_free_block + sizeof(block_meta) + last_free_block->length == block)
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
		if ((void*) block + sizeof(block_meta) + block->length == free_blocks)
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
		//safe_print("freeing block\n", 14);
		
		// next_block immediately after this one (could be free or not)
		block_meta *next_block = (void*) block + sizeof(block_meta) + block->length;
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
			// while next_block isn't free, keep moving forward until we reach one
			//while (next_block->next == NULL)
				//next_block = (void*) next_block + sizeof(block_meta) + next_block->length;
			// get prev_block before changing it so we know our prev_block
			//prev_block = next_block->prev;
			
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
		if ((void*) prev_block + sizeof(block_meta) + prev_block->length == block)
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
}

void *calloc(size_t nmemb, size_t size)
{
	size_t real_size = nmemb * size;
	
	if (!real_size)
		return NULL;
	
	void *ptr = malloc(real_size);
	
	memset(ptr, 0, real_size);
	
	return ptr;
}

void *realloc(void *ptr, size_t size)
{
	size = round_up_multof_8(size);
	
	// if ptr is NULL, equivalent to malloc(size)
	if (ptr == NULL)
		return malloc(size);
	
	// if ptr not NULL and size is 0, equivalent to free(ptr)
	if (!size)
	{
		free(ptr);
		return NULL;
	}
	
	void *new_ptr = malloc(size);
	
	size_t old_size = * (size_t*) (ptr - sizeof(block_meta));
	
	size_t min_size = MIN(old_size, size);
	
	memcpy(new_ptr, ptr, min_size);
	
	free(ptr);
	
	//printf("old size = %d, new size = %d, min size = %d\n", old_size, size, min_size);
	
	return new_ptr;
}
