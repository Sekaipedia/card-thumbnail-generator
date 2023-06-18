GCC = gcc
MINGW = x86_64-w64-mingw32-gcc

BUILD_DIR = build
BUILD_DIR_SHARED = $(BUILD_DIR)/shared
BUILD_DIR_LINUX = $(BUILD_DIR)/linux
BUILD_DIR_WIN = $(BUILD_DIR)/win
DIST_DIR = dist
IMAGE_DIR = src/images

VIPS_DEV_DIR_WIN = vips-dev

.DEFAULT_GOAL = build

build: generate_cmdline generate_assets linux_main.o linux_cmdline.o
	$(GCC) $(BUILD_DIR)/*.o `pkg-config vips --libs` -o $(DIST_DIR)/card_thumbnail_generator

build_win: generate_cmdline generate_assets win_main.o win_cmdline.o
	$(MINGW) $(BUILD_DIR)/*.o -L$(VIPS_DEV_DIR_WIN)/lib `pkg-config vips --libs` -o $(DIST_DIR)/card_thumbnail_generator.exe

generate_cmdline:
	gengetopt < ./gengetopt.config.ggo --output-dir src

generate_assets: $(notdir $(wildcard $(IMAGE_DIR)/*.png))

%.png: $(IMAGE_DIR)/%.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386:x86-64 $< $(BUILD_DIR)/$(notdir $(basename $<)).o

linux_main.o:
	$(GCC) -c -Wall src/main.c `pkg-config vips --cflags` -o $(BUILD_DIR)/main.o

linux_cmdline.o:
	$(GCC) -c -Wall src/cmdline.c -o $(BUILD_DIR)/cmdline.o

win_main.o:
	$(MINGW) -I$(VIPS_DEV_DIR_WIN)/include -I$(VIPS_DEV_DIR_WIN)/include/glib-2.0 -I$(VIPS_DEV_DIR_WIN)/lib/glib-2.0/include -c -Wall src/main.c -mms-bitfields -o $(BUILD_DIR)/main.o

win_cmdline.o:
	$(MINGW) -c -Wall src/cmdline.c -o $(BUILD_DIR)/cmdline.o

clean:
	rm -r build/*