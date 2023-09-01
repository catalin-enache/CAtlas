// ImageMagick_INCLUDE_DIRS C:/z/msys64/mingw64/include/ImageMagick-7
// ImageMagick_LIBRARIES    C:/z/msys64/mingw64/lib/libMagickWand-7.Q16HDRI.dll.a

//#define MAGICKCORE_QUANTUM_DEPTH 32
//#define MAGICKCORE_HDRI_ENABLE 1
#include <ImageMagick-7/MagickWand/MagickWand.h>

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

    int config_entries_total = 17;
    const char *lines[] = {
            "cols 2\n\n",
            "shrink 0.92\n\n",
            "shrink_filter 22\n\n", // 0 - 31
            "sample_pixel_x 10\n\n",
            "sample_pixel_y 10\n\n",
            "uv_rect_color #ff000055\n\n",
            "output_bit_depth 0 // 8, 16, 32 \n\n",
            "output_image_type 0\n",
            "## GrayscaleType(2), GrayscaleAlphaType(3), TrueColorType(6), TrueColorAlphaType(7), ColorSeparationType(8), ColorSeparationAlphaType(9)\n\n",
            "output_colorspace 0\n",
            "## LinearGRAYColorspace(33),  GRAYColorspace(3), RGBColorspace(21 Linear), sRGBColorspace(23 - non-linear), CMYKColorspace (2) \n\n",
            "output_format png // png, tiff, exr, jpg\n\n",
            "output_quantum_format floating-point // floating-point, signed(work with 8bit)\n\n",
            "output_quantum_format_apply 0 // 0, 1\n\n",
            "output_exr_color_type RGBA // RGB, RGBA, YC, YCA, Y, YA, R, G, B, A\n\n",
            "output_exr_color_type_apply 0 // 0, 1\n\n",
            "output_compression_algorithm 20 // 0 no compression algorithm \n",
            "## ZipCompression(20)/LZWCompression(15) (TIFF) || \n",
            "## ZipCompression(20)/ZipSCompression(21)/PizCompression(17 lossy)/Pxr24Compression(18 lossy)/DWAACompression(24 lossy)/DWABCompression(25 lossy) (EXR) \n\n",
            "output_compression_strength 100 // 0 default compression strength \n",
            "## (100 strong compression smaller size, 1 soft compression larger size) \n",
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

    int cols = config_find(kv_arr, config_entries, "cols")->value.i;
    double shrink = config_find(kv_arr, config_entries, "shrink")->value.d;
    int shrink_filter = config_find(kv_arr, config_entries, "shrink_filter")->value.i;
    int sample_pixel_x = config_find(kv_arr, config_entries, "sample_pixel_x")->value.i;
    int sample_pixel_y = config_find(kv_arr, config_entries, "sample_pixel_y")->value.i;

    char uv_rect_color[16]; // #ff000055
    strcpy(uv_rect_color, config_find(kv_arr, config_entries, "uv_rect_color")->value.s);

    int output_bit_depth = config_find(kv_arr, config_entries, "output_bit_depth")->value.i;
    int output_image_type = config_find(kv_arr, config_entries, "output_image_type")->value.i;
    int output_colorspace = config_find(kv_arr, config_entries, "output_colorspace")->value.i;

    char output_format[16]; // png, jpg
    strcpy(output_format, config_find(kv_arr, config_entries, "output_format")->value.s);

    char output_quantum_format[32]; // floating-point, signed
    strcpy(output_quantum_format, config_find(kv_arr, config_entries, "output_quantum_format")->value.s);

    bool output_quantum_format_apply = config_find(kv_arr, config_entries, "output_quantum_format_apply")->value.i;

    char output_exr_color_type[32]; // floating-point, signed
    strcpy(output_exr_color_type, config_find(kv_arr, config_entries, "output_exr_color_type")->value.s);

    bool output_exr_color_type_apply = config_find(kv_arr, config_entries, "output_exr_color_type_apply")->value.i;

    int output_compression_algorithm = config_find(kv_arr, config_entries, "output_compression_algorithm")->value.i;
    int output_compression_strength = config_find(kv_arr, config_entries, "output_compression_strength")->value.i;
    int output_jpg_quality = config_find(kv_arr, config_entries, "output_jpg_quality")->value.i;

    free(kv_arr);

    printf("\nConfig values: \n"
           "\tcols %d, \n"
           "\tshrink %f, \n"
           "\tshrink_filter %d, \n"
           "\tsample_pixel_x %d, sample_pixel_y %d, \n"
           "\tuv_rect_color %s, \n"
           "\toutput_bit_depth %d, \n"
           "\toutput_image_type %d, output_colorspace %d, \n"
           "\toutput_format %s, \n"
           "\toutput_quantum_format %s, \n"
           "\toutput_compression_algorithm %d, \n"
           "\toutput_compression_strength %d, \n"
           "\toutput_jpg_quality %d \n",
           cols,
           shrink,
           shrink_filter,
           sample_pixel_x, sample_pixel_y,
           uv_rect_color,
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
    char **image_paths = list_files_with_filter(IMAGES_DIR, &num_paths, true, is_image_file_and_not_atlas);
    char **image_paths_no_dir = list_files_with_filter(IMAGES_DIR, &num_paths, false, is_image_file_and_not_atlas);
    if (image_paths == NULL)
        return exit_with_error("Could not list files.\n");
    if (num_paths == 0) {
        return exit_with_error("No image found.\n");
    }

    printf("\nFound %d files \n", num_paths);

    int rows = num_paths / cols + (num_paths % cols == 0 ? 0 : 1);

    // =========================== << List images ========================================

    MagickWandGenesis();

    MagickWand * wand = NewMagickWand();
    if(wand == NULL) return exit_with_error("Unable to create wand\n");

    MagickWand * output_rows[rows];

    printf("Creating %d output rows...\n", rows);
    for(int i = 0; i < rows; i++) {
        output_rows[i] = NewMagickWand();
        if(output_rows[i] == NULL) return exit_with_error("Unable to create output_rows[%d]\n", i);
    }

//    MagickWand **input_wands = (MagickWand**)malloc(num_paths * sizeof(MagickWand*));
    MagickWand* input_wands[num_paths];
    size_t** input_sizes = (size_t**)malloc(num_paths * sizeof(size_t*)); // TODO: free these

    for(int i = 0; i < num_paths; i++) {
        // =========================== >> Read each image ========================================
        printf("Reading image path[%d] = %s\n", i, image_paths[i]);
        input_wands[i] = NewMagickWand();
        if(input_wands[i] == NULL) return exit_with_error("Unable to create input_wands[%d]\n", i);
        if (MagickReadImage(input_wands[i], image_paths[i]) == MagickFalse) return exit_with_error("Unable to read image %s \n", image_paths[i]);

        input_sizes[i] = (size_t*)malloc(2 * sizeof(size_t));
        input_sizes[i][0] = MagickGetImageWidth(input_wands[i]);
        input_sizes[i][1] = MagickGetImageHeight(input_wands[i]);

        printf("Original image[%d] %s\n", i, image_paths[i]);
        print_info(input_wands[i], sample_pixel_x, sample_pixel_y);
//        used with alternate way to create atlas using MagickMontageImage (see code after)
//        printf("Adding image[%d] %s to wand\n", i, image_paths[i]);
//        if(MagickAddImage(wand, input_wands[i]) == MagickFalse) {
//            return exit_with_error("Unable to add image %s to wand\n", image_paths[i]);
//        }

        // =========================== >> Zoom out ========================================
        // TODO: add an option to scale down the image (according with smallest image size) before adding it to the atlas
        input_wands[i] = zoom_out(input_wands[i], shrink, shrink_filter); // LanczosFilter
        // =========================== << Zoom out ========================================
        // TODO: add option to insert image to complete the requested rows (if rows are defined in config)
        int curr_row = i / cols;
        printf("Adding image[%d] %s to output_rows[%d]\n", i, image_paths[i], curr_row);
        if(MagickAddImage(output_rows[curr_row], input_wands[i]) == MagickFalse) {
            return exit_with_error("Unable to add image %s to output_rows[%d]\n", image_paths[i], i/rows);
        }
        printf("------------------------------------\n");
        // =========================== << Read each image ========================================
    }
    // END of input_wands iteration : for(int i = 0; i < num_paths; i++) input_wands[i] = NewMagickWand();
    printf("Creating rows by appending output_rows[i] and replacing output_rows[i] with the result\n");
    for(int i = 0; i < rows; i++) {
        MagickResetIterator(output_rows[i]);
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
    MagickWand * output_wand = MagickAppendImages(atlas, MagickTrue); // false => horizontally, true => vertically
    if (output_wand == NULL) return exit_with_error("Unable to append images to output_wand\n");

    printf("output_wand created\n");

    // =========================== >> Config Output Wand ==================
    printf("Configuring output wand\n");
    MagickSetImageFormat(output_wand, output_format);
    if (output_bit_depth) {
        // 16 bit exr to 16 bit tiff does not preserve the values unless we set quantum:format = floating-point (see later)
        MagickSetImageDepth(output_wand, output_bit_depth);
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
    }

    if(strcmp(output_format, "tiff") == 0 && output_quantum_format_apply) {
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

    if (strcmp(output_format, "exr") == 0 && output_exr_color_type_apply) {
        MagickSetOption(output_wand, "exr:color-type", output_exr_color_type); // RGB, RGBA, YC, YCA, Y, YA, R, G, B, A
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

    /*
    // Alternate way to create atlas using MagickMontageImage
    MagickResetIterator(wand);
    DrawingWand *drawing_wand = NewDrawingWand();
    // tile("2x2+0+0") Set the montage grid size and spacing
    // geometry("2048x2048+0+0") Set the tile size and offset inside final frame
    // frame("0x0+0+0") Surround the image with an ornamental border (e.g. 15x15+3+3). The frame color is that of the thumbnail's matte color.
    // MagickMontageImage(wand, drawing_wand, tile("2x2+0+0"), geometry("2048x2048+0+0"), UnframeMode, frame("0x0+0+0"))
    MagickWand *montage_wand = MagickMontageImage(wand, drawing_wand, "2x2+0+0", "2048x2048+0+0", ConcatenateMode, "0x0+0+0");
    if (montage_wand == NULL) return exit_with_error("Unable to create montage_wand\n");

    MagickWriteImage(montage_wand, "./atlas/montage_wand.atlas.png");
    DestroyMagickWand(montage_wand);
    DestroyDrawingWand(drawing_wand);
    */

    int uv_help_lines_num = 0;
    UVCorners* uvCornersArr = get_uv_corners_arr(num_paths, input_sizes, cols, shrink);
    char **uv_help_lines = print_UV_help(uvCornersArr, num_paths, cols, &uv_help_lines_num);
    if (uv_help_lines == NULL) return exit_with_error("Could not get uv_help_lines.\n");

    if (WRITE_LINES_ERROR == write_lines(ATLAS_UV_HELP_FILE, (const char**)uv_help_lines, uv_help_lines_num, false))
        return exit_with_error("Could not write UV help lines.\n");

    printf("\nUV help:\n");
    for (int i = 0; i < uv_help_lines_num; i++) {
        printf(uv_help_lines[i]);
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

    // =========================== >> Remaining cleanup ========================================
    printf("Cleaning up\n");
    DestroyMagickWand(output_wand);
    DestroyMagickWand(wand);
    DestroyMagickWand(atlas);
    for(int i = 0; i < rows; i++) {
        DestroyMagickWand(output_rows[i]);
    }
    for(int i = 0; i < num_paths; i++) {
        DestroyMagickWand(input_wands[i]);
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