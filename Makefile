CC = g++
OPTIONS = -std=c++11 -fopenmp -O3 -Ipuddi/include -Lpuddi/release
DEBUGOPTIONS = -std=c++11 -fopenmp -Wall -g -Ipuddi/include -Lpuddi/debug
LDLIBS = -lpuddi -lGLEW -lGL -lSDL2 -lpng -ljpeg -lz -lSDL2_image -lassimp
RELEASEDIR = release
DEBUGDIR = debug
OBJDIR = obj
INCLUDEDIR = include
SRCDIR = src

releaseobjects = $(addprefix $(RELEASEDIR)/$(OBJDIR)/, SourceCode.o Token.o Lexer.o AST.o SyntaxParser.o )

debugobjects = $(addprefix $(DEBUGDIR)/$(OBJDIR)/, SourceCode.o Token.o Lexer.o AST.o SyntaxParser.o )

Release: $(SRCDIR)/main.cc puddiRelease releasedirs $(releaseobjects)
	$(CC) $(SRCDIR)/main.cc $(releaseobjects) -I$(INCLUDEDIR) $(OPTIONS) $(LDLIBS) -o $(RELEASEDIR)/Grumpy3D

Debug: $(SRCDIR)/main.cc puddiDebug debugdirs $(debugobjects)
	$(CC) $(SRCDIR)/main.cc $(debugobjects) -I$(INCLUDEDIR) $(DEBUGOPTIONS) $(LDLIBS) -o $(DEBUGDIR)/Grumpy3D

$(RELEASEDIR)/$(OBJDIR)/%.o: $(SRCDIR)/%.cc $(INCLUDEDIR)/%.h
	$(CC) $< -c $(OPTIONS) -I$(INCLUDEDIR) $(LDLIBS) -o $@

$(DEBUGDIR)/$(OBJDIR)/%.o: $(SRCDIR)/%.cc $(INCLUDEDIR)/%.h
	$(CC) $< -c $(DEBUGOPTIONS) -I$(INCLUDEDIR) $(LDLIBS) -o $@

# for testing my malloc implementation
$(RELEASEDIR)/$(OBJDIR)/malloc.o: $(SRCDIR)/malloc.c
	gcc $< -g -c -o $@

$(DEBUGDIR)/$(OBJDIR)/malloc.o: $(SRCDIR)/malloc.c
	gcc $< -g -c -o $@

puddiRelease:
	$(MAKE) -C puddi Release

puddiDebug:
	$(MAKE) -C puddi Debug

releasedirs:
	mkdir -p $(RELEASEDIR)
	mkdir -p $(RELEASEDIR)/$(OBJDIR)

debugdirs:
	mkdir -p $(DEBUGDIR)
	mkdir -p $(DEBUGDIR)/$(OBJDIR)

clean: cleanRelease cleanDebug

cleanRelease:
	rm -f $(RELEASEDIR)/Grumpy3D $(releaseobjects)
	$(MAKE) -C puddi cleanRelease

cleanDebug:
	rm -f $(DEBUGDIR)/Grumpy3D $(debugobjects)
	$(MAKE) -C puddi cleanDebug
