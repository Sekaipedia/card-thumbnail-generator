# Card Thumbnail Generator

The new card thumbnail generator program for Sekaipedia, replacing the old Python script.

## Usage
This program can only be run on x86_64 versions of Linux and Windows and requires the vips program to be installed. For
details on installation, see [libvip's guide](https://www.libvips.org/install.html).

### Arguments
| Argument | Flag | Description |
| -- | -- | -- |
| Input image | `-i`, `--input` | Path of input image |
| Output image | `-o`, `--output` | Path of output image (optional, default: export.png) |
| Rarity | `-r`, `--rarity` | Rarity frame, can be "1", 2", "3", "4", or "birthday" |
| Attribute | `-a`, `--attribute` | Attribute icon, can be "cool", "cute", "happy", "mysterious", or "pure" |
| Trained | `-t`, `--trained` | Trained or untrained variant of the frame for rarity "3" and "4" (optional, default: false) |
| Compression | `-c`, `--compression` | Compression level of output image, range is 0-9 (optional, default: 6) |

## Development
To build a binary for Linux, the development package for vips needs to be installed.

To build a binary for Windows, the folder containing the Windows binaries of vips must be placed within this repository and renamed as "vips-dev".