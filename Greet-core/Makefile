CC=@g++
AR=@ar
BIN=bin
OBJPATH=$(BIN)/intermediates
SOURCES=$(wildcard src/*.cpp) $(wildcard src/**/*.cpp) $(wildcard src/**/**/*.cpp) $(wildcard src/**/**/**/*.cpp)
OBJECTS=$(addprefix $(OBJPATH)/, $(notdir $(SOURCES:.cpp=.o)))
INCLUDES=-I../deps/includes/ -I src/
CFLAGS=$(INCLUDES) -std=c++17 -c -w -g3 -D_DEBUG -D_USE_MATH_DEFINES
LIBS=-lGL -l:libglfw.so -l:libfreeimage.so
LIBRARY=$(BIN)/libgreet.a
PRINT=${info ---- $<}

all: $(LIBRARY)
	$(info ---------------------------------)
	$(info ----  Greet Engine Compiled  ----)
	$(info ---------------------------------)
rebuild: clean all


$(LIBRARY): $(OBJECTS)
	$(info Generating library file)
	$(AR) rs $(LIBRARY) $(OBJECTS)

$(OBJPATH)/%.o: src/%.cpp src/%.h
	$(PRINT)
	$(CC) $(CFLAGS) -o $@ $<

$(OBJPATH)/%.o: src/**/%.cpp src/**/%.h
	$(PRINT)
	$(CC) $(CFLAGS) -o $@ $<

$(OBJPATH)/%.o: src/**/**/%.cpp src/**/**/%.h
	$(PRINT)
	$(CC) $(CFLAGS) -o $@ $<

$(OBJPATH)/%.o: src/**/**/**/%.cpp src/**/**/**/%.h
	$(PRINT)
	$(CC) $(CFLAGS) -o $@ $<


clean:
	$(info Removing intermediates)
	rm -rf $(OBJPATH)/*
