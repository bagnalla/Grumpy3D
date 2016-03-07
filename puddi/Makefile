CC = gcc
CPP = g++
OPTIONS = -std=c++11 -fopenmp -O3
DEBUGOPTIONS = -std=c++11 -fopenmp -Wall -g
RELEASEDIR = release
DEBUGDIR = debug
OBJDIR = obj
INCLUDEDIR = include
SRCDIR = src

releaseobjects = $(addprefix $(RELEASEDIR)/$(OBJDIR)/, Camera.o CRSpline.o Cube.o \
DrawableObject.o EnvironmentMap.o FpsTracker.o Puddi.o Geometry.o \
HeightMap.o HeightMapTerrain.o InitShader.o LightSource.o Material.o \
ModelGraph.o Object.o Rectangle.o RenderGraph.o RenderNode.o \
Schematic.o SDLUtil.o Shader.o Shadow.o Skybox.o Sphere.o \
TerrainVertexMesh.o Texture.o UpdateGraph.o Util.o VertexMesh.o Font.o )

debugobjects = $(addprefix $(DEBUGDIR)/$(OBJDIR)/, Camera.o CRSpline.o Cube.o \
DrawableObject.o EnvironmentMap.o FpsTracker.o Puddi.o Geometry.o \
HeightMap.o HeightMapTerrain.o InitShader.o LightSource.o Material.o \
ModelGraph.o Object.o Rectangle.o RenderGraph.o RenderNode.o \
Schematic.o SDLUtil.o Shader.o Shadow.o Skybox.o Sphere.o \
TerrainVertexMesh.o Texture.o UpdateGraph.o Util.o VertexMesh.o Font.o )

Release: releasedirs $(releaseobjects)
	ar rcs $(RELEASEDIR)/libpuddi.a $(releaseobjects)

Debug: debugdirs $(debugobjects)
	ar rcs $(DEBUGDIR)/libpuddi.a $(debugobjects)

$(RELEASEDIR)/$(OBJDIR)/%.o: $(SRCDIR)/%.cc $(INCLUDEDIR)/%.h
	$(CPP) $< -c $(OPTIONS) -I$(INCLUDEDIR) -o $@

$(DEBUGDIR)/$(OBJDIR)/%.o: $(SRCDIR)/%.cc $(INCLUDEDIR)/%.h
	$(CPP) $< -c $(DEBUGOPTIONS) -I$(INCLUDEDIR) -o $@

releasedirs:
	mkdir -p $(RELEASEDIR)
	mkdir -p $(RELEASEDIR)/$(OBJDIR)

debugdirs:
	mkdir -p $(DEBUGDIR)
	mkdir -p $(DEBUGDIR)/$(OBJDIR)

clean: cleanRelease cleanDebug

cleanRelease:
	rm -f $(RELEASEDIR)/libpuddi.a $(releaseobjects)

cleanDebug:
	rm -f $(DEBUGDIR)/libpuddi.a $(debugobjects)
