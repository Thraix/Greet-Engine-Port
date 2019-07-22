# This Makefile was generated using MakeGen v1.1.3 made by Tim Håkansson
# and is licensed under MIT. Full source of the project can be found at
# https://github.com/Thraix/MakeGen
CC=@g++
CO=@g++ -o
MKDIR_P=mkdir -p
BIN=bin/
OBJPATH=$(BIN)intermediates
INCLUDES=-I./../../Greet-core/src/ -I./../../deps/includes/ 
OBJECTS=$(OBJPATH)/Chunk.o $(OBJPATH)/main.o 
CFLAGS=$(INCLUDES) -std=c++17 -c -w -g3 -D_DEBUG -D_GREET_HOTSWAP -g 
LIBDIR=-L./../../Greet-core/bin/ 
LDFLAGS=-Wl,-rpath=../../Greet-core/bin/ 
LIBS=$(LIBDIR) -lgreet -lGL -lGLEW -lglfw -lfreeimage -lfreetype 
DEPENDENCIES=/home/thraix/Documents/Greet-Engine-Port/Greet-core/ 
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
$(OBJPATH)/Chunk.o : src/Chunk.cpp ../../Greet-core/src/graphics/models/MeshFactory.h ../../Greet-core/src/graphics/models/MeshData.h ../../Greet-core/src/internal/GreetGL.h ../../Greet-core/src/logging/Log.h ../../Greet-core/src/logging/LogLevel.h ../../Greet-core/src/logging/Logger.h  ../../Greet-core/src/logging/LogStream.h  ../../Greet-core/src/utils/LogUtils.h ../../Greet-core/src/internal/GreetTypes.h   ../../Greet-core/src/math/Maths.h ../../Greet-core/src/math/Mat3.h ../../Greet-core/src/math/Vec2.h ../../Greet-core/src/math/Vec3.h ../../Greet-core/src/math/MathFunc.h  ../../Greet-core/src/math/Quaternion.h ../../Greet-core/src/math/Vec4.h   ../../Greet-core/src/math/Mat4.h      ../../Greet-core/src/math/Transform.h      src/Chunk.h ../../Greet-core/src/graphics/models/Mesh.h ../../Greet-core/src/graphics/buffers/Buffer.h  ../../Greet-core/src/graphics/buffers/VertexArray.h    ../../Greet-core/src/utils/Noise.h 
	$(info -[50%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
$(OBJPATH)/main.o : src/main.cpp ../../Greet-core/src/graphics/renderers/GUIRenderer.h ../../Greet-core/src/graphics/fonts/Font.h ../../Greet-core/src/graphics/fonts/FontAtlas.h ../../Greet-core/src/graphics/textures/Texture2D.h ../../Greet-core/src/graphics/textures/Texture.h ../../Greet-core/src/internal/GreetGL.h ../../Greet-core/src/logging/Log.h ../../Greet-core/src/logging/LogLevel.h ../../Greet-core/src/logging/Logger.h  ../../Greet-core/src/logging/LogStream.h  ../../Greet-core/src/utils/LogUtils.h ../../Greet-core/src/internal/GreetTypes.h   ../../Greet-core/src/utils/HotSwapping.h ../../Greet-core/src/utils/FileUtils.h  ../../Greet-core/src/utils/ImageUtils.h     ../../Greet-core/src/math/Vec4.h   ../../Greet-core/src/graphics/renderers/Renderer2D.h ../../Greet-core/src/graphics/Sprite.h  ../../Greet-core/src/graphics/textures/TextureManager.h ../../Greet-core/src/graphics/textures/CubeMap.h       ../../Greet-core/src/math/Maths.h ../../Greet-core/src/math/Mat3.h ../../Greet-core/src/math/Vec2.h ../../Greet-core/src/math/Vec3.h ../../Greet-core/src/math/MathFunc.h  ../../Greet-core/src/math/Quaternion.h    ../../Greet-core/src/math/Mat4.h      ../../Greet-core/src/math/Transform.h           src/Greet.h src/Chunk.h ../../Greet-core/src/graphics/models/Mesh.h ../../Greet-core/src/graphics/buffers/Buffer.h  ../../Greet-core/src/graphics/buffers/VertexArray.h  ../../Greet-core/src/graphics/models/MeshData.h     ../../Greet-core/src/utils/Noise.h  src/Portal.h ../../Greet-core/src/graphics/models/EntityModel.h ../../Greet-core/src/graphics/models/Material.h ../../Greet-core/src/graphics/models/Camera.h ../../Greet-core/src/event/Event.h ../../Greet-core/src/utils/Utils.h    ../../Greet-core/src/graphics/shaders/Shader.h     ../../Greet-core/src/graphics/shaders/ShaderFactory.h  ../../Greet-core/src/graphics/shaders/Uniformable.h ../../Greet-core/src/graphics/shaders/uniforms/Uniform.h   ../../Greet-core/src/utils/ColorUtils.h     ../../Greet-core/src/graphics/models/MeshFactory.h     src/World.h ../../Greet-core/src/graphics/layers/Scene.h  ../../Greet-core/src/event/WindowEvent.h     src/keyboardcontrol.h   src/lensflare.h src/tree.h     
	$(info -[100%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
