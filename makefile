target := hello_gl.exe
libs := lib/glad/gl.c

devkitdir := C:/Users/kINo/OneDrive/Apps/Cdevkits/w64devkit/x86_64-w64-mingw32
libdir := $(devkitdir)/lib
incdir := $(devkitdir)/include

res := -mwindows res/icons/icon.o

all: $(target) shaders 

run: all
	@$(target)

hello_gl.exe: $(wildcard *.c) $(wildcard lib/*.c) $(libs)
	@echo building ...
	@gcc -o $@ $^ -I $(incdir) $(libdir)/libglfw3dll.a -lgdi32
# $(res)

shaders: $(wildcard shaders/*.vert) $(wildcard shaders/*.frag)

.PHONY: clean
clean:
	rm $(wildcard *.exe)