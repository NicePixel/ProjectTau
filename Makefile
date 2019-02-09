CC=g++

SRC_DIR=./src/
OBJ_DIR=./obj/
OBJ_DIRMAKE=obj
OBJECT_FILES=$(OBJ_DIR)eng_camera.o $(OBJ_DIR)eng_entry.o $(OBJ_DIR)eng_io.o $(OBJ_DIR)eng_object.o $(OBJ_DIR)eng_print.o $(OBJ_DIR)eng_render.o $(OBJ_DIR)eng_timer.o $(OBJ_DIR)eng_ogl_exception.o $(OBJ_DIR)eng_ogl_font.o $(OBJ_DIR)eng_ogl_framebuffer.o $(OBJ_DIR)eng_ogl_main.o $(OBJ_DIR)eng_ogl_mesh.o $(OBJ_DIR)eng_ogl_render.o $(OBJ_DIR)eng_ogl_shader.o $(OBJ_DIR)eng_ogl_texture.o $(OBJ_DIR)gam_main.o $(OBJ_DIR)gam_world.o $(OBJ_DIR)gam_world_load.o

LIB_WINDOWS=-L./lib/ -lmingw32 -lSDL2 -lSDL2_image -lopengl32 -lglew32 -lfreetype
LIB=-lSDL2 -lSDL2_image -lopengl32 -lglew32 -lfreetype
INCLUDE=-I./ -I./include/

LFLAGS=
CFLAGS=-std=c++17 -Wall -g -DGLM_ENABLE_EXPERIMENTAL -DTED_OGL $(INCLUDE)

BIN_DIR=./bin/
BIN_DIRMAKE=bin
BIN=program

# This compiles the whole program.
#$(CC) -Wall -DGLM_ENABLE_EXPERIMENTAL -DTED_OGL src/eng/camera.cpp src/eng/entry.cpp src/eng/io.cpp src/eng/object.cpp src/eng/print.cpp src/eng/render.cpp src/eng/timer.cpp src/eng/ogl/ogl_exception.cpp src/eng/ogl/ogl_font.cpp src/eng/ogl/ogl_framebuffer.cpp src/eng/ogl/ogl_main.cpp src/eng/ogl/ogl_mesh.cpp src/eng/ogl/ogl_render.cpp src/eng/ogl/ogl_shader.cpp src/eng/ogl/ogl_test.cpp src/eng/ogl/ogl_texture.cpp src/gam/main.cpp -o bin -I./ -I./include/ -L./lib/ -lmingw32 -lSDL2 -lSDL2_image -lopengl32 -lglew32 -lfreetype

all:  program
	@echo ==========DONE==========
	
$(OBJ_DIR)eng_camera.o: $(SRC_DIR)eng/camera.cpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)eng/camera.cpp -o $(OBJ_DIR)eng_camera.o
$(OBJ_DIR)eng_entry.o: $(SRC_DIR)eng/entry.cpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)eng/entry.cpp -o $(OBJ_DIR)eng_entry.o
$(OBJ_DIR)eng_io.o: $(SRC_DIR)eng/io.cpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)eng/io.cpp -o $(OBJ_DIR)eng_io.o
$(OBJ_DIR)eng_object.o: $(SRC_DIR)eng/object.cpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)eng/object.cpp -o $(OBJ_DIR)eng_object.o
$(OBJ_DIR)eng_print.o: $(SRC_DIR)eng/print.cpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)eng/print.cpp -o $(OBJ_DIR)eng_print.o
$(OBJ_DIR)eng_render.o: $(SRC_DIR)eng/render.cpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)eng/render.cpp -o $(OBJ_DIR)eng_render.o
$(OBJ_DIR)eng_timer.o: $(SRC_DIR)eng/timer.cpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)eng/timer.cpp -o $(OBJ_DIR)eng_timer.o
	
