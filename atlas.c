// ImageMagick_INCLUDE_DIRS C:/z/msys64/mingw64/include/ImageMagick-7
// ImageMagick_LIBRARIES    C:/z/msys64/mingw64/lib/libMagickWand-7.Q16HDRI.dll.a

//#define MAGICKCORE_QUANTUM_DEPTH 32
//#define MAGICKCORE_HDRI_ENABLE 1
#include <MagickWand/MagickWand.h>
#include <stdlib.h>

#include "src/errors.h"
#include "src/img_manip.h"
#include "src/utils.h"
#include "src/dir_files_utils.h"
#include "src/extract_config.h"
#include "src/calculations.h"

#define IMAGES_DIR "./atlas/"
#define CONFIG_FILE "./atlas/config.txt"
#define ATLAS_UV_HELP_FILE "./atlas/uv_help.txt"

int main(int argc, char **argv) {

    if(MAKE_DIR_ERROR == make_dir_if_not_exists(IMAGES_DIR))
        return exit_with_error("Dir %s does not exist.\n", IMAGES_DIR);

    // =========================== >> Write config ========================================

    int config_entries_total = 31;
    const char *lines[] = {
            "inputs_are_non_color 0 // 0 or 1\n\n",
            "cols 2 // it is ignored when using absolute_positioning\n\n",
            "shrink 0.92 // 0.5 - 1.0 // it is ignored when using absolute_positioning\n",
            "## between 0.5 and 1.0, where 1.0 means no downscale (important to not omit the decimal - e.g. not 1 but 1.0)\n\n",
            "fill_in none // it is ignored when using absolute_positioning\n",
            "## none | 0:2048x2048 | 2:2048x2048,4:1024x1024 | e.t.c.\n\n",
            "fill_in_color #000000ff\n",
            "## #ffffffff (white opaque) | #000000FF (black opaque) | #7F7F7FFF (0.5 gray opaque) | #7F7F7F00 (0.5 gray transparent) | e.t.c.\n\n",
            "absolute_positioning none\n",
            "## none || 1.png:512x512(1.0)/0-0,1.tif:512x512(1.0)/256-256 // the images must exist in directory\n",
            "## <img_name>:<width>x<height>(zoom_out)/offsetX-offsetY\n\n",
            "output_format png // png, tif, exr, jpg\n\n",

            "\n\n## ------------------ The followings need not be touched in general ------------------ \n\n\n\n",

            "shrink_filter 22 // 0 - 31 // the filter used for shrinking (zooming out)\n",
            "## LanczosFilter(22)\n\n",
            "downscale_to_min_size 0 // 0 or 1 // is ignored when absolute_positioning is not none\n",
            "## if 1 then all images will be downscaled according with smallest image size using resize_filter\n\n",
            "resize_filter 22 // 0 - 31 // applied when downscale_to_min_size is 1 or for absolute_positioning\n",
            "## LanczosFilter(22)\n\n",

            "force_non_linear_colorspace_on_input_images 1 // 0 or 1\n\n",

            "output_compression_algorithm 20 // 0 no compression algorithm \n",
            "## ZipCompression(20)/LZWCompression(15) (TIFF) || \n",
            "## ZipCompression(20)/ZipSCompression(21)/PizCompression(17 lossy)/Pxr24Compression(18 lossy)/DWAACompression(24 lossy)/DWABCompression(25 lossy) (EXR) \n\n",
            "output_compression_strength 1 // 0 default compression strength \n",
            "## (100 strong compression smaller size, 1 soft compression larger size) \n",
            "## affects png size and tif size with ZipCompression (not with LZWCompression) - does not apply to jpg, exr \n\n",
            "output_jpg_quality 100 // affects jpg size (100 higher Q larger size, 1 lower Q smaller size)\n\n",

            "output_bit_depth 0 // 0, 8, 16, 32 (0 means auto) - auto means the greatest bit depth wil be used \n\n",
            "output_image_type 0 // this should be let as 0 (auto)\n",
            "## GrayscaleType(2), GrayscaleAlphaType(3), TrueColorType(6), TrueColorAlphaType(7), ColorSeparationType(8), ColorSeparationAlphaType(9)\n\n",
            "output_colorspace 0 // this should be let as 0 (auto)\n",
            "## LinearGRAYColorspace(33)(for ao/metallic/roughness/height/displacement), GRAYColorspace(3),\n",
            "## RGBColorspace(21 Linear)(for normals), sRGBColorspace(23 - non-linear)(for diffuse/color), CMYKColorspace (2) \n\n",

            "prevent_gray_channel_optimization 0 // 0 or 1\n\n",

            "resize_all_images_to_width 0 // (integer) // is ignored when absolute_positioning is not none \n",
            "resize_all_images_to_height 0 // (integer) // is ignored when absolute_positioning is not none \n",
            "## both need to be set to be considered \n\n",

            "output_quantum_format floating-point // floating-point(works with 16/32 bits), unsigned, signed\n\n",
            "output_quantum_format_apply 0 // 0, 1\n\n",
            "output_exr_color_type RGBA // RGB, RGBA, YC, YCA, Y(gray), YA(gray/alpha), R, G, B, A(alpha)\n\n",
            "output_exr_color_type_apply 0 // 0, 1\n\n",

            "sample_pixel_x 10\n\n",
            "sample_pixel_y 10\n\n",
            "debug_zooming_out 0 // print image info during zooming_out\n\n",
            "debug_resizing 0 // print image info during resizing\n\n",
            "debug_uv_help 0 // print details during generating uv help\n\n",
            "uv_rect_color #ff000011\n\n",
            "print_uv_help 0 // print uv help\n\n"

    };
    int config_lines_length = sizeof(lines)/sizeof(lines[0]);
    if (WRITE_LINES_ERROR == write_lines(CONFIG_FILE, lines, config_lines_length, true))
        return exit_with_error("Could not write config.\n");

    // =========================== << Write config ========================================

    // =========================== >> Read config ========================================

    int config_entries = 0;
    KeyValue* kv_arr = extract_config(CONFIG_FILE, &config_entries);
    if (kv_arr == NULL)
        return exit_with_error("Could not extract kv_arr config\n");

    printf("\nConfig lines found: %d (from %d)\n", config_entries, config_entries_total);
    if (config_entries != config_entries_total) return exit_with_error("Invalid number of config entries\n");

    for (int i = 0; i < config_entries; i++) {
        printf(" - ");
        print_key_value(kv_arr[i]);
    }

    bool inputs_are_non_color = config_find(kv_arr, config_entries, "inputs_are_non_color")->value.i;

    int cols = config_find(kv_arr, config_entries, "cols")->value.i;
    double shrink = config_find(kv_arr, config_entries, "shrink")->value.d;

    char fill_in[256];
    strcpy(fill_in, config_find(kv_arr, config_entries, "fill_in")->value.s);

    char fill_in_color[32];
    strcpy(fill_in_color, config_find(kv_arr, config_entries, "fill_in_color")->value.s);

    int fill_in_array_length = 0;
    FillInEntry *fill_in_array = extract_fill_in_array(fill_in, &fill_in_array_length);
    qsort(fill_in_array, fill_in_array_length, sizeof(FillInEntry), compareFillInEntry);
    for (int i = 0; i < fill_in_array_length; i++) {
        printf("fill_in_array[%d] = {position: %d, width: %d, height: %d}\n", i, fill_in_array[i].position, fill_in_array[i].width, fill_in_array[i].height);
    }

    char absolute_positioning[1024];
    strcpy(absolute_positioning, config_find(kv_arr, config_entries, "absolute_positioning")->value.s);

    int absolute_positioning_array_length = 0;
    AbsolutePositioningEntry *absolute_positioning_array = extract_absolute_positioning_array(absolute_positioning, &absolute_positioning_array_length);
    int absolute_positioning_canvas_width = get_absolute_position_canvas_width(absolute_positioning_array, absolute_positioning_array_length);
    int absolute_positioning_canvas_height = get_absolute_position_canvas_height(absolute_positioning_array, absolute_positioning_array_length);

    if (absolute_positioning_array_length) {
        printf("\nAbsolute positioning: \n");
        for (int i = 0; i < absolute_positioning_array_length; i++) {
            printf("absolute_positioning_array[%d] = {name: %s, width: %d, height: %d, shrink: %.2f, offset_x: %d, offset_y: %d}\n", i, absolute_positioning_array[i].name, absolute_positioning_array[i].width, absolute_positioning_array[i].height, absolute_positioning_array[i].shrink, absolute_positioning_array[i].offset_x, absolute_positioning_array[i].offset_y);
        }
        printf("absolute_positioning_canvas_width: %d\n", absolute_positioning_canvas_width);
        printf("absolute_positioning_canvas_height: %d\n", absolute_positioning_canvas_height);

        // if we use absolute positioning then we don't need fill_in_array
        free(fill_in_array);
        fill_in_array_length = 0;
    }

    char output_format[16]; // png, jpg
    strcpy(output_format, config_find(kv_arr, config_entries, "output_format")->value.s);


    FilterType shrink_filter = (FilterType)config_find(kv_arr, config_entries, "shrink_filter")->value.i;
    bool downscale_to_min_size = config_find(kv_arr, config_entries, "downscale_to_min_size")->value.i;
    FilterType resize_filter = (FilterType)config_find(kv_arr, config_entries, "resize_filter")->value.i;

    bool force_non_linear_colorspace_on_input_images = (bool)config_find(kv_arr, config_entries, "force_non_linear_colorspace_on_input_images")->value.i;

    CompressionType output_compression_algorithm = (CompressionType)config_find(kv_arr, config_entries, "output_compression_algorithm")->value.i;
    int output_compression_strength = config_find(kv_arr, config_entries, "output_compression_strength")->value.i;
    int output_jpg_quality = config_find(kv_arr, config_entries, "output_jpg_quality")->value.i;

    int output_bit_depth = config_find(kv_arr, config_entries, "output_bit_depth")->value.i;
    ImageType output_image_type = (ImageType)config_find(kv_arr, config_entries, "output_image_type")->value.i;
    ColorspaceType output_colorspace = (ColorspaceType)config_find(kv_arr, config_entries, "output_colorspace")->value.i;

    bool prevent_gray_channel_optimization = config_find(kv_arr, config_entries, "prevent_gray_channel_optimization")->value.i;

    int resize_all_images_to_width = config_find(kv_arr, config_entries, "resize_all_images_to_width")->value.i;
    int resize_all_images_to_height = config_find(kv_arr, config_entries, "resize_all_images_to_height")->value.i;

    char output_quantum_format[32]; // floating-point, signed
    strcpy(output_quantum_format, config_find(kv_arr, config_entries, "output_quantum_format")->value.s);

    bool output_quantum_format_apply = config_find(kv_arr, config_entries, "output_quantum_format_apply")->value.i;

    char output_exr_color_type[32]; // floating-point, signed
    strcpy(output_exr_color_type, config_find(kv_arr, config_entries, "output_exr_color_type")->value.s);

    bool output_exr_color_type_apply = config_find(kv_arr, config_entries, "output_exr_color_type_apply")->value.i;


    int sample_pixel_x = config_find(kv_arr, config_entries, "sample_pixel_x")->value.i;
    int sample_pixel_y = config_find(kv_arr, config_entries, "sample_pixel_y")->value.i;

    bool debug_zooming_out = config_find(kv_arr, config_entries, "debug_zooming_out")->value.i;
    bool debug_resizing = config_find(kv_arr, config_entries, "debug_resizing")->value.i;

    bool debug_uv_help = config_find(kv_arr, config_entries, "debug_uv_help")->value.i;

    char uv_rect_color[16]; // #ff000055
    strcpy(uv_rect_color, config_find(kv_arr, config_entries, "uv_rect_color")->value.s);

    bool print_uv_help = config_find(kv_arr, config_entries, "print_uv_help")->value.i;

    free(kv_arr);

    printf("\nConfig values: \n"
           "\tinputs_are_non_color %d \n"
           "\tcols %d \n"
           "\tshrink %f \n"
           "\tfill_in %s \n"
           "\tfill_in_color %s \n"
           "\tabsolute_positioning %s \n"
           "\toutput_format %s \n"
           "\tshrink_filter %d \n"
           "\tdownscale_to_min_size %d \n"
           "\tresize_filter %d \n"
           "\toutput_compression_algorithm %d \n"
           "\toutput_compression_strength %d \n"
           "\toutput_jpg_quality %d \n"
           "\toutput_bit_depth %d \n"
           "\toutput_image_type %d \n"
           "\toutput_colorspace %d \n"
           "\tprevent_gray_channel_optimization %d \n"
           "\tresize_all_images_to_width %d \n"
           "\tresize_all_images_to_height %d \n"
           "\toutput_quantum_format %s \n"
           "\toutput_quantum_format_apply %d \n"
           "\toutput_exr_color_type %s \n"
           "\toutput_exr_color_type_apply %d \n"
           "\tsample_pixel_x %d \n"
           "\tsample_pixel_y %d \n"
           "\tdebug_zooming_out %d \n"
           "\tdebug_resizing %d \n"
           "\tdebug_uv_help %d \n"
           "\tuv_rect_color %s, \n"
           "\tprint_uv_help %d \n",
           inputs_are_non_color,
           cols,
           shrink,
           fill_in,
           fill_in_color,
           absolute_positioning,
           output_format,
           shrink_filter,
           downscale_to_min_size,
           resize_filter,
           output_compression_algorithm,
           output_compression_strength,
           output_jpg_quality,
           output_bit_depth,
           output_image_type,
           output_colorspace,
           prevent_gray_channel_optimization,
           resize_all_images_to_width,
           resize_all_images_to_height,
           output_quantum_format,
           output_quantum_format_apply,
           output_exr_color_type,
           output_exr_color_type_apply,
           sample_pixel_x,
           sample_pixel_y,
           debug_zooming_out,
           debug_resizing,
           debug_uv_help,
           uv_rect_color,
           print_uv_help
       );

    // =========================== << Read config ========================================


    // =========================== << Validate config =====================================

    if (cols < 1) return exit_with_error("cols must be greater than 0\n");
    if (shrink < 0.5 || shrink > 1) return exit_with_error("shrink must be between 0.5 and 1. Current value is: %f\n", shrink);
    if (shrink_filter < 0 || shrink_filter > 31) return exit_with_error("shrink_filter must be between 0 and 31\n");
    if (resize_filter < 0 || resize_filter > 31) return exit_with_error("resize_filter must be between 0 and 31\n");
    if (sample_pixel_x < 0) return exit_with_error("sample_pixel_x must be greater than 0\n");
    if (sample_pixel_y < 0) return exit_with_error("sample_pixel_y must be greater than 0\n");
    if (output_bit_depth != 0 && output_bit_depth != 8 && output_bit_depth != 16 && output_bit_depth != 32) return exit_with_error("output_bit_depth must be 0, 8, 16 or 32\n");
    if (output_image_type < 0 || output_image_type > 11) return exit_with_error("output_image_type must be between 0 and 11\n");
    if (inputs_are_non_color != 0 && inputs_are_non_color != 1) return exit_with_error("inputs_are_non_color must be 0 or 1\n");
    if (output_colorspace != 0 && output_colorspace != 2 && output_colorspace != 3 && output_colorspace != 21 && output_colorspace != 23 && output_colorspace != 33) return exit_with_error("output_colorspace must be 0, 2, 3, 21, 23 or 33\n");
    if (output_compression_algorithm < 0 || output_compression_algorithm > 27) return exit_with_error("output_compression_algorithm must be between 0 and 27\n");
    if (output_compression_strength < 0 || output_compression_strength > 100) return exit_with_error("output_compression_strength must be between 0 and 100\n");
    if (output_jpg_quality < 0 || output_jpg_quality > 100) return exit_with_error("output_jpg_quality must be between 0 and 100\n");
    if (strcmp(output_format, "png") != 0 && strcmp(output_format, "tif") != 0 && strcmp(output_format, "exr") != 0 && strcmp(output_format, "jpg") != 0 && strcmp(output_format, "tga") != 0) return exit_with_error("output_format must be png, tif, exr, tga or jpg\n");
    if (strcmp(output_quantum_format, "floating-point") != 0 && strcmp(output_quantum_format, "signed") != 0 && strcmp(output_quantum_format, "unsigned") != 0) return exit_with_error("output_quantum_format must be floating-point, signed, unsigned\n");
    if (strcmp(output_exr_color_type, "RGB") != 0 && strcmp(output_exr_color_type, "RGBA") != 0 && strcmp(output_exr_color_type, "YC") != 0 && strcmp(output_exr_color_type, "YCA") != 0 && strcmp(output_exr_color_type, "Y") != 0 && strcmp(output_exr_color_type, "YA") != 0 && strcmp(output_exr_color_type, "R") != 0 && strcmp(output_exr_color_type, "G") != 0 && strcmp(output_exr_color_type, "B") != 0 && strcmp(output_exr_color_type, "A") != 0) return exit_with_error("output_exr_color_type must be RGB, RGBA, YC, YCA, Y, YA, R, G, B or A\n");
    if (resize_all_images_to_width < 0) return exit_with_error("resize_all_images_to_width must be greater than 0\n");
    if (resize_all_images_to_height < 0) return exit_with_error("resize_all_images_to_height must be greater than 0\n");

    // =========================== >> Validate config =====================================



    // =========================== >> List images ========================================

    int num_paths = 0;
    char **image_paths = list_files_with_filter(IMAGES_DIR, &num_paths, true, is_image_file_and_not_atlas);
    char **image_paths_no_dir = list_files_with_filter(IMAGES_DIR, &num_paths, false, is_image_file_and_not_atlas);
    if (absolute_positioning_array_length) {
        // validate that all images in absolute_positioning_array exist in image_paths_no_dir
        for (int i = 0; i < absolute_positioning_array_length; i++) {
            bool exists = false;
            for (int j = 0; j < num_paths; j++) {
                if (strcmp(absolute_positioning_array[i].name, image_paths_no_dir[j]) == 0) {
                    exists = true;
                }
            }
            if (!exists) return exit_with_error("Image %s does has not been listed in directory %s\n", absolute_positioning_array[i].name, IMAGES_DIR);
        }
    }
    if (image_paths == NULL)
        return exit_with_error("Could not list files.\n");
    if (num_paths == 0) {
        return exit_with_error("No image found.\n");
    }

    if (fill_in_array_length) {
        // validating fill_in entry positions to not overflow
        if (fill_in_array[fill_in_array_length - 1].position > num_paths + fill_in_array_length - 1) {
            return exit_with_error("Fill in position %d is greater than number of images + prev fill_in_array entries = %d.\n", fill_in_array[fill_in_array_length - 1].position, num_paths + fill_in_array_length - 1);
        }
    }

    size_t size_t_num_paths = (size_t)num_paths;
    size_t size_t_num_paths_no_dir = (size_t)num_paths;
    for (int i = 0; i < fill_in_array_length; i++) {
        char *memory_filename = (char *)malloc(32 * sizeof(char));
        sprintf(memory_filename, "%d:%dx%d", fill_in_array[i].position, fill_in_array[i].width, fill_in_array[i].height); // needed when freeing later to not free same memory twice

        char *memory_filename2 = (char *)malloc(32 * sizeof(char));
        sprintf(memory_filename2, "%d:%dx%d", fill_in_array[i].position, fill_in_array[i].width, fill_in_array[i].height);

        if(!array_insert((void**)&image_paths, &memory_filename, fill_in_array[i].position, &size_t_num_paths, sizeof(char *))) {
            return exit_with_error("Could not insert memory_filename at position %d\n", fill_in_array[i].position);
        }
        if(!array_insert((void**)&image_paths_no_dir, &memory_filename2, fill_in_array[i].position, &size_t_num_paths_no_dir, sizeof(char *))) {
            return exit_with_error("Could not insert memory_filename2 at position %d\n", fill_in_array[i].position);
        }
    }

    printf("\nFound %d files \n", num_paths);
    num_paths = (int)size_t_num_paths;
    printf("Using %d entries \n", num_paths);

    // print info about image_paths
    char image_paths_info[1000] = "";
    int dimensions[] = {num_paths};
    arrayToString(image_paths_info, (void *)image_paths, dimensions, 1, 0, "string", sizeof(char *), "%s", 128);
    printf("\nimage_paths: %s\n\n", image_paths_info);

    int rows = num_paths / cols + (num_paths % cols == 0 ? 0 : 1);

    // =========================== << List images ========================================

    MagickWandGenesis();

    MagickWand * wand = NewMagickWand();
    if(wand == NULL) return exit_with_error("Unable to create wand\n");

    // MagickWand **input_wands = (MagickWand**)malloc(num_paths * sizeof(MagickWand*));
    MagickWand* input_wands[num_paths];
    size_t min_width = SIZE_MAX;
    size_t min_height = SIZE_MAX;
    bool any_floating_point_tiff = false;
    bool any_unsigned_tiff = false;
    bool any_signed_tiff = false;
    bool any_true_color_alpha_type = false;
    bool any_true_color_type = false;
    bool any_gray_alpha_type = false;
    bool any_gray_type = false;
    bool any_exr_file = false;
    for(int i = 0; i < num_paths; i++) {
        // =========================== >> Read or fill_in each image ========================================
        FillInEntry *fill_in_entry = find_fill_in_entry(fill_in_array, fill_in_array_length, i);

        input_wands[i] = NewMagickWand();

        if (input_wands[i] == NULL) return exit_with_error("Unable to create input_wands[%d]\n", i);
        if (fill_in_entry) {
            printf("Creating fill_in_entry[%d] = {position: %d, width: %d, height: %d}\n", i, fill_in_entry->position,
                   fill_in_entry->width, fill_in_entry->height);
            PixelWand *pixel_wand = NewPixelWand();
            PixelSetColor(pixel_wand, fill_in_color);
            MagickNewImage(input_wands[i], fill_in_entry->width, fill_in_entry->height, pixel_wand);
            MagickSetImageFilename(input_wands[i], image_paths[i]);
            DestroyPixelWand(pixel_wand);
        } else {
            printf("Reading image path[%d] = %s\n", i, image_paths[i]);
            if (MagickReadImage(input_wands[i], image_paths[i]) == MagickFalse) {
                char *description;
                ExceptionType severity;
                description = MagickGetException(input_wands[i], &severity);
                fprintf(stderr, "Error: %s\n", description);
                description = (char *)MagickRelinquishMemory(description);
                return exit_with_error("Unable to read image %s \n", image_paths[i]);
            }
        }

        printf("Original image[%d] %s\n", i, image_paths[i]);
        print_info(input_wands[i], sample_pixel_x, sample_pixel_y);

        // ================================= >> Enforce colorspace and image_type optimisations =================================

        // force non-linear colorspace especially for exr to tif conversion (exr seems to be assumed as linear by imagemagick)
        if (force_non_linear_colorspace_on_input_images) {
            ColorspaceType colorspace = sRGBColorspace;
            // it seems to be safe for grayscale/grayscaleAlpha images too
            // but just in case if the image is grayscale or grayscaleAlpha we use GRAYColorspace
            if (MagickGetImageColorspace(input_wands[i]) == GRAYColorspace || MagickGetImageColorspace(input_wands[i]) == LinearGRAYColorspace) {
                colorspace = GRAYColorspace;
            } else if (MagickGetImageColorspace(input_wands[i]) == CMYKColorspace) {
                colorspace = CMYKColorspace; // should not be encountered
            }

            printf("\nApplying forced non_linear_colorspace %s to image[%d] %s\n", colorSpaceAsString(colorspace), i, image_paths[i]);
            MagickSetImageColorspace(input_wands[i], colorspace);
            MagickSetColorspace(input_wands[i], colorspace);
            printf("New colorspace is %s\n", colorSpaceAsString(MagickGetImageColorspace(input_wands[i])));
        }

        bool _has_alpha_channel_in_theory = (bool)MagickGetImageAlphaChannel(input_wands[i]);
        bool _has_alpha_channel_in_practice = (bool)has_alpha_channel_in_practice(input_wands[i]);
        bool _are_rgb_channels_equal_in_theory =
                MagickGetImageType(input_wands[i]) == GrayscaleType || MagickGetImageType(input_wands[i]) == GrayscaleAlphaType;
        bool _are_rgb_channels_equal_in_practice = (bool)are_rgb_channels_equal(input_wands[i]);

        if (_has_alpha_channel_in_theory) {
            printf("Image[%d] %s has alpha channel in theory\n", i, image_paths[i]);
        } else {
            printf("Image[%d] %s has no alpha channel in theory\n", i, image_paths[i]);
        }

        if (_has_alpha_channel_in_practice) {
            printf("Image[%d] %s has alpha channel in practice\n", i, image_paths[i]);
        } else {
            printf("Image[%d] %s has no alpha channel in practice\n", i, image_paths[i]);
            if (_has_alpha_channel_in_theory) {
                printf("Deactivating alpha channel for image[%d] %s\n", i, image_paths[i]);
                MagickSetImageAlphaChannel(input_wands[i], OffAlphaChannel);
            }
        }

        if (_are_rgb_channels_equal_in_theory) {
            printf("Image[%d] %s has equal RGB channels in theory\n", i, image_paths[i]);
        } else {
            printf("Image[%d] %s has different RGB channels in theory\n", i, image_paths[i]);
        }

        if (_are_rgb_channels_equal_in_practice) {
            bool _has_alpha_channel = (bool)has_alpha_channel_in_practice(input_wands[i]);
            printf("Image[%d] %s has equal RGB channels in practice\n", i, image_paths[i]);
            if (_has_alpha_channel_in_practice) {
                printf("Setting image type to %s\n", imageTypeAsString(GrayscaleAlphaType));
                MagickSetImageType(input_wands[i], GrayscaleAlphaType);
                MagickSetType(input_wands[i], GrayscaleAlphaType);
            } else {
                printf("Setting image type to %s\n", imageTypeAsString(GrayscaleType));
                MagickSetImageType(input_wands[i], GrayscaleType);
                MagickSetType(input_wands[i], GrayscaleType);
            }
            printf("New image type is %s\n", imageTypeAsString(MagickGetImageType(input_wands[i])));
        }

        // ================================= << Enforce colorspace and image_type optimisations =================================

        // ============================== >> Collect statistics ===============================
        if (strcmp(MagickGetImageFormat(input_wands[i]), "TIFF") == 0 || strcmp(MagickGetImageFormat(input_wands[i]), "TIF") == 0) {
            uint16_t sampleFormat = get_tiff_sample_format(image_paths[i]);
            if (sampleFormat == SAMPLEFORMAT_IEEEFP) {
                any_floating_point_tiff = true;
            } else if (sampleFormat == SAMPLEFORMAT_UINT) {
                any_unsigned_tiff = true;
            } else if (sampleFormat == SAMPLEFORMAT_INT) {
                any_signed_tiff = true;
            }
        }
        if (strcmp(MagickGetImageFormat(input_wands[i]), "EXR") == 0) {
            any_exr_file = true;
        }
        if (MagickGetImageType(input_wands[i]) == TrueColorAlphaType) {
            any_true_color_alpha_type = true;
        } else if (MagickGetImageType(input_wands[i]) == TrueColorType) {
            any_true_color_type = true;
        } else if (MagickGetImageType(input_wands[i]) == GrayscaleAlphaType) {
            any_gray_alpha_type = true;
        } else if (MagickGetImageType(input_wands[i]) == GrayscaleType) {
            any_gray_type = true;
        }

        size_t width = MagickGetImageWidth(input_wands[i]);
        size_t height = MagickGetImageHeight(input_wands[i]);
        if (width < min_width) min_width = width;
        if (height < min_height) min_height = height;

        // ============================== << Collect statistics ===============================
        // =========================== << Read or fill_in each image ========================================

        printf("\n--------------------------------\n\n");
    }

    printf("\nFound => Min width: %zu, Min height: %zu\n\n", min_width, min_height);

    if(downscale_to_min_size && !absolute_positioning_array_length) {
        printf("======================== Downscaling all images to min size %zux%zu using filter %d ==================\n\n",  min_width, min_height, resize_filter);
        for(int i = 0; i < num_paths; i++) {
            resize(input_wands[i], min_width, min_height, resize_filter, !inputs_are_non_color, debug_resizing);
            printf("\n-------------------------------\n\n");
        }
    }

    if(resize_all_images_to_width && resize_all_images_to_height && !absolute_positioning_array_length) {
        printf("======================== Resizing all images to min size %zux%zu using filter %d ==================\n\n", resize_all_images_to_width, resize_all_images_to_height, resize_filter);
        for(int i = 0; i < num_paths; i++) {
            resize(input_wands[i], resize_all_images_to_width, resize_all_images_to_height, resize_filter, !inputs_are_non_color, debug_resizing);
            printf("\n-------------------------------\n\n");
        }
    }

    if (absolute_positioning_array_length) {
        printf("\n================================ Absolute resizing ======================================\n\n");
        for(int i = 0; i < num_paths; i++) {
            AbsolutePositioningEntry *absolute_positioning_entry = find_absolute_positioning_entry_by_name(
                    absolute_positioning_array, absolute_positioning_array_length, image_paths_no_dir[i]);
            if (absolute_positioning_entry) {
                // printf("Absolute resizing image[%d] %s to %dx%d\n", i, image_paths_no_dir[i], absolute_positioning_entry->width, absolute_positioning_entry->height);
                resize(input_wands[i], absolute_positioning_entry->width, absolute_positioning_entry->height, resize_filter, !inputs_are_non_color, debug_resizing);
            }
            printf("\n-------------------------------\n\n");
        }
    }

    size_t** input_sizes = (size_t**)malloc(num_paths * sizeof(size_t*));


    MagickWand * output_wand = NULL;

    if (absolute_positioning_array_length) {
        printf("\nZoom out and compositing absolute positioning ...\n\n");
        // =========================== >> Zoom out and compositing absolute positioning ========================================
        size_t canvasWidth = absolute_positioning_canvas_width;
        size_t canvasHeight = absolute_positioning_canvas_height;
        output_wand = NewMagickWand();
        PixelWand *background = NewPixelWand();
        PixelSetColor(background, fill_in_color);
        MagickNewImage(output_wand, canvasWidth, canvasHeight, background);


        for(int i = 0; i < num_paths; i++) {
            char *image_name = image_paths_no_dir[i];

            AbsolutePositioningEntry *absolute_positioning_entry = find_absolute_positioning_entry_by_name(absolute_positioning_array, absolute_positioning_array_length, image_name);
            if (!absolute_positioning_entry) continue; // for the case where absolute positioning images are less than loaded images

            MagickWand * zoomed_out_wand = zoom_out(input_wands[i], absolute_positioning_entry->shrink, shrink_filter, !inputs_are_non_color, debug_zooming_out, debug_resizing); // LanczosFilter
            DestroyMagickWand(input_wands[i]);
            input_wands[i] = zoomed_out_wand;


            printf("Compositing image[%d] %s %dx%d at %d,%d\n", i, image_name, absolute_positioning_entry->width, absolute_positioning_entry->height, absolute_positioning_entry->offset_x, absolute_positioning_entry->offset_y);
            if(MagickCompositeImage(output_wand, input_wands[i], CopyCompositeOp, MagickTrue , absolute_positioning_entry->offset_x, absolute_positioning_entry->offset_y) == MagickFalse) {
                return exit_with_error("Unable to composite image %s\n", absolute_positioning_entry->name);
            }
            printf("\n-------------------------------\n\n");
        }
        DestroyPixelWand(background);
        // =========================== << Zoom out and compositing absolute positioning ========================================
    } else {
        // =========================== >> Zoom out and adding images to rows and rows to canvas  ========================================
        printf("Creating %d output rows...\n\n", rows);
        MagickWand * output_rows[rows];
        for(int i = 0; i < rows; i++) {
            output_rows[i] = NewMagickWand();
            if(output_rows[i] == NULL) return exit_with_error("Unable to create output_rows[%d]\n", i);
        }
        printf("Zoom out and adding images to output rows ...\n\n");
        for(int i = 0; i < num_paths; i++) {
            input_sizes[i] = (size_t *) malloc(2 * sizeof(size_t));
            input_sizes[i][0] = MagickGetImageWidth(input_wands[i]);
            input_sizes[i][1] = MagickGetImageHeight(input_wands[i]);

            MagickWand * zoomed_out_wand = zoom_out(input_wands[i], shrink, shrink_filter, !inputs_are_non_color, debug_zooming_out, debug_resizing); // LanczosFilter
            DestroyMagickWand(input_wands[i]);
            input_wands[i] = zoomed_out_wand;

            int curr_row = i / cols;
            printf("Adding image[%d] %s to output_rows[%d]\n", i, image_paths[i], curr_row);
            if(MagickAddImage(output_rows[curr_row], input_wands[i]) == MagickFalse) {
                return exit_with_error("Unable to add image %s to output_rows[%d]\n", image_paths[i], i/rows);
            }
            printf("\n-------------------------------\n\n");
        }

        printf("Creating rows by appending output_rows[i] and replacing output_rows[i] with the result\n");
        for(int i = 0; i < rows; i++) {
            MagickResetIterator(output_rows[i]);
            // creating rows
            MagickWand * output_rows_i_collage = MagickAppendImages(output_rows[i], MagickFalse); // false => horizontally, true => vertically
            if (output_rows_i_collage == NULL) return exit_with_error("Unable to append images to output_rows[%d]\n", i);
            DestroyMagickWand(output_rows[i]);
            output_rows[i] = output_rows_i_collage;
        }

        MagickWand * atlas = NewMagickWand();
        if(atlas == NULL) return exit_with_error("Unable to create atlas\n");
        printf("Adding rows to atlas\n");

        for(int i = 0; i < rows; i++) {
            if (MagickAddImage(atlas, output_rows[i]) == MagickFalse) {
                return exit_with_error("Unable to add image to atlas\n");
            }
        }

        printf("Creating output_wand by appending atlas rows\n");
        MagickResetIterator(atlas);
        // filling canvas with rows
        output_wand = MagickAppendImages(atlas, MagickTrue); // false => horizontally, true => vertically
        if (output_wand == NULL) return exit_with_error("Unable to append images to output_wand\n");

        DestroyMagickWand(atlas);
        for(int i = 0; i < rows; i++) {
            DestroyMagickWand(output_rows[i]);
        }
        // =========================== << Zoom out and adding images to rows and rows to canvas  ========================================
    }

    // =========================== >> Config Output Wand ==================
    printf("Configuring output wand\n");
    MagickSetImageFilename(output_wand, "output_wand");
    MagickSetImageFormat(output_wand, output_format);
    // "C:\z\msys64\mingw64\bin\convert.exe" output.tiff -define png:bit-depth=16 output.png
    if (output_bit_depth) {
        // 16 bit exr to 16 bit tif does not preserve the values unless we set quantum:format = floating-point (see later)
        MagickSetImageDepth(output_wand, output_bit_depth);
        MagickSetDepth(output_wand, output_bit_depth);
        // if !output_bit_depth then IM uses the greatest one from input_wands
        // Note: Tiff alpha channel looks to be changed when saving from 32 bit depth (floating-point) to 16 bit depth (floating-point)
        // but only in Windows Image Viewer
        // In Gimp/Blender they look the same.
        // Also NOTE: IM cannot save exr files with 32 bit depth or 8 bit depth. It saves them as 16 bit depth.
        // However, tiff files can be saved with 32 bit depth or 8 bit depth.
    }

    if (output_colorspace) {
        MagickSetColorspace(output_wand, output_colorspace); // LinearGRAYColorspace  GRAYColorspace RGBColorspace sRGBColorspace
        MagickSetImageColorspace(output_wand, output_colorspace);
    }

    if (output_image_type) {
        // GrayscaleType GrayscaleAlphaType TrueColorType TrueColorAlphaType ColorSeparationType
        // This does NOT work for saving exr as one grayscale image.
        // for that we have: exr:color-type=Y
        MagickSetType(output_wand, output_image_type);
        MagickSetImageType(output_wand, output_image_type);
    } else {
        if (any_true_color_alpha_type) {
            MagickSetType(output_wand, TrueColorAlphaType);
            MagickSetImageType(output_wand, TrueColorAlphaType);
        } else if (any_true_color_type) {
            if (any_gray_alpha_type) {
                MagickSetType(output_wand, TrueColorAlphaType);
                MagickSetImageType(output_wand, TrueColorAlphaType);
            } else {
                MagickSetType(output_wand, TrueColorType);
                MagickSetImageType(output_wand, TrueColorType);
            }
        } else if (any_gray_alpha_type) {
            MagickSetType(output_wand, GrayscaleAlphaType);
            MagickSetImageType(output_wand, GrayscaleAlphaType);
        } else if (any_gray_type) {
            MagickSetType(output_wand, GrayscaleType);
            MagickSetImageType(output_wand, GrayscaleType);
        }
    }

    // Not sure when this is actually needed
    if (prevent_gray_channel_optimization && (MagickGetImageType(output_wand) == TrueColorAlphaType || MagickGetImageType(output_wand) == TrueColorType)) {
        PixelIterator *iterator = NewPixelIterator(output_wand);
        // Get the first row of pixels
        size_t _width = 1;
        PixelWand **pixels = PixelGetNextIteratorRow(iterator, &_width);
        // Slightly adjust the red value of the first pixel to prevent optimising into one gray channel
        double red = PixelGetRed(pixels[0]);
        if (red < 0.5) {
            PixelSetRed(pixels[0], red + (1.0/255.0));
        } else {
            PixelSetRed(pixels[0], red - (1.0/255.0));
        }
        PixelSyncIterator(iterator);
        DestroyPixelIterator(iterator);
    }

    if (strcmp(output_format, "png") == 0) {
        if (MagickGetImageDepth(output_wand) == 16 || MagickGetImageDepth(output_wand) == 32) {
            // https://imagemagick.org/script/defines.php
            MagickSetOption(output_wand, "png:bit-depth", "16");
        }
        // Math.floor(1/100 * output_compression_strength * 9)
        int png_compression = (int)floor(1.0 / 100.0 * output_compression_strength * 9); // LERP calculation
        char png_compression_str[2];
        sprintf(png_compression_str, "%d", png_compression);
        MagickSetOption(output_wand, "png:compression-level", png_compression_str); // 0 - 100 => 0 - 9
        // MagickSetOption(output_wand, "png:color-type", "TrueColor"); // this does not work // the intent is to prevent gray channel optimization, but we're doing this later as a hack
    }

    if (strcmp(output_format, "tif") == 0) {
        // for tif to store values as integers or as floating-point / do not apply this on non tif output, it will mess min/max
        if (output_quantum_format_apply) {
            // https://imagemagick.org/script/defines.php
            MagickSetOption(output_wand, "quantum:format", output_quantum_format); // floating-point | signed
            /*
             Docs (not quite correct):
             Set the type to floating-point to specify a floating-point format for raw files (e.g. GRAY)
             or for MIFF and TIFF images in HDRI mode to preserve negative values.
             If -depth 16 is included, the result is a single precision floating point format.
             If -depth 32 is included, the result is double precision floating point format.
             For signed pixel data, use -define quantum:format=signed
             Note: signed pixel data is less likely to be encountered.
             There is also the "unsigned" option which should be used for 8 bit depth.
             */
        }
        else {
            if ((any_floating_point_tiff /*|| any_exr_file*/) && (MagickGetImageDepth(output_wand) == 32 || MagickGetImageDepth(output_wand) == 16)) {
                MagickSetOption(output_wand, "quantum:format", "floating-point");
            } else if (any_signed_tiff && (MagickGetImageDepth(output_wand) == 32 || MagickGetImageDepth(output_wand) == 16)) {
                MagickSetOption(output_wand, "quantum:format", "signed"); // less likely to encounter this
            } else if (MagickGetImageDepth(output_wand) == 8) {
                MagickSetOption(output_wand, "quantum:format", "unsigned");
            }
        }
    }

    if (strcmp(output_format, "exr") == 0 && output_exr_color_type_apply) {
        MagickSetOption(output_wand, "exr:color-type", output_exr_color_type); // RGB, RGBA, YC, YCA, Y, YA, R, G, B, A
    } else {
        if (any_true_color_alpha_type) {
            MagickSetOption(output_wand, "exr:color-type", "RGBA");
        } else if (any_true_color_type) {
            if (any_gray_alpha_type) {
                MagickSetOption(output_wand, "exr:color-type", "RGBA");
            } else {
                MagickSetOption(output_wand, "exr:color-type", "RGB");
            }
        } else if (any_gray_alpha_type) {
            MagickSetOption(output_wand, "exr:color-type", "YA");
        } else if (any_gray_type) {
            MagickSetOption(output_wand, "exr:color-type", "Y");
        }
    }

    if (strcmp(output_format, "jpg") == 0) {
        // affects jpg size and pixel values
        // (1 lowQ/lower size, 100 highQ/higher size)
        MagickSetCompressionQuality(output_wand, output_jpg_quality);
        MagickSetImageCompressionQuality(output_wand, output_jpg_quality);
    }

    bool is_tiff_or_exr = strcmp(output_format, "tif") == 0 || strcmp(output_format, "exr") == 0;
    if (output_compression_algorithm && is_tiff_or_exr) {
        // ZipCompression(20)/LZWCompression(15) (TIFF) ZipCompression(20)/ZipSCompression(21)/PizCompression(17 lossy)/Pxr24Compression(18 lossy)/DWAACompression(24 lossy)/DWABCompression(25 lossy) (EXR)
        MagickSetCompression(output_wand, output_compression_algorithm);
        MagickSetImageCompression(output_wand, output_compression_algorithm);
    }

    if (output_compression_strength && is_tiff_or_exr) {
        // (100 smaller, 1 bigger)
        // affects png size  / tif size with ZipCompression (not with LZWCompression)
        // it seems it doesn't apply to exr
        MagickSetCompressionQuality(output_wand, output_compression_strength);
        MagickSetImageCompressionQuality(output_wand, output_compression_strength);
    }

    // =========================== << Config  Output Wand ==================

    printf("\nOutput_wand configured\n");
    print_info(output_wand, sample_pixel_x, sample_pixel_y);

    // =========================== >> Saving Output Wand ==================
    char atlas_file_name[512];
    str_concat(atlas_file_name, sizeof(atlas_file_name),
               IMAGES_DIR,
               "atlas",
               ".", output_format,
               NULL);
    printf("Saving output wand %s\n", atlas_file_name);
    MagickWriteImage(output_wand, atlas_file_name);
    DestroyMagickWand(output_wand);
    output_wand = NewMagickWand();

    printf("Reloading output wand %s\n", atlas_file_name);
    if (MagickReadImage(output_wand, atlas_file_name) == MagickFalse) return exit_with_error("Unable to reload image %s \n", atlas_file_name);
    print_info(output_wand, sample_pixel_x, sample_pixel_y);

    // =========================== << Saving Output Wand ==================



    // =========================== >> Generating UV corners ==================

    int uv_help_lines_num = 0;
    UVCorners* uvCornersArr = absolute_positioning_array_length ?
            get_abs_uv_corners_arr(absolute_positioning_array, absolute_positioning_array_length, absolute_positioning_canvas_width, absolute_positioning_canvas_height, shrink, debug_uv_help)
            : get_uv_corners_arr(input_sizes, num_paths, cols, shrink, debug_uv_help);
    char **uv_help_lines = print_UV_help(uvCornersArr, absolute_positioning_array_length ? absolute_positioning_array_length : num_paths, absolute_positioning_array_length ? 0 : cols, &uv_help_lines_num);
    if (uv_help_lines == NULL) return exit_with_error("Could not get uv_help_lines.\n");

    if (WRITE_LINES_ERROR == write_lines(ATLAS_UV_HELP_FILE, (const char**)uv_help_lines, uv_help_lines_num, false))
        return exit_with_error("Could not write UV help lines.\n");

    if (print_uv_help) printf("\nUV help:\n");
    for (int i = 0; i < uv_help_lines_num; i++) {
        if (print_uv_help) printf(uv_help_lines[i]);
        free(uv_help_lines[i]);
    }
    free(uv_help_lines);

    printf("Drawing UV corners on atlas\n");
    draw_UVCorners_on_atlas(output_wand, uvCornersArr, num_paths, uv_rect_color);

    char atlas_file_name_uv[512];
    str_concat(atlas_file_name_uv, sizeof(atlas_file_name),
               IMAGES_DIR,
               "uv.atlas",
               ".", output_format,
               NULL);
    printf("Saving output wand (UV) %s\n", atlas_file_name_uv);
    MagickWriteImage(output_wand, atlas_file_name_uv);

    // =========================== << Generating UV corners ==================

    // =========================== >> Remaining cleanup ========================================
    printf("Cleaning up\n");
    DestroyMagickWand(output_wand);
    DestroyMagickWand(wand);

    for(int i = 0; i < num_paths; i++) {
        DestroyMagickWand(input_wands[i]);
        free(image_paths[i]);
        free(image_paths_no_dir[i]);
        free(input_sizes[i]);
    }
    free(image_paths);
    free(image_paths_no_dir);
    free(input_sizes);
    free(fill_in_array);
    for (int i = 0; i < absolute_positioning_array_length; i++) {
        free(absolute_positioning_array[i].name);
    }
    free(absolute_positioning_array);
    free(uvCornersArr);

    MagickWandTerminus();
    // =========================== << Remaining cleanup ========================================

    printf("Type something then press enter to exit ...\n");
    getchar();
    return 0;
}