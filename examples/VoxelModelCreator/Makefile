# This Makefile was generated using MakeGen v1.1.3 made by Tim Håkansson
# and is licensed under MIT. Full source of the project can be found at
# https://github.com/Thraix/MakeGen
CC=@g++
CO=@g++ -o
MKDIR_P=mkdir -p
BIN=bin/
OBJPATH=$(BIN)intermediates
INCLUDES=-I./../../Greet-core/src/ -I./../../deps/includes/ 
OBJECTS=$(OBJPATH)/Cube.o $(OBJPATH)/Grid.o $(OBJPATH)/GridRenderer3D.o $(OBJPATH)/ModelExport.o $(OBJPATH)/Ray.o $(OBJPATH)/main.o $(OBJPATH)/ColorTool.o $(OBJPATH)/PlaceTool.o 
CFLAGS=$(INCLUDES) -std=c++17 -c -w -g3 -D_DEBUG -g 
LIBDIR=-L./../../Greet-core/bin/ 
LDFLAGS=-Wl,-rpath=../../Greet-core/bin/ 
LIBS=$(LIBDIR) -lgreet -lGL -lGLEW -lglfw -lfreeimage -lfreetype 
OUTPUT=$(BIN)Sandbox.a
.PHONY: all directories rebuild clean dependencies
all: dependencies directories $(OUTPUT)
dependencies:
directories: $(BIN) $(OBJPATH)
$(BIN):
	$(info Creating output directories)
	@$(MKDIR_P) $(BIN)
$(OBJPATH):
	@$(MKDIR_P) $(OBJPATH)
rebuild: clean all
clean:
	$(info Removing intermediates)
	rm -rf $(OBJPATH)/*.o
$(OUTPUT): $(OBJECTS)
	$(info Generating output file)
	$(CO) $(OUTPUT) $(OBJECTS) $(LDFLAGS) $(LIBS)
install: all
	$(info Installing Sandbox to /usr/bin/)
	@cp $(OUTPUT) /usr/bin/Sandbox.a
$(OBJPATH)/Cube.o : src/Cube.cpp src/Cube.h
	$(info -[12%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Grid.o : src/Grid.cpp src/Grid.h src/Cube.h src/GridRenderer3D.h  src/ModelExport.h  src/Ray.h  src/tools/ToolBox.h src/tools/ColorTool.h src/tools/Tool.h src/tools/PlaceTool.h  
	$(info -[25%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/GridRenderer3D.o : src/GridRenderer3D.cpp src/GridRenderer3D.h src/Cube.h
	$(info -[37%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/ModelExport.o : src/ModelExport.cpp src/Grid.h src/Cube.h src/GridRenderer3D.h  src/ModelExport.h  src/Ray.h  src/tools/ToolBox.h src/tools/ColorTool.h src/tools/Tool.h src/tools/PlaceTool.h   
	$(info -[50%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/Ray.o : src/Ray.cpp src/Ray.h src/Cube.h
	$(info -[62%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/main.o : src/main.cpp src/Grid.h src/Cube.h src/GridRenderer3D.h  src/ModelExport.h  src/Ray.h  src/tools/ToolBox.h src/tools/ColorTool.h src/tools/Tool.h src/tools/PlaceTool.h  
	$(info -[75%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/ColorTool.o : src/tools/ColorTool.cpp src/tools/ColorTool.h src/tools/Tool.h
	$(info -[87%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/PlaceTool.o : src/tools/PlaceTool.cpp src/tools/PlaceTool.h src/tools/Tool.h
	$(info -[100%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
