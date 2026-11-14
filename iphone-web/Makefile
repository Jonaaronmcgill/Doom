# Build linuxdoom-1.10 for Safari / iPhone via Emscripten

EMSDK_ENV ?= $(HOME)/emsdk/emsdk_env.sh
CC       := emcc
ROOT     := $(abspath ..)
SRC_GAME := $(ROOT)/linuxdoom-1.10
SRC_WEB  := src
INC      := -Iinclude -I$(SRC_GAME)

CFLAGS := -O2 -std=gnu89 -Wall -DNORMALUNIX -DLINUX \
	$(INC) \
	-Wno-unused-variable -Wno-unused-but-set-variable \
	-Wno-pointer-sign -Wno-implicit-function-declaration \
	-Wno-dangling-else -Wno-string-plus-int \
	-Wno-implicit-int -Wno-return-type -Wno-deprecated-non-prototype

LDFLAGS := -O2 \
	-sWASM=1 \
	-sALLOW_MEMORY_GROWTH=1 \
	-sINITIAL_MEMORY=67108864 \
	-sMODULARIZE=1 \
	-sEXPORT_NAME=createDoomModule \
	-sEXPORTED_FUNCTIONS='["_main","_I_PushKey","_I_GetFrameBuffer","_I_GetScreenWidth","_I_GetScreenHeight","_malloc","_free"]' \
	-sEXPORTED_RUNTIME_METHODS='["ccall","cwrap","UTF8ToString","HEAPU8","HEAPU32"]' \
	-sFORCE_FILESYSTEM=1 \
	-sNO_EXIT_RUNTIME=1 \
	-sENVIRONMENT=web \
	-sSTACK_SIZE=1048576 \
	--preload-file wad/doom1.wad@doom1.wad

GAME_SRCS := $(shell cat game_sources.txt | grep -v '^d_main.c$$')
GAME_OBJS := $(patsubst %.c,build/game/%.o,$(GAME_SRCS))

WEB_SRCS := i_main.c i_system.c i_video.c i_sound.c i_net.c d_main.c w_wad.c
WEB_OBJS := $(patsubst %.c,build/web/%.o,$(WEB_SRCS))

.PHONY: all clean serve fetch-wad

all: fetch-wad dist/doom.js dist/index.html

fetch-wad: wad/doom1.wad

wad/doom1.wad:
	@mkdir -p wad
	@echo "Downloading Freedoom Phase 1 IWAD..."
	curl -fsSL -L -o /tmp/freedoom.zip https://github.com/freedoom/freedoom/releases/download/v0.13.0/freedoom-0.13.0.zip
	unzip -p /tmp/freedoom.zip freedoom-0.13.0/freedoom1.wad > wad/doom1.wad
	@ls -lh wad/doom1.wad


dist/doom.js: $(GAME_OBJS) $(WEB_OBJS) | dist
	$(CC) $(WEB_OBJS) $(GAME_OBJS) $(LDFLAGS) -o dist/doom.js

dist/index.html: index.html styles.css app.js | dist
	cp index.html styles.css app.js dist/

build/game/%.o: $(SRC_GAME)/%.c | build/game
	$(CC) $(CFLAGS) -c $< -o $@

build/web/%.o: $(SRC_WEB)/%.c | build/web
	$(CC) $(CFLAGS) -c $< -o $@

build/game build/web dist:
	mkdir -p $@

clean:
	rm -rf build dist/doom.js dist/doom.wasm dist/doom.data

serve: all
	cd dist && python3 -m http.server 8080 --bind 127.0.0.1
