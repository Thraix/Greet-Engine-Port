# This Makefile was generated using MakeGen v1.1.3 made by Tim Håkansson
# and is licensed under MIT. Full source of the project can be found at
# https://github.com/Thraix/MakeGen
CC=@g++
CO=@g++ -o
MKDIR_P=mkdir -p
BIN=bin/
OBJPATH=$(BIN)intermediates
INCLUDES=-I./../../Greet-core/src/ -I./../../deps/includes/ 
OBJECTS=$(OBJPATH)/GuiTesting.o 
CFLAGS=$(INCLUDES) -std=c++17 -c -w -g3 -D_DEBUG -g 
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
$(OBJPATH)/GuiTesting.o : src/GuiTesting.cpp ../../Greet-core/src/Greet.h  ../../Greet-core/src/audio/AudioChannel.h ../../Greet-core/src/utils/StringUtils.h ../../Greet-core/src/audio/AudioChannelManager.h  ../../Greet-core/src/audio/Sound.h   ../../Greet-core/src/logging/Log.h ../../Greet-core/src/logging/LogLevel.h ../../Greet-core/src/logging/Logger.h  ../../Greet-core/src/logging/LogStream.h  ../../Greet-core/src/audio/SoundManager.h  ../../Greet-core/src/internal/GreetTypes.h ../../Greet-core/src/utils/ErrorHandle.h  ../../Greet-core/src/utils/Utils.h ../../Greet-core/src/internal/GreetGL.h  ../../Greet-core/src/utils/LogUtils.h    ../../Greet-core/src/drivers/Driver.h ../../Greet-core/src/drivers/DriverListener.h  ../../Greet-core/src/drivers/Driver1.h  ../../Greet-core/src/drivers/Driver2.h  ../../Greet-core/src/drivers/Driver4.h  ../../Greet-core/src/drivers/DriverAdapter.h  ../../Greet-core/src/drivers/DriverDispatcher.h   ../../Greet-core/src/drivers/LinearDriver.h  ../../Greet-core/src/drivers/RectDriver.h  ../../Greet-core/src/event/Event.h  ../../Greet-core/src/event/EventDispatcher.h  ../../Greet-core/src/event/JoystickEvent.h  ../../Greet-core/src/math/Maths.h ../../Greet-core/src/math/Mat3.h ../../Greet-core/src/math/Vec2.h ../../Greet-core/src/math/Vec3.h ../../Greet-core/src/math/MathFunc.h  ../../Greet-core/src/math/Quaternion.h ../../Greet-core/src/math/Vec4.h   ../../Greet-core/src/math/Mat4.h      ../../Greet-core/src/math/Transform.h     ../../Greet-core/src/event/KeyEvent.h   ../../Greet-core/src/event/MouseEvent.h    ../../Greet-core/src/event/WindowEvent.h  ../../Greet-core/src/graphics/Animation.h ../../Greet-core/src/graphics/Sprite.h ../../Greet-core/src/graphics/textures/Texture2D.h ../../Greet-core/src/graphics/textures/Texture.h   ../../Greet-core/src/utils/HotSwapping.h ../../Greet-core/src/utils/FileUtils.h  ../../Greet-core/src/utils/ImageUtils.h   ../../Greet-core/src/graphics/textures/TextureManager.h ../../Greet-core/src/graphics/textures/CubeMap.h         ../../Greet-core/src/graphics/FrameBufferObject.h   ../../Greet-core/src/graphics/Light.h ../../Greet-core/src/graphics/shaders/Shader.h      ../../Greet-core/src/graphics/ParticlePool.h ../../Greet-core/src/graphics/RenderEngine.h  ../../Greet-core/src/graphics/layers/Scene.h   ../../Greet-core/src/graphics/Renderable.h ../../Greet-core/src/graphics/renderers/Renderer2D.h  ../../Greet-core/src/graphics/fonts/Font.h ../../Greet-core/src/graphics/fonts/FontAtlas.h         ../../Greet-core/src/graphics/Renderable2D.h     ../../Greet-core/src/graphics/RenderablePoly.h  ../../Greet-core/src/graphics/Skybox.h ../../Greet-core/src/graphics/cameras/Camera.h   ../../Greet-core/src/graphics/Window.h ../../Greet-core/src/input/Joystick.h       ../../Greet-core/src/graphics/models/Mesh.h ../../Greet-core/src/graphics/buffers/Buffer.h  ../../Greet-core/src/graphics/buffers/VertexArray.h  ../../Greet-core/src/graphics/models/MeshData.h         ../../Greet-core/src/graphics/atlas/Atlas.h    ../../Greet-core/src/graphics/atlas/AtlasManager.h  ../../Greet-core/src/graphics/buffers/BatchIndexBuffer.h     ../../Greet-core/src/graphics/cameras/FPCamera.h  ../../Greet-core/src/graphics/cameras/TPCamera.h       ../../Greet-core/src/graphics/fonts/FontContainer.h   ../../Greet-core/src/graphics/fonts/FontManager.h  ../../Greet-core/src/graphics/gui/Button.h ../../Greet-core/src/graphics/gui/Component.h   ../../Greet-core/src/graphics/gui/ComponentFactory.h ../../Greet-core/src/utils/xml/XMLObject.h ../../Greet-core/src/graphics/gui/GUIScene.h     ../../Greet-core/src/graphics/renderers/GUIRenderer.h      ../../Greet-core/src/graphics/gui/Style.h ../../Greet-core/src/graphics/gui/TLBR.h ../../Greet-core/src/graphics/gui/GUIUtils.h  ../../Greet-core/src/utils/ColorUtils.h     ../../Greet-core/src/utils/xml/XML.h     ../../Greet-core/src/graphics/gui/Label.h    ../../Greet-core/src/graphics/gui/ColorPicker.h ../../Greet-core/src/graphics/gui/ColorPickerWindow.h ../../Greet-core/src/graphics/gui/Frame.h ../../Greet-core/src/graphics/gui/Container.h   ../../Greet-core/src/graphics/gui/GUIMouseListener.h  ../../Greet-core/src/graphics/gui/HueSlider.h ../../Greet-core/src/graphics/gui/Slider.h  ../../Greet-core/src/graphics/gui/SatValSlider.h  ../../Greet-core/src/graphics/gui/TextBox.h        ../../Greet-core/src/graphics/gui/Divider.h    ../../Greet-core/src/graphics/gui/GUIProperty.h     ../../Greet-core/src/graphics/gui/ProgressBar.h  ../../Greet-core/src/graphics/gui/RadioButton.h  ../../Greet-core/src/graphics/gui/RadioGroup.h        ../../Greet-core/src/graphics/layers/Group.h  ../../Greet-core/src/graphics/layers/Layer.h     ../../Greet-core/src/graphics/layers/Layer3D.h  ../../Greet-core/src/graphics/renderers/Renderer3D.h    ../../Greet-core/src/graphics/models/EntityModel.h ../../Greet-core/src/graphics/models/Material.h   ../../Greet-core/src/graphics/shaders/ShaderFactory.h  ../../Greet-core/src/graphics/shaders/Uniformable.h ../../Greet-core/src/graphics/shaders/uniforms/Uniform.h            ../../Greet-core/src/graphics/models/MeshFactory.h   ../../Greet-core/src/graphics/renderers/BatchRenderer.h      ../../Greet-core/src/graphics/renderers/BatchRenderer3D.h       ../../Greet-core/src/utils/OBJUtils.h           ../../Greet-core/src/graphics/textures/ImageFactory.h      ../../Greet-core/src/internal/App.h   ../../Greet-core/src/utils/Timer.h                ../../Greet-core/src/utils/AABBUtils.h       ../../Greet-core/src/utils/Loaders.h  ../../Greet-core/src/utils/Noise.h     ../../Greet-core/src/utils/UUID.h   ../../Greet-core/src/utils/json/JSON.h   ../../Greet-core/src/utils/json/JSONArray.h  ../../Greet-core/src/utils/json/JSONLoader.h  ../../Greet-core/src/utils/json/JSONObject.h     ../../Greet-core/src/utils/xml/XMLException.h  
	$(info -[100%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
