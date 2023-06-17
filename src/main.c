#include <stdio.h>
#include <string.h>
#include <vips/vips.h>

#include "cmdline.h"

int load_frame_img(const char *rarity, int trained, VipsImage **out);
int load_attribute_img(const char *attribute, VipsImage **out);

int main(int argc, char **argv)
{
    struct gengetopt_args_info arguments;

    if (cmdline_parser(argc, argv, &arguments) != 0) {
        exit(1);
    }

    if (VIPS_INIT(argv[0]))
    {
        vips_error_exit("Unable to start vips.");
    }

    VipsImage *img_in, *img_out, *thumb_img, *frame_img, *attr_img;

    if (vips_thumbnail(arguments.input_arg, &img_in, 140, "size", VIPS_SIZE_DOWN, NULL) ||
        vips_embed(img_in, &thumb_img, 8, 8, 156, 156, NULL))
    {
        vips_error_exit("Unable to load input image.");
    }

    g_object_unref(img_in);

    if (load_frame_img(arguments.rarity_arg, arguments.trained_flag, &frame_img))
    {
        vips_error_exit("Unable to load frame image.");
    }

    if (load_attribute_img(arguments.attribute_arg, &attr_img))
    {
        vips_error_exit("Unable to load attribute image.");
    }

    VipsImage *images[] = { thumb_img, frame_img, attr_img };
    int blend_modes[] = { VIPS_BLEND_MODE_OVER, VIPS_BLEND_MODE_OVER, VIPS_BLEND_MODE_OVER };
    if (vips_composite(images, &img_out, 3, blend_modes, NULL))
    {
        vips_error_exit("Unable to compose images.");
    }
    
    g_object_unref(thumb_img);
    g_object_unref(frame_img);
    g_object_unref(attr_img);

    if (vips_pngsave(img_out, arguments.output_arg, "compression", arguments.compression_arg, NULL))
    {
        vips_error_exit("Unable to save output image.");
    }

    g_object_unref(img_out);

    vips_shutdown();

    return 0;
}

int load_frame_img(const char *rarity, int trained, VipsImage **out)
{
    char *start, *end;

    if (strcmp(rarity, "1") == 0)
    {
        extern char _binary_src_images_frame_1_png_start, _binary_src_images_frame_1_png_end;
        start = &_binary_src_images_frame_1_png_start;
        end = &_binary_src_images_frame_1_png_end;
    }
    else if (strcmp(rarity, "2") == 0)
    {
        extern char _binary_src_images_frame_2_png_start, _binary_src_images_frame_2_png_end;
        start = &_binary_src_images_frame_2_png_start;
        end = &_binary_src_images_frame_2_png_end;
    }
    else if (strcmp(rarity, "3") == 0)
    {
        if (trained == 0)
        {
            extern char _binary_src_images_frame_3_png_start, _binary_src_images_frame_3_png_end;
            start = &_binary_src_images_frame_3_png_start;
            end = &_binary_src_images_frame_3_png_end;
        }
        else
        {
            extern char _binary_src_images_frame_3_trained_png_start, _binary_src_images_frame_3_trained_png_end;
            start = &_binary_src_images_frame_3_trained_png_start;
            end = &_binary_src_images_frame_3_trained_png_end;
        }
    }
    else if (strcmp(rarity, "4") == 0)
    {
        if (trained == 0)
        {
            extern char _binary_src_images_frame_4_png_start, _binary_src_images_frame_4_png_end;
            start = &_binary_src_images_frame_4_png_start;
            end = &_binary_src_images_frame_4_png_end;
        }
        else
        {
            extern char _binary_src_images_frame_4_trained_png_start, _binary_src_images_frame_4_trained_png_end;
            start = &_binary_src_images_frame_4_trained_png_start;
            end = &_binary_src_images_frame_4_trained_png_end;
        }
    }
    else if (strcasecmp(rarity, "birthday") == 0)
    {
        extern char _binary_src_images_frame_birthday_png_start, _binary_src_images_frame_birthday_png_end;
        start = &_binary_src_images_frame_birthday_png_start;
        end = &_binary_src_images_frame_birthday_png_end;
    }
    else
    {
        return -1;
    }

    size_t len = (size_t)(end - start);

    return vips_pngload_buffer(start, len, out, NULL);
}

int load_attribute_img(const char *attribute, VipsImage **out)
{
    char *start, *end;

    if (strcasecmp(attribute, "cool") == 0)
    {
        extern char _binary_src_images_attr_cool_png_start, _binary_src_images_attr_cool_png_end;
        start = &_binary_src_images_attr_cool_png_start;
        end = &_binary_src_images_attr_cool_png_end;
    }
    else if (strcasecmp(attribute, "cute") == 0)
    {
        extern char _binary_src_images_attr_cute_png_start, _binary_src_images_attr_cute_png_end;
        start = &_binary_src_images_attr_cute_png_start;
        end = &_binary_src_images_attr_cute_png_end;
    }
    else if (strcasecmp(attribute, "happy") == 0)
    {
        extern char _binary_src_images_attr_happy_png_start, _binary_src_images_attr_happy_png_end;
        start = &_binary_src_images_attr_happy_png_start;
        end = &_binary_src_images_attr_happy_png_end;
    }
    else if (strcasecmp(attribute, "mysterious") == 0)
    {
        extern char _binary_src_images_attr_mysterious_png_start, _binary_src_images_attr_mysterious_png_end;
        start = &_binary_src_images_attr_mysterious_png_start;
        end = &_binary_src_images_attr_mysterious_png_end;
    }
    else if (strcasecmp(attribute, "pure") == 0)
    {
        extern char _binary_src_images_attr_pure_png_start, _binary_src_images_attr_pure_png_end;
        start = &_binary_src_images_attr_pure_png_start;
        end = &_binary_src_images_attr_pure_png_end;
    }
    else
    {
        return -1;
    }

    size_t len = (size_t)(end - start);

    return vips_pngload_buffer(start, len, out, NULL);
}