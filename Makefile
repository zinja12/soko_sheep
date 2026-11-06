# Project name
PROJECT_NAME ?= soko_sheep
# Source files
OBJS ?= $(wildcard *.c)

# Define raylib paths (adjust to where your raylib files are located)
RAYLIB_PATH ?= C:/raylib/raylib
RAYLIB_INCLUDE_PATH ?= $(RAYLIB_PATH)/src
RAYLIB_LIB_PATH ?= $(RAYLIB_PATH)/src

EMCC ?= emcc
WEB_DIR ?= build/web
RAYLIB_WEB_LIB ?= $(RAYLIB_PATH)/src/libraylib.web.a
WEB_SHELL_FILE ?= $(RAYLIB_PATH)/src/minshell.html
WEB_CFLAGS = -Os -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -DPLATFORM_WEB
WEB_CFLAGS += -I. -I$(RAYLIB_INCLUDE_PATH)
WEB_LDFLAGS = -sUSE_GLFW=3 -sALLOW_MEMORY_GROWTH=1
WEB_LDFLAGS += --preload-file assets --shell-file $(WEB_SHELL_FILE)

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result
CFLAGS += -I. -I$(RAYLIB_INCLUDE_PATH)

# Linker flags for Windows (Desktop)
LDFLAGS = -L. -L$(RAYLIB_LIB_PATH)
LDLIBS = -lraylib -lopengl32 -lgdi32 -lwinmm -lpthread

# Build target
all:
	$(CC) $(OBJS) -o $(PROJECT_NAME).exe $(CFLAGS) $(LDFLAGS) $(LDLIBS)

run: all
	./$(PROJECT_NAME).exe

clean:
	rm -f $(PROJECT_NAME).exe
	rm -rf $(WEB_DIR)

web:
	mkdir -p $(WEB_DIR)
	$(EMCC) $(OBJS) -o $(WEB_DIR)/index.html $(WEB_CFLAGS) $(RAYLIB_WEB_LIB) $(WEB_LDFLAGS)

serve: web
	emrun --no_browser --port 8080 $(WEB_DIR)/index.html

.PHONY: all run clean web serve
