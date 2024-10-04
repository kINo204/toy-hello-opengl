target := hello_gl.exe
libs := lib/glad/gl.c lib/stb_image.c
devkitdir := C:/Users/kINo/OneDrive/Apps/Cdevkits/w64devkit/x86_64-w64-mingw32
libdir := $(devkitdir)/lib
incdir := $(devkitdir)/include

all: $(target)

run: all
	@$(target)

hello_gl.exe: $(wildcard *.c)
	@gcc -o $@ $^ $(libs) -I $(incdir) $(libdir)/libglfw3dll.a -lgdi32

.PHONY: clean
clean:
	rm $(wildcard *.exe)