#include <argp.h>
#include <stdio.h>
#include <string.h>
#include <vips/vips.h>

const char *argp_program_version = "version 1.0.0";

static char args_doc[] = "";
static char doc[] = "Generates a 156x156 thumbnail with rarity frame and attribute icon";

static struct argp_option options[] = {
  { "input",       'i', "IN_FILE",   0, "File path of thumbnail image from assets" },
  { "output",      'o', "OUT_FILE",  0, "File path of output image, must have png extension (default: \"export.png\")" },
  { "rarity",      'r', "RARITY",    0, "Sets rarity frame of thumbnail, can be \"1\", \"2\", \"3\", \"4\", or \"birthday\"" },
  { "attribute",   'a', "ATTRIBUTE", 0, "Sets attribute icon of thumbnail, can be \"cool\", \"cute\", \"happy\", \"mysterious\", or \"pure\"" },
  { "trained",     't', 0,           0, "Sets untrained or trained variant of frame for rarities \"3\" and \"4\" (default: false)" },
  { "compression", 'c', "LEVEL",     0, "Sets compression level of resulting image, range from 0-9, where higher means more compressed (default: 6)" },
  { 0 }
};

struct arguments
{
    char *in;
    char *out;
    char *rarity;
    char *attribute;
    int trained;
    int compression;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state);

static struct argp argp = { options, parse_opt, args_doc, doc };

int load_frame_img(const char *rarity, int trained, VipsImage **out);
int load_attribute_img(const char *attribute, VipsImage **out);

int main(int argc, char **argv)
{
    struct arguments arguments;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    if (VIPS_INIT(argv[0]))
    {
        vips_error_exit("Unable to start vips.");
    }

    VipsImage *img_in, *img_out, *thumb_img, *frame_img, *attr_img;

    if (vips_thumbnail(arguments.in, &img_in, 140, "size", VIPS_SIZE_DOWN, NULL) ||
        vips_embed(img_in, &thumb_img, 8, 8, 156, 156, NULL))
    {
        vips_error_exit("Unable to load input image.");
    }

    g_object_unref(img_in);

    if (load_frame_img(arguments.rarity, arguments.trained, &frame_img))
    {
        vips_error_exit("Unable to load frame image.");
    }

    if (load_attribute_img(arguments.attribute, &attr_img))
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

    if (vips_pngsave(img_out, arguments.out, "compression", arguments.compression, NULL))
    {
        vips_error_exit("Unable to save output image.");
    }

    g_object_unref(img_out);

    vips_shutdown();

    return 0;
}

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;

    switch (key)
    {
        case ARGP_KEY_INIT:
            arguments->in = "";
            arguments->out = "export.png";
            arguments->rarity = "";
            arguments->attribute = "";
            arguments->trained = 0;
            arguments->compression = 6;
            break;
        case 'i':
            arguments->in = arg;
            break;
        case 'o':
            arguments->out = arg;
            break;
        case 'r':
            arguments->rarity = arg;
            break;
        case 'a':
            arguments->attribute = arg;
            break;
        case 't':
            arguments->trained = 1;
            break; 
        case 'c':
            arguments->compression = atoi(arg);
            break;
        case ARGP_KEY_END:
            if (strcmp(arguments->in, "") == 0)
            {
                fprintf(stderr, "Input file must be specified\n");
                argp_usage(state);
            }

            if (strcmp(arguments->out, "") == 0)
            {
                fprintf(stderr, "Output file must be specified\n");
                argp_usage(state);
            }

            char *rarity = arguments->rarity;
            if (
                strcmp(rarity, "1") != 0 &&
                strcmp(rarity, "2") != 0 &&
                strcmp(rarity, "3") != 0 &&
                strcmp(rarity, "4") != 0 &&
                strcasecmp(rarity, "birthday") != 0)
            {
                fprintf(stderr, "Invalid rarity\n");
                argp_usage(state);
            }

            char *attribute = arguments->attribute;
            if (strcasecmp(attribute, "cool") != 0 && 
                strcasecmp(attribute, "cute") != 0 && 
                strcasecmp(attribute, "happy") != 0 && 
                strcasecmp(attribute, "mysterious") != 0 &&
                strcasecmp(attribute, "pure") != 0)
            {
                fprintf(stderr, "Invalid attribute\n");
                argp_usage(state);
            }

            int compression = arguments->compression;
            if (compression < 0 || compression > 9)
            {
                fprintf(stderr, "Invalid compression level\n");
                argp_usage(state);
            }
            
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }

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