$(OBJ_DIR)eng_ogl_exception.o: $(SRC_DIR)eng/ogl/ogl_exception.cpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)eng/ogl/ogl_exception.cpp -o $(OBJ_DIR)eng_ogl_exception.o
$(OBJ_DIR)eng_ogl_font.o: $(SRC_DIR)eng/ogl/ogl_font.cpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)eng/ogl/ogl_font.cpp -o $(OBJ_DIR)eng_ogl_font.o
$(OBJ_DIR)eng_ogl_framebuffer.o: $(SRC_DIR)eng/ogl/ogl_framebuffer.cpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)eng/ogl/ogl_framebuffer.cpp -o $(OBJ_DIR)eng_ogl_framebuffer.o
$(OBJ_DIR)eng_ogl_main.o: $(SRC_DIR)eng/ogl/ogl_main.cpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)eng/ogl/ogl_main.cpp -o $(OBJ_DIR)eng_ogl_main.o
$(OBJ_DIR)eng_ogl_mesh.o: $(SRC_DIR)eng/ogl/ogl_mesh.cpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)eng/ogl/ogl_mesh.cpp -o $(OBJ_DIR)eng_ogl_mesh.o
$(OBJ_DIR)eng_ogl_render.o: $(SRC_DIR)eng/ogl/ogl_render.cpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)eng/ogl/ogl_render.cpp -o $(OBJ_DIR)eng_ogl_render.o
$(OBJ_DIR)eng_ogl_shader.o: $(SRC_DIR)eng/ogl/ogl_shader.cpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)eng/ogl/ogl_shader.cpp -o $(OBJ_DIR)eng_ogl_shader.o
$(OBJ_DIR)eng_ogl_texture.o: $(SRC_DIR)eng/ogl/ogl_texture.cpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)eng/ogl/ogl_texture.cpp -o $(OBJ_DIR)eng_ogl_texture.o
	
$(OBJ_DIR)gam_main.o: $(SRC_DIR)gam/main.cpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)gam/main.cpp -o $(OBJ_DIR)gam_main.o
$(OBJ_DIR)gam_world.o: $(SRC_DIR)gam/world.cpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)gam/world.cpp -o $(OBJ_DIR)gam_world.o
$(OBJ_DIR)gam_world_load.o: $(SRC_DIR)gam/world_load.cpp
	$(CC) $(CFLAGS) -c $(SRC_DIR)gam/world_load.cpp -o $(OBJ_DIR)gam_world_load.o


.PHONY: clean
clean:
	rm $(OBJ_DIR)*.o

# ================
# Ran before anything else
# ================
.PHONY: prepare
prepare:
	echo "job PREPARE"
	-@mkdir $(OBJ_DIRMAKE)
	-@mkdir $(BIN_DIRMAKE)
	
# ================
# Default build
# ================
.PHONY: program
program: prepare $(OBJ_DIR)eng_camera.o $(OBJ_DIR)eng_entry.o $(OBJ_DIR)eng_io.o $(OBJ_DIR)eng_object.o $(OBJ_DIR)eng_print.o $(OBJ_DIR)eng_render.o $(OBJ_DIR)eng_timer.o $(OBJ_DIR)eng_ogl_exception.o $(OBJ_DIR)eng_ogl_font.o $(OBJ_DIR)eng_ogl_framebuffer.o $(OBJ_DIR)eng_ogl_main.o $(OBJ_DIR)eng_ogl_mesh.o $(OBJ_DIR)eng_ogl_render.o $(OBJ_DIR)eng_ogl_shader.o $(OBJ_DIR)eng_ogl_texture.o $(OBJ_DIR)gam_main.o $(OBJ_DIR)gam_world.o $(OBJ_DIR)gam_world_load.o
	$(CC) $(LFLAGS) $(OBJECT_FILES) -o $(BIN_DIR)$(BIN) $(LIB)

# ================
# Microsoft Windows build
# Using LIB_WINDOWS
# ================
.PHONY: program_windows
program_windows: prepare $(OBJ_DIR)eng_camera.o $(OBJ_DIR)eng_entry.o $(OBJ_DIR)eng_io.o $(OBJ_DIR)eng_object.o $(OBJ_DIR)eng_print.o $(OBJ_DIR)eng_render.o $(OBJ_DIR)eng_timer.o $(OBJ_DIR)eng_ogl_exception.o $(OBJ_DIR)eng_ogl_font.o $(OBJ_DIR)eng_ogl_framebuffer.o $(OBJ_DIR)eng_ogl_main.o $(OBJ_DIR)eng_ogl_mesh.o $(OBJ_DIR)eng_ogl_render.o $(OBJ_DIR)eng_ogl_shader.o $(OBJ_DIR)eng_ogl_texture.o $(OBJ_DIR)gam_main.o $(OBJ_DIR)gam_world.o $(OBJ_DIR)gam_world_load.o
	$(CC) $(LFLAGS) $(OBJECT_FILES) -o $(BIN_DIR)$(BIN) $(LIB_WINDOWS)
