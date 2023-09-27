// ImageMagick_INCLUDE_DIRS C:/z/msys64/mingw64/include/ImageMagick-7
// ImageMagick_LIBRARIES    C:/z/msys64/mingw64/lib/libMagickWand-7.Q16HDRI.dll.a

//#define MAGICKCORE_QUANTUM_DEPTH 32
//#define MAGICKCORE_HDRI_ENABLE 1
#include <MagickWand/MagickWand.h>

#include "src/errors.h"
#include "src/img_manip.h"
#include "src/utils.h"
#include "src/dir_files_utils.h"
#include "src/extract_config.h"

#define IMAGES_DIR "./merge_channels/"
#define CONFIG_FILE "./merge_channels/config.txt"

int main(int argc, char **argv) {
    if(MAKE_DIR_ERROR == make_dir_if_not_exists(IMAGES_DIR))
        return exit_with_error("Dir %s does not exist.\n", IMAGES_DIR);

    // =========================== >> Write config ========================================
    int config_entries_total = 10;
    const char *lines[] = {
            "output_as_color_image 1 // 0, 1 // only applied for output image when no target image exists. \n\n",
            "output_bit_depth 32 // 8, 16, 32 \n\n",
            "output_format png // png, tiff, exr, jpg\n\n",
            "output_quantum_format floating-point // floating-point, signed\n\n",
            "output_compression_algorithm 20 \n",
            "## ZipCompression(20)/LZWCompression(15) (TIFF) || \n",
            "## ZipCompression(20)/ZipSCompression(21)/PizCompression(17 lossy)/Pxr24Compression(18 lossy)/DWAACompression(24 lossy)/DWABCompression(25 lossy) (EXR) \n\n",
            "output_compression_strength 1 \n",
            "## (100 string compression smaller size, 1 soft compression larger size) \n",
            "## affects png size ; and tiff size with ZipCompression (not with LZWCompression)\n\n",
            "output_jpg_quality 100 // affects jpg size (100 higher Q larger size, 1 lower Q smaller size)\n\n",
            "blank_image_color #000000\n\n",
            "sample_pixel_x 10\n",
            "sample_pixel_y 10\n\n",
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

    bool output_as_color_image = config_find(kv_arr, config_entries, "output_as_color_image")->value.i;

    int output_bit_depth = config_find(kv_arr, config_entries, "output_bit_depth")->value.i;

    char output_format[16]; // png, jpg
    strcpy(output_format, config_find(kv_arr, config_entries, "output_format")->value.s);

    char output_quantum_format[32]; // floating-point, signed
    strcpy(output_quantum_format, config_find(kv_arr, config_entries, "output_quantum_format")->value.s);

    int output_compression_algorithm = config_find(kv_arr, config_entries, "output_compression_algorithm")->value.i;
    int output_compression_strength = config_find(kv_arr, config_entries, "output_compression_strength")->value.i;
    int output_jpg_quality = config_find(kv_arr, config_entries, "output_jpg_quality")->value.i;

    char blank_image_color[16]; // png, jpg
    strcpy(blank_image_color, config_find(kv_arr, config_entries, "blank_image_color")->value.s);

    int sample_pixel_x = config_find(kv_arr, config_entries, "sample_pixel_x")->value.i;
    int sample_pixel_y = config_find(kv_arr, config_entries, "sample_pixel_y")->value.i;

    free(kv_arr);

    printf("\nConfig values: \n"
           "\toutput_as_color_image %d, \n"
           "\toutput_bit_depth %d, \n"
           "\toutput_format %s, \n"
           "\toutput_quantum_format %s, \n"
           "\toutput_compression_algorithm %d, \n"
           "\toutput_compression_strength %d, \n"
           "\toutput_jpg_quality %d \n"
           "\tblank_image_color %s, \n"
           "\tsample_pixel_x %d, sample_pixel_y %d, \n",
           output_as_color_image,
           output_bit_depth,
           output_format, output_quantum_format,
           output_compression_algorithm,
           output_compression_strength,
           output_jpg_quality,
           blank_image_color,
           sample_pixel_x, sample_pixel_y
       );
    // =========================== << Read config ========================================

    // =========================== >> List Sources ========================================
    int num_source_paths = 0;
    char **source_paths = list_files_with_filter(IMAGES_DIR, &num_source_paths, true, is_image_file_and_is_mergeable_source);
    char **source_paths_no_dir = list_files_with_filter(IMAGES_DIR, &num_source_paths, false, is_image_file_and_is_mergeable_source);
    if (source_paths == NULL)
        return exit_with_error("Could not list sources.\n");
    if (num_source_paths == 0) {
        return exit_with_error("No image sources found.\n");
    }
    // =========================== << List Sources ========================================

    // =========================== >> List Target ========================================
    int targets_num = 0;
    char **targets = list_files_with_filter(IMAGES_DIR, &targets_num, true, is_image_file_and_is_mergeable_target);
    if (targets == NULL)
        return exit_with_error("Could not list target.\n");
    bool there_is_a_target = !!targets_num;
    // =========================== << List Target ========================================

    MagickWandGenesis();

    // =========================== >> Read Sources ========================================
    //    MagickWand **source_wands = (MagickWand**)malloc(num_source_paths * sizeof(MagickWand*));
    MagickWand * source_wands[num_source_paths];
    printf("Found %d source files.\n", num_source_paths);

    int last_width = 0, last_height = 0;
    for(int i = 0; i < num_source_paths; i++) {
        printf("Reading source source_paths[%d] = %s\n", i, source_paths[i]);
        source_wands[i] = NewMagickWand();
        if(source_wands[i] == NULL) return exit_with_error("Unable to create source_wands[%d]\n", i);
        if (MagickReadImage(source_wands[i], source_paths[i]) == MagickFalse) return exit_with_error("Unable to read image %s \n", source_paths[i]);
        int width = MagickGetImageWidth(source_wands[i]);
        int height = MagickGetImageHeight(source_wands[i]);
        if (last_width != 0 && last_height != 0 && (last_width != width || last_height != height)) {
            return exit_with_error("Source images have different sizes.\n");
        }
        last_width = width;
        last_height = height;

        print_info(source_wands[i], sample_pixel_x, sample_pixel_y);
        printf("-----------------------------\n");
    }
    // =========================== << Read Sources ========================================

    // =========================== >> Load or Create Output Wand ==================
    ColorspaceType target_color_space = UndefinedColorspace;
    ImageType target_image_type = UndefinedType;
    MagickWand *target_wand = NewMagickWand();
    if(target_wand == NULL) return exit_with_error("Unable to create target_wand\n");
    if (there_is_a_target) {
        printf("Found target = %s\n", targets[0]);
        if (MagickReadImage(target_wand, targets[0]) == MagickFalse) return exit_with_error("Unable to read target %s \n", targets[0]);
        int target_width = MagickGetImageWidth(target_wand);
        int target_height = MagickGetImageHeight(target_wand);
        target_color_space = MagickGetImageColorspace(target_wand);
        target_image_type = MagickGetImageType(target_wand);
        if (last_width != target_width || last_height != target_height) {
            return exit_with_error("Target image has different size than source images.\n");
        }
        printf("Target wand:\n");
        print_info(target_wand, sample_pixel_x, sample_pixel_y);
    } else {
        printf("No targets found. A blank target will be created.\n");
        int width = MagickGetImageWidth(source_wands[0]);
        int height = MagickGetImageHeight(source_wands[0]);
        PixelWand *pixel_wand = NewPixelWand();
        PixelSetColor(pixel_wand, blank_image_color); // if we let it #000000 then it compresses 3 from RGBA channels to 1
        MagickNewImage(target_wand, width, height, pixel_wand);
        // =========================== << Load or Create Output Wand ==================
    }


    // =========================== >> get number of needed channels for output ==================
    bool target_needs_more_channels = false;
    int number_of_channels_needed_for_output = 0;
    int source_channels[num_source_paths];
    for(int i = 0; i < num_source_paths; i++) {
        char * cloned_path = strdup(source_paths_no_dir[i]);
        *(strstr(cloned_path, ".")) = '\0';
        int intValue = get_int(cloned_path).value;
        source_channels[i] = intValue;
    }
    // this is for updating number_of_channels_needed_for_output
    if (there_is_a_target) {
        PixelChannel* channels = get_image_channels(target_wand, &number_of_channels_needed_for_output);
        if (channels == NULL) {
            exit_with_error("Could not get channels (get_image_channels).\n");
        }
    }
    int max_source_channel = source_channels[num_source_paths - 1] + 1;
    if (max_source_channel > number_of_channels_needed_for_output) {
        target_needs_more_channels = true;
        number_of_channels_needed_for_output = max_source_channel;
    }

    int final_source_channels[number_of_channels_needed_for_output];
    MagickWand * final_source_wands[number_of_channels_needed_for_output];
    for (int i = 0; i < number_of_channels_needed_for_output; i++) {
        final_source_channels[i] = -1;
        final_source_wands[i] = NULL;
        // search for the source that has the channel we need
        for (int j = 0; j < num_source_paths; j++) {
            if (source_channels[j] == i) {
                final_source_channels[i] = source_channels[j];
                final_source_wands[i] = source_wands[j];
                break;
            }
        }
    }
    printf("number_of_channels_needed_for_output %d\n", number_of_channels_needed_for_output);
    for (int i = 0; i < number_of_channels_needed_for_output; i++) {
        printf("final_source_channels[%d] = %d\n", i, final_source_channels[i]);
    }
    // =========================== << get number of needed channels for output ==================


    MagickWand *output_wand = CloneMagickWand(target_wand);


    // =========================== >> Set ColorSpace and ImageType for blank Target Wand ==================

    if (!there_is_a_target || target_needs_more_channels) {
        ColorspaceType output_color_space = UndefinedColorspace;
        ImageType output_image_type = UndefinedType;

        if (number_of_channels_needed_for_output == 2) {
            output_color_space = output_as_color_image ? GRAYColorspace : LinearGRAYColorspace;
            output_image_type = GrayscaleAlphaType;
        } else if (number_of_channels_needed_for_output == 3) {
            output_color_space = output_as_color_image ? sRGBColorspace : RGBColorspace;
            output_image_type = TrueColorType;
        } else if (number_of_channels_needed_for_output == 4) {
            output_color_space = output_as_color_image ? sRGBColorspace : RGBColorspace;
            output_image_type = TrueColorAlphaType;
        } else {
            return exit_with_error("Invalid number of sources found: %d.\n", num_source_paths);
        }

        MagickSetColorspace(output_wand, output_color_space);
        MagickSetImageColorspace(output_wand, output_color_space);
        MagickSetType(output_wand, output_image_type);
        MagickSetImageType(output_wand, output_image_type);
    }
    // =========================== << Set ColorSpace and ImageType for blank Target Wand ==================



    // =========================== >> Config Output Wand ==================
    MagickSetImageFormat(output_wand, output_format);
    // 16 bit exr to 16 bit tiff does not preserve the values unless we set quantum:format = floating-point (see later)
    MagickSetImageDepth(output_wand, output_bit_depth);

    if(strcmp(output_format, "tiff") == 0) {
        // for tiff to store values as integers or as floating-point / do not apply this on non tiff output, it will mess min/max
        MagickSetOption(output_wand, "quantum:format", output_quantum_format); // floating-point | signed
        /*
         Set the type to floating-point to specify a floating-point format for raw files (e.g. GRAY)
         or for MIFF and TIFF images in HDRI mode to preserve negative values.
         If -depth 16 is included, the result is a single precision floating point format.
         If -depth 32 is included, the result is double precision floating point format.
         For signed pixel data, use -define quantum:format=signed
         */
    }

    if (strcmp(output_format, "jpg") == 0) {
        // affects jpg size and pixel values
        // (1 lowQ/lower size, 100 highQ/higher size)
        MagickSetImageCompressionQuality(output_wand, output_jpg_quality);
    }

    bool is_tiff_or_exr = strcmp(output_format, "tiff") == 0 || strcmp(output_format, "exr") == 0;
    if (output_compression_algorithm && is_tiff_or_exr) {
        // ZipCompression(20)/LZWCompression(15) (TIFF) ZipCompression(20)/ZipSCompression(21)/PizCompression(17 lossy)/Pxr24Compression(18 lossy)/DWAACompression(24 lossy)/DWABCompression(25 lossy) (EXR)
        MagickSetCompression(output_wand, output_compression_algorithm);
    }

    if (output_compression_strength) {
        // (100 smaller, 1 bigger)
        // affects png size  / tiff size with ZipCompression (not with LZWCompression)
        MagickSetCompressionQuality(output_wand, output_compression_strength);
    }
    // =========================== << Config Output Wand ==================


    // TODO: What if target image is 2 channels and I want to add a 3rd channel from a source image?
    CompositeOperator composite_operators[] = {CopyRedCompositeOp, CopyGreenCompositeOp, CopyBlueCompositeOp, CopyAlphaCompositeOp };
    // =========================== >> Compositing each image ========================================

    ChannelType channels_output_write[] = {RedChannel, GreenChannel, BlueChannel, AlphaChannel};
    ChannelType channels_true_color_alpha_types_target_read[] = {RedChannel, GreenChannel, BlueChannel, AlphaChannel};
    ChannelType channels_true_color_types_target_read[] = {RedChannel, GreenChannel, BlueChannel, UndefinedChannel};
    ChannelType channels_gray_alpha_types_target_read[] = {GrayChannel, AlphaChannel, UndefinedChannel, UndefinedChannel};
    ChannelType channels_gray_types_target_read[] = {GrayChannel, UndefinedChannel, UndefinedChannel, UndefinedChannel};
    ChannelType channels_target_read[4];

    size_t sizeof_channels = sizeof(channels_output_write); // same size all variants

    if (target_image_type == TrueColorAlphaType) {
        // TODO: can we use a pointer instead of using memcpy ?
        memcpy(channels_target_read, channels_true_color_alpha_types_target_read, sizeof_channels);
    } else if (target_image_type == TrueColorType) {
        memcpy(channels_target_read, channels_true_color_types_target_read, sizeof_channels);
    } else if (target_image_type == GrayscaleAlphaType) {
        memcpy(channels_target_read, channels_gray_alpha_types_target_read, sizeof_channels);
    } else if (target_image_type == GrayscaleType) {
        memcpy(channels_target_read, channels_gray_types_target_read, sizeof_channels);
    } else {
        return exit_with_error("Invalid target_image_type %d\n", target_image_type);
    }

    for(int i = 0; i < number_of_channels_needed_for_output; i++) {
        if (number_of_channels_needed_for_output == 2) {
            // TODO: experiment with how channels are copied from source when source has 1,2,3,4 channels
            switch (final_source_channels[i]) {
//                case 0: MagickCompositeImage(output_wand, final_source_wands[i], CopyRedCompositeOp, MagickFalse, 0, 0); break;
//                case 1: MagickCompositeImage(output_wand, final_source_wands[i], CopyAlphaCompositeOp, MagickFalse, 0, 0); break;
                default: break;
            }
        } else {
            // TODO: should we name our source files such that we can extract a specific channel from them into target? like 0_2 ?
            CompositeOperator composite_op = UndefinedCompositeOp;
            switch (final_source_channels[i]) {
                /*
                How compositors work:
                  - if target is gray color type (1 channel) it copies Gray channel into Red or Green or Blue or Alpha as compositor name suggests
                  - if target is gray alpha color type (2 channels) it copies Gray channel into Red or Green or Blue or Alpha channel into Alpha
                  - if target is true color type (3 channels) it copies Red or Green or Blue channel into Red or Green or Blue channel or the mixed gray scale image into Alpha
                  - if target is true color alpha type (4 channels) it copies Red or Green or Blue or Alpha channel into Red or Green or Blue or Alpha channel
                 */
                case 0: { composite_op = CopyRedCompositeOp; break; }
                case 1: { composite_op = CopyGreenCompositeOp; break; }
                case 2: { composite_op = CopyBlueCompositeOp; break; }
                case 3: { composite_op = CopyAlphaCompositeOp; break; }
                default: {
                    // here we copy channels from the target to preserve them when, for example, target has 2 channels and the output cloned from target got changed to have more channels
                    // in which case output's alpha channel (from a Gray/A image type) gets lost. So we copy it from the target.
                    char * channelTypeOutputWrite = channelTypeAsString(channels_output_write[i]);
                    char * channelTypeTargetRead = channelTypeAsString(channels_target_read[i]);
                    printf("Copying channel %d from target: %s <= %s \n", i, channelTypeOutputWrite, channelTypeTargetRead);
                    free(channelTypeOutputWrite);
                    free(channelTypeTargetRead);
                    if (channels_target_read[i] == UndefinedChannel) continue;
                    copy_channel(output_wand, target_wand, channels_output_write[i], channels_target_read[i]);
                }
            }
            if (composite_op == UndefinedCompositeOp) continue;
            printf("Copying channel %d from source using %s\n", i, compositeOperatorAsString(composite_op));
            MagickCompositeImage(output_wand, final_source_wands[i], composite_op, MagickFalse, 0, 0);
        }
    }
    // =========================== << Compositing each image ========================================


    // =========================== >> Writing output wand ========================================
    printf("Output wand:\n");
    print_info(output_wand, sample_pixel_x, sample_pixel_y);

    char output_wand_file_name[512];
    str_concat(output_wand_file_name, sizeof(output_wand_file_name),
               IMAGES_DIR,
               "output",
               ".", output_format,
               NULL);

    printf("\nWriting output wand to file: %s\n", output_wand_file_name);
    MagickBooleanType status = MagickWriteImage(output_wand, output_wand_file_name);
    if (status == MagickFalse) {
        return exit_with_error("Unable to write output image %s\n", output_wand_file_name);
    }
    DestroyMagickWand(target_wand);
    DestroyMagickWand(output_wand);

    // =========================== << Writing output wand ========================================


    // =========================== >> Reloading output wand ========================================
    MagickWand *reloaded_output_wand = NewMagickWand();
    if (MagickReadImage(reloaded_output_wand, output_wand_file_name) == MagickFalse) {
        return exit_with_error("Unable to reload image %s into reloaded_output_wand \n", output_wand_file_name);
    }
    printf("\nOutput wand after reloading from disk\n");
    print_info(reloaded_output_wand, sample_pixel_x, sample_pixel_y);
    // =========================== << Reloading output wand ========================================


    // =========================== >> Remaining cleanup ========================================
    for(int i = 0; i < num_source_paths; i++) {
        free(source_paths[i]);
        free(source_paths_no_dir[i]);
        DestroyMagickWand(source_wands[i]);
    }
    free(targets[0]); // we have just one target actually
    free(targets);
    free(source_paths);
    free(source_paths_no_dir);
    DestroyMagickWand(reloaded_output_wand);
    MagickWandTerminus();
    // =========================== << Remaining cleanup ========================================

    printf("Type something then press enter ...\n");
    getchar();
    return 0;
}