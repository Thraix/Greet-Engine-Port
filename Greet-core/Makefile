CC=@g++
AR=@ar

OBJPATH=bin/intermediates
SOURCES=$(wildcard src/*.cpp) $(wildcard src/**/*.cpp) $(wildcard src/**/**/*.cpp) $(wildcard src/**/**/**/*.cpp)
OBJECTS=$(addprefix $(OBJPATH)/, $(notdir $(SOURCES:.cpp=.o)))
INCLUDES=-I../deps/includes/ -I src/
CFLAGS=$(INCLUDES) -std=c++17 -c -w -g3
LIBS=-lGL -l:libglfw.so -l:libfreeimage.so
EXECUTABLE=bin/libgreet.a
PRINT=${info Compiling $<}

all: $(EXECUTABLE)
	$(info Done :D)
rebuild: clean all


$(EXECUTABLE): $(OBJECTS)
	$(info Linking libraries)
	$(AR) rvs $(EXECUTABLE) $(OBJECTS)

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
