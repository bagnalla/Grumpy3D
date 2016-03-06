CC = clang
CPP = clang++
OPTIONS = -std=c++11 -fopenmp -O3 -Ipuddi/include
DEBUGOPTIONS = -std=c++11 -fopenmp -Wall -g -Ipuddi/include
LDLIBS = -lpuddi -lGLEW -lGL -lSDL2 -lpng -ljpeg -lz -lSDL2_image -lassimp
RELEASEDIR = release
DEBUGDIR = debug
OBJDIR = obj
INCLUDEDIR = include
SRCDIR = src
LINKDIR = puddi/release
DEBUGLINKDIR = puddi/debug

releaseobjects = $(addprefix $(RELEASEDIR)/$(OBJDIR)/, SourceCode.o Token.o Lexer.o AST.o SyntaxParser.o GrumpyConfig.o )

debugobjects = $(addprefix $(DEBUGDIR)/$(OBJDIR)/, SourceCode.o Token.o Lexer.o AST.o SyntaxParser.o GrumpyConfig.o )

Release: $(SRCDIR)/main.cc puddiRelease releasedirs $(releaseobjects)
	$(CPP) $(SRCDIR)/main.cc $(releaseobjects) -I$(INCLUDEDIR) -L$(LINKDIR) $(OPTIONS) $(LDLIBS) -o $(RELEASEDIR)/Grumpy3D

Debug: $(SRCDIR)/main.cc puddiDebug debugdirs $(debugobjects)
	$(CPP) $(SRCDIR)/main.cc $(debugobjects) -I$(INCLUDEDIR) -L$(DEBUGLINKDIR) $(DEBUGOPTIONS) $(LDLIBS) -o $(DEBUGDIR)/Grumpy3D

$(RELEASEDIR)/$(OBJDIR)/%.o: $(SRCDIR)/%.cc $(INCLUDEDIR)/%.h
	$(CPP) $< -c $(OPTIONS) -I$(INCLUDEDIR) -o $@

$(DEBUGDIR)/$(OBJDIR)/%.o: $(SRCDIR)/%.cc $(INCLUDEDIR)/%.h
	$(CPP) $< -c $(DEBUGOPTIONS) -I$(INCLUDEDIR) -o $@

# for testing my malloc implementation
$(RELEASEDIR)/$(OBJDIR)/malloc.o: $(SRCDIR)/malloc.c
	$(CC) $< -g -c -o $@

$(DEBUGDIR)/$(OBJDIR)/malloc.o: $(SRCDIR)/malloc.c
	$(CC) $< -g -c -o $@

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
