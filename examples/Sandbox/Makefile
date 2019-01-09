# This Makefile was generated using MakeGen v1.0.7 made by Tim Håkansson
# and is licensed under MIT. Full source of the project can be found at
# https://github.com/Thraix/MakeGen
CC=@g++ $(CFLAGS)
CO=@g++ -o
CFLAGS=-g 
BIN=bin/
OBJPATH=$(BIN)intermediates
INCLUDES=-I./../../Greet-core/src/ -I./../../deps/includes/ 
OBJECTS=$(OBJPATH)/main.o 
CFLAGS=$(INCLUDES) -std=c++17 -c -w -g3 -D_DEBUG 
LIBDIR=-L./../../deps/libs/unix/ -L./../../Greet-core/bin/ 
LIBS=$(LIBDIR) -lgreet -lGL -lGLEW -lglfw -lfreeimage -lfreetype 
OUTPUT=$(BIN)Sandbox.a
all: $(OUTPUT)
rebuild: clean all
clean:
	$(info Removing intermediates)
	rm -rf $(OBJPATH)/*.o
$(OUTPUT): $(OBJECTS)
	$(info Generating output file)
	$(CO) $(OUTPUT) $(OBJECTS) $(LIBS)
install: all
	$(info Installing Sandbox to /usr/bin/)
	@cp $(OUTPUT) /usr/bin/Sandbox.a
$(OBJPATH)/main.o : src/main.cpp src/keyboardcontrol.h src/tree.h
	$(info -[100%]- $<)
	$(CC) $(CFLAGS) -o $@ $<
