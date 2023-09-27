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

#define IMAGES_DIR "./extract_channels/"
#define TEST_DIR "./extract_channels/test/"
#define CONFIG_FILE "./extract_channels/config.txt"

int main(int argc, char **argv) {
    if(MAKE_DIR_ERROR == make_dir_if_not_exists(IMAGES_DIR))
        return exit_with_error("Dir %s does not exist.\n", IMAGES_DIR);
    if(MAKE_DIR_ERROR == make_dir_if_not_exists(TEST_DIR))
        return exit_with_error("Dir %s does not exist.\n", TEST_DIR);

    // =========================== >> Write config ========================================

    int config_entries_total = 11;
    const char *lines[] = {
            "extract_icc_profile 1\n\n",
            "sample_pixel_x 10\n",
            "sample_pixel_y 10\n\n",
            "output_bit_depth 32 // 8, 16, 32 \n\n",
            "output_image_type 0\n",
            "## GrayscaleType(2), GrayscaleAlphaType(3), TrueColorType(6), TrueColorAlphaType(7), ColorSeparationType(8), ColorSeparationAlphaType(9)\n\n",
            "output_colorspace 0\n",
            "## LinearGRAYColorspace(33),  GRAYColorspace(3), RGBColorspace(21 Linear), sRGBColorspace(23 - non-linear), CMYKColorspace (2) \n\n",
            "output_format tiff // png, tiff, exr, jpg\n\n",
            "output_quantum_format floating-point // floating-point, signed\n\n",
            "output_compression_algorithm 20 \n",
            "## ZipCompression(20)/LZWCompression(15) (TIFF) || \n",
            "## ZipCompression(20)/ZipSCompression(21)/PizCompression(17 lossy)/Pxr24Compression(18 lossy)/DWAACompression(24 lossy)/DWABCompression(25 lossy) (EXR) \n\n",
            "output_compression_strength 1 \n",
            "## (100 string compression smaller size, 1 soft compression larger size) \n",
            "## affects png size ; and tiff size with ZipCompression (not with LZWCompression)\n\n",
            "output_jpg_quality 100 // affects jpg size (100 higher Q larger size, 1 lower Q smaller size)\n"
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

    bool extract_icc_profile = config_find(kv_arr, config_entries, "extract_icc_profile")->value.i;
    int sample_pixel_x = config_find(kv_arr, config_entries, "sample_pixel_x")->value.i;
    int sample_pixel_y = config_find(kv_arr, config_entries, "sample_pixel_y")->value.i;
    int output_bit_depth = config_find(kv_arr, config_entries, "output_bit_depth")->value.i;
    ImageType output_image_type = (ImageType)config_find(kv_arr, config_entries, "output_image_type")->value.i;
    ColorspaceType output_colorspace = (ColorspaceType)config_find(kv_arr, config_entries, "output_colorspace")->value.i;

    char output_format[16]; // png, jpg
    strcpy(output_format, config_find(kv_arr, config_entries, "output_format")->value.s);

    char output_quantum_format[32]; // floating-point, signed
    strcpy(output_quantum_format, config_find(kv_arr, config_entries, "output_quantum_format")->value.s);

    CompressionType output_compression_algorithm = (CompressionType)config_find(kv_arr, config_entries, "output_compression_algorithm")->value.i;
    int output_compression_strength = config_find(kv_arr, config_entries, "output_compression_strength")->value.i;
    int output_jpg_quality = config_find(kv_arr, config_entries, "output_jpg_quality")->value.i;

    free(kv_arr);

    printf("\nConfig values: \n"
           "\textract_icc_profile %d, \n"
           "\tsample_pixel_x %d, sample_pixel_y %d, \n"
           "\toutput_bit_depth %d, \n"
           "\toutput_image_type %d, output_colorspace %d, \n"
           "\toutput_format %s, \n"
           "\toutput_quantum_format %s, \n"
           "\toutput_compression_algorithm %d, \n"
           "\toutput_compression_strength %d, \n"
           "\toutput_jpg_quality %d \n",
           extract_icc_profile,
           sample_pixel_x, sample_pixel_y,
           output_bit_depth,
           output_image_type, output_colorspace,
           output_format, output_quantum_format,
           output_compression_algorithm,
           output_compression_strength,
           output_jpg_quality
       );

    // =========================== << Read config ========================================

    // =========================== >> List images ========================================

    int num_paths = 0;
    char **image_paths = list_files_with_filter(IMAGES_DIR, &num_paths, true, is_image_file_and_not_extracted);
    char **image_paths_no_dir = list_files_with_filter(IMAGES_DIR, &num_paths, false, is_image_file_and_not_extracted);
    if (image_paths == NULL)
        return exit_with_error("Could not list files.\n");
    if (num_paths == 0) {
        return exit_with_error("No image found.\n");
    }

    // =========================== << List images ========================================

    MagickWandGenesis();

//    MagickWand **input_wands = (MagickWand**)malloc(num_paths * sizeof(MagickWand*));
    MagickWand * input_wands[num_paths];
    printf("Found %d files \n", num_paths);

    for(int i = 0; i < num_paths; i++) {
        // =========================== >> Read each image ========================================
        printf("Reading image path[%d] = %s\n", i, image_paths[i]);
        input_wands[i] = NewMagickWand();
        if(input_wands[i] == NULL) return exit_with_error("Unable to create input_wands[%d]\n", i);
        if (MagickReadImage(input_wands[i], image_paths[i]) == MagickFalse) return exit_with_error("Unable to read image %s \n", image_paths[i]);


        char *input_image_format = MagickGetImageFormat(input_wands[i]);
        int input_image_depth = MagickGetImageDepth(input_wands[i]);
        ColorspaceType input_colorspace = MagickGetColorspace(input_wands[i]);
        ColorspaceType input_image_colorspace = MagickGetImageColorspace(input_wands[i]);
        ImageType input_type = MagickGetType(input_wands[i]);
        ImageType input_image_type = MagickGetImageType(input_wands[i]);

        printf("Original image[%d] %s\n", i, image_paths[i]);
        print_info(input_wands[i], sample_pixel_x, sample_pixel_y);

        size_t input_profile_length;
        unsigned char *input_profile = MagickGetImageProfile(input_wands[i], "ICC", &input_profile_length); // ICC, IPTC

        // write profile to file
        if (extract_icc_profile && input_profile_length > 0) {
            char icc_profile_path[128];
            str_concat(icc_profile_path, sizeof(icc_profile_path), image_paths[i], ".profile.icc", NULL);
            printf("Writing profile to %s\n", icc_profile_path);
            FILE *file;
            file = fopen(icc_profile_path, "wb");
            if (file == NULL) return exit_with_error("Error opening %s!", icc_profile_path);
            fwrite(input_profile, 1, input_profile_length, file);
            fclose(file);
        }

        int channels_num = 0;
        PixelChannel *pixelChannels = get_image_channels(input_wands[i], &channels_num);

        // =========================== << Read each image ========================================


        MagickWand *channels[channels_num];
        for (int j = 0; j < channels_num; j++) {
            // =========================== >> Extract channel and save it ========================================
            channels[j] = CloneMagickWand(input_wands[i]);
            MagickSeparateImage(channels[j], getChannelFromPixelChannel(pixelChannels[j]));
            // channels[j] = MagickChannelFxImage(input_wand, "r"); // r, g, b, a //  alternate way to get the channels

            printf("===========================\n");
            printf("channel: %d (before settings)\n", j);
            print_info(channels[j], sample_pixel_x, sample_pixel_y);
            printf("-----------------------------\n");

            MagickSetImageFormat(channels[j], output_format);
            // 16 bit exr to 16 bit tiff does not preserve the values unless we set quantum:format = floating-point (see later)
            MagickSetImageDepth(channels[j], output_bit_depth);

            if (output_colorspace) {
                MagickSetColorspace(channels[j], output_colorspace); // LinearGRAYColorspace  GRAYColorspace
                MagickSetImageColorspace(channels[j], output_colorspace);
            }

            if (output_image_type) {
                // GrayscaleType TrueColorType PaletteType ColorSeparationType
                // This does NOT work for saving exr as one grayscale image.
                // for that we have: exr:color-type=Y
                MagickSetType(channels[j], output_image_type);
                MagickSetImageType(channels[j], output_image_type);
            }

            if(strcmp(output_format, "tiff") == 0) {
                // for tiff to store values as integers or as floating-point / do not apply this on non tiff output, it will mess min/max
                MagickSetOption(channels[j], "quantum:format", output_quantum_format); // floating-point | signed
                /*
                 Set the type to floating-point to specify a floating-point format for raw files (e.g. GRAY)
                 or for MIFF and TIFF images in HDRI mode to preserve negative values.
                 If -depth 16 is included, the result is a single precision floating point format.
                 If -depth 32 is included, the result is double precision floating point format.
                 For signed pixel data, use -define quantum:format=signed
                 */
            }

            if (strcmp(output_format, "exr") == 0) {
                MagickSetOption(channels[j], "exr:color-type", "Y"); // !!!! exr:color-type=Y // works for exr to save as one grayscale channel
                // but when opening the image in ImageMagick it is not interpreted as one grayscale channel (Gimp though sees it as one grayscale channel)
                // (no exr image opened in IM seems to be interpreted as one grayscale channel even if they are)
                // When the image type is Grayscale(Alpha) (and not TrueColor(Alpha)) it might be safe though to just read the first channel
                // However when saving tiff/png/jpg the image is saved and interpreted as one grayscale channel
                // without setting any option like we need for exr (exr:color-type=Y)
            }

            if (strcmp(output_format, "jpg") == 0) {
                // affects jpg size and pixel values
                // (1 lowQ/lower size, 100 highQ/higher size)
                MagickSetImageCompressionQuality(channels[j], output_jpg_quality);
            }

            bool is_tiff_or_exr = strcmp(output_format, "tiff") == 0 || strcmp(output_format, "exr") == 0;
            if (output_compression_algorithm && is_tiff_or_exr) {
                // ZipCompression(20)/LZWCompression(15) (TIFF) ZipCompression(20)/ZipSCompression(21)/PizCompression(17 lossy)/Pxr24Compression(18 lossy)/DWAACompression(24 lossy)/DWABCompression(25 lossy) (EXR)
                MagickSetCompression(channels[j], output_compression_algorithm);
            }

            if (output_compression_strength) {
                // (100 smaller, 1 bigger)
                // affects png size  / tiff size with ZipCompression (not with LZWCompression)
                MagickSetCompressionQuality(channels[j], output_compression_strength);
            }



            printf("channel: %d (after settings applied)\n", j);
            print_info(channels[j], sample_pixel_x, sample_pixel_y);
            printf("-----------------------------\n");

            char channel_file_name[512];
            str_concat(channel_file_name, sizeof(channel_file_name),
                       image_paths[i], ".",
                       pixelChannels[j] == RedPixelChannel ? "0" : pixelChannels[j] == GreenPixelChannel ? "1" : pixelChannels[j] == BluePixelChannel ? "2" : "3",
                       ".", output_format,
                       NULL);
            printf("Writing single channel to file: %s\n", channel_file_name);
            MagickWriteImage(channels[j], channel_file_name);

            // =========================== << Extract channel and save it ========================================

            // =========================== >> Read channel ========================================
            DestroyMagickWand(channels[j]);
            channels[j] = NewMagickWand();

            printf("Reloading single channel from file: %s\n", channel_file_name);
            if (MagickReadImage(channels[j], channel_file_name) == MagickFalse) {
                return exit_with_error("Unable to read image %s\n", channel_file_name);
            }

            printf("channel: %d (after reloading from disk)\n", j);
            print_info(channels[j], sample_pixel_x, sample_pixel_y);

            // =========================== << Read channel ========================================
        }
        // END of channels iteration : for (int j = 0; j < channels_num; j++) channels[j] = CloneMagickWand(input_wands[i]);

        // =========================== >> Create composite_wand for testing each image ========================================

        int width = MagickGetImageWidth(channels[0]);
        int height = MagickGetImageHeight(channels[0]);
        PixelWand *pixel_wand = NewPixelWand();
        if (input_image_colorspace == CMYKColorspace) {
            PixelSetColor(pixel_wand, "cmyk(0%,0%,0%,0%)"); // black
        } else {
            PixelSetColor(pixel_wand, "#000000");
        }
        MagickWand * composite_wand = NewMagickWand();

        MagickNewImage(composite_wand, width, height, pixel_wand);
        DestroyPixelWand(pixel_wand);

        MagickSetImageFormat(composite_wand, input_image_format); //  ex: EXR, TIFF
        MagickSetDepth(composite_wand, input_image_depth); // preserving: exr to tiff (16 => 32), exr to exr (16 => 16)
        MagickSetImageDepth(composite_wand, input_image_depth); // 8 16 32
        MagickSetColorspace(composite_wand, input_colorspace); // sRGBColorspace RGBColorspace GRAYColorspace CMYKColorspace
        MagickSetImageColorspace(composite_wand, input_image_colorspace);
        MagickSetType(composite_wand, input_type);
        MagickSetImageType(composite_wand, input_image_type); // GrayscaleType TrueColorType PaletteType ColorSeparationType

        MagickSetCompressionQuality(composite_wand, 1); // affects png size (100 smaller, 1 bigger) / tiff size with ZipCompression (not with LZWCompression) (100 smaller, 1 bigger)
        MagickSetCompression(composite_wand, ZipCompression); // ZipCompression/LZWCompression (TIFF) ZipCompression/ZipSCompression/PizCompression/Pxr24Compression/DWAACompression/DWABCompression (EXR)
        MagickSetImageCompressionQuality(composite_wand, 100); // affects jpg size (100 higher Q larger size, 1 lower Q smaller size)

        if (input_profile_length) {
            MagickProfileImage(composite_wand, "icc", input_profile, input_profile_length);
        }

        printf("\ncomposite_wand merging channels\n");
        for (int j = 0; j < channels_num; j++) {
            if (input_image_colorspace == CMYKColorspace) {
                switch (j) {
                    case 0: MagickCompositeImage(composite_wand, channels[j], CopyCyanCompositeOp, MagickFalse, 0, 0); break;
                    case 1: MagickCompositeImage(composite_wand, channels[j], CopyMagentaCompositeOp, MagickFalse, 0, 0); break;
                    case 2: MagickCompositeImage(composite_wand, channels[j], CopyYellowCompositeOp, MagickFalse, 0, 0); break;
                    case 3: MagickCompositeImage(composite_wand, channels[j+1], CopyBlackCompositeOp, MagickFalse, 0, 0); break;
                }
            } else {
                switch (j) {
                    case 0: MagickCompositeImage(composite_wand, channels[j], CopyRedCompositeOp, MagickFalse, 0, 0); break;
                    case 1: MagickCompositeImage(composite_wand, channels[j], CopyGreenCompositeOp, MagickFalse, 0, 0); break;
                    case 2: MagickCompositeImage(composite_wand, channels[j], CopyBlueCompositeOp, MagickFalse, 0, 0); break;
                    case 3: MagickCompositeImage(composite_wand, channels[j], CopyAlphaCompositeOp, MagickFalse, 0, 0); break;
                }
            }

            DestroyMagickWand(channels[j]);
        }

        DestroyMagickWand(input_wands[i]);

        printf("\ncomposite_wand test in memory\n");
        print_info(composite_wand, sample_pixel_x, sample_pixel_y);

        char composite_wand_file_name[512];
        str_concat(composite_wand_file_name, sizeof(composite_wand_file_name),
                   TEST_DIR, image_paths_no_dir[i], ".",
                   "merge_test",
                   ".", input_image_format,
                   NULL);


        printf("\nWriting composite wand merge test to file: %s\n", composite_wand_file_name);
        MagickWriteImage(composite_wand, composite_wand_file_name);

        MagickRelinquishMemory(input_image_format);
        MagickRelinquishMemory(input_profile);
        DestroyMagickWand(composite_wand);
        composite_wand = NewMagickWand();

        // =========================== << Create composite_wand for testing each image ========================================

        // =========================== >> Read composite_wand ========================================

        if (MagickReadImage(composite_wand, composite_wand_file_name) == MagickFalse) {
            return exit_with_error("Unable to reload image %s into composite_wand \n", composite_wand_file_name);
        }

        printf("\ncomposite_wand after reloading from disk\n");
        print_info(composite_wand, sample_pixel_x, sample_pixel_y);

        DestroyMagickWand(composite_wand);
        free(pixelChannels);

        // =========================== << Read composite_wand ========================================
    }
    // END of input_wands iteration : for(int i = 0; i < num_paths; i++) input_wands[i] = NewMagickWand();

    // =========================== >> Remaining cleanup ========================================
    for (int i = 0; i < num_paths; i++) {
        free(image_paths[i]);
        free(image_paths_no_dir[i]);
    }
    free(image_paths);
    free(image_paths_no_dir);
    MagickWandTerminus();
    // =========================== << Remaining cleanup ========================================

    printf("Type something then press enter ...\n");
    getchar();
    return 0;
}