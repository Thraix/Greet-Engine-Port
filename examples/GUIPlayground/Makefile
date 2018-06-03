CC=g++
PLATFORM=unix
OBJPATH=bin/intermediates
SOURCES=$(wildcard src/*.cpp) $(wildcard src/**/*.cpp) $(wildcard src/**/**/*.cpp) $(wildcard src/**/**/**/*.cpp)
OBJECTS=$(addprefix $(OBJPATH)/, $(notdir $(SOURCES:.cpp=.o)))
INCLUDES=-I../../deps/includes/ -I ../../Greet-core/src/
LIBDIR=-L../../deps/libs/$(PLATFORM)/ -L../../Greet-core/bin/

CFLAGS=$(INCLUDES) -std=c++17 -c -w -g3
LIBS=$(LIBDIR) -l:libgreet.a -lGL -l:libGLEW.so -l:libglfw.so -l:libfreeimage.so -l:libfreetype-gl.a -l:libfreetype.so 
EXECUTABLE=bin/Sandbox.a
PRINT=${info Compiling $<}

all: $(EXECUTABLE)
	$(info Done :D)
rebuild: clean all

$(EXECUTABLE): $(OBJECTS) ../../Greet-core/bin/libgreet.a
	$(info Linking libraries)
	$(CC) -o $(EXECUTABLE) $(OBJECTS) $(LIBS)

$(OBJPATH)/%.o: src/%.cpp
	$(PRINT)
	$(CC) $(CFLAGS) -o $@ $<

$(OBJPATH)/%.o: src/**/%.cpp
	$(PRINT)
	$(CC) $(CFLAGS) -o $@ $<

$(OBJPATH)/%.o: src/**/**/%.cpp
	$(PRINT)
	$(CC) $(CFLAGS) -o $@ $<

$(OBJPATH)/%.o: src/**/**/**/%.cpp
	$(PRINT)
	$(CC) $(CFLAGS) -o $@ $<


clean:
	$(info Removing intermediates)
	rm -rf $(OBJPATH)/*
	rm -f $(EXECUTABLE)
