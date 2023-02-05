BUILD_DIR = build
DIST_DIR = dist
IMAGE_DIR = src/images

.DEFAULT_GOAL = build

build: generate_assets main.o
	gcc $(BUILD_DIR)/*.o `pkg-config vips --cflags --libs` -o $(DIST_DIR)/card_thumbnail_generator

generate_assets: $(notdir $(wildcard $(IMAGE_DIR)/*.png))

main.o:
	gcc -c -Wall src/main.c `pkg-config vips --cflags --libs` -o $(BUILD_DIR)/main.o

%.png: $(IMAGE_DIR)/%.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386:x86-64 $< $(BUILD_DIR)/$(notdir $(basename $<)).o