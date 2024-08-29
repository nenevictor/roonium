libs = -lglfw3 -lm
include_directory = -Ivendor/include/
lib_directory = -Lvendor/lib
src = src/roonium.c
warnings = -Wall -Wextra -Werror -Wpedantic -Wfatal-errors

precompiled_objects = objects/glad.o objects/stb_image.o objects/roonmath.o

ifeq ($(OS),Windows_NT)
	libs += -lopengl32 -lgdi32 -lwinmm
endif
ifeq ($(OS),Linux)
	libs += -lGL
endif


packer:
	gcc -O3 -std=c99 $(warnings) src/packer.c -o packer

pack_resources:
	./packer rb resources/shader.vs src/shader.vs.h
	./packer rb resources/shader.fs src/shader.fs.h
	./packer rb resources/roon.jpg src/roon.h
	./packer rb resources/roon_icon.png src/roon_icon.h

# I precompiled some libraries to avoid ISO c90 errors.
precompile:
	gcc -O3 -std=c99 -fexceptions -H -g -c $(include_directory) vendor/src/glad.c -o objects/glad.o
	gcc -O3 -std=c99 -fexceptions -H -g -c $(include_directory) vendor/src/stb_image_precompile.c -o objects/stb_image.o
	gcc -O3 -std=c99 -fexceptions -H -g -c $(include_directory) vendor/src/roonmath_precompile.c -o objects/roonmath.o

debug:
	gcc -O0 -std=c89 $(warnings) $(include_directory) $(lib_directory) $(src) $(precompiled_objects) -o roonium $(libs)
	./roonium

product:
	make packer
	make pack_resources
	make precompile
	gcc -O3 -std=c89 $(include_directory) $(lib_directory) $(src) $(precompiled_objects) -o roonium $(libs)
