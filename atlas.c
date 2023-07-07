#include <stdio.h>
#include <vips/vips.h>
#include <string.h>

#include "./src/extract_config.h"
#include "./src/errors.h"
#include "./src/img_manip.h"
#include "./src/dir_files_utils.h"
#include "./src/calculations.h"

#define IMAGES_DIR "./atlas/"
#define ATLAS_FILE "./atlas/atlas."
#define ATLAS_CONFIG_FILE "./atlas/config.txt"
#define ATLAS_UV_HELP_FILE "./atlas/uv_help.txt"

int
main( int argc, char **argv )
{
//    for (int i = 0; i < argc; i++) {
//        printf("argc[%d] = %s\n", i, argv[i]);
//    }
//    printf("argv[0] = %s\n", argv[0]);

    if(VIPS_INIT(argv[0]))
        return exit_with_error("Could not initialize VIPS");

    if(MAKE_DIR_ERROR == make_dir_if_not_exists(IMAGES_DIR))
        return exit_with_error("Dir %s does not exist.\n", IMAGES_DIR);

    const char *lines[] = {"shrink 0.92\n", "cols 2\n", "png_compression 6\n", "jpg_quality 100\n", "atlas_output_format png\n", "vips_interpretation 22\n"};
    int config_lines_length = sizeof(lines)/sizeof(lines[0]);
    if (WRITE_LINES_ERROR == write_lines(ATLAS_CONFIG_FILE, lines, config_lines_length, true))
        return exit_with_error("Could not write config.\n");

    int num_entries = 0;
    KeyValue* kv_arr = extract_config(ATLAS_CONFIG_FILE, &num_entries);
    if (kv_arr == NULL)
        return exit_with_error("Could not extract kv_arr config\n");

    printf("\nConfig lines found: %d (from %d)\n", num_entries, config_lines_length);
    if (num_entries != config_lines_length) return exit_with_error("Invalid number of config entries\n");

    for (int i = 0; i < num_entries; i++) {
        printf(" - ");
        print_key_value(kv_arr[i]);
    }

    double shrink = config_find(kv_arr, num_entries, "shrink")->value.d;
    int cols = config_find(kv_arr, num_entries, "cols")->value.i;
    int png_compression = config_find(kv_arr, num_entries, "png_compression")->value.i;
    int jpg_quality = config_find(kv_arr, num_entries, "jpg_quality")->value.i;
    int vips_interpretation = config_find(kv_arr, num_entries, "vips_interpretation")->value.i;
    char atlas_output_format[MAX_LINE_LENGTH]; // png, jpg
    strcpy(atlas_output_format, config_find(kv_arr, num_entries, "atlas_output_format")->value.s);

    char atlas_file_name[MAX_LINE_LENGTH];
    strncpy(atlas_file_name, ATLAS_FILE, sizeof(atlas_file_name) - 1);
    strncat(atlas_file_name, atlas_output_format, sizeof(atlas_file_name) - strlen(atlas_file_name) - 1);

    free(kv_arr);

    printf("\nConfig values: shrink: %lf, cols %d, png_compression %d, jpg_quality %d, atlas_output_format %s \n\n",
           shrink, cols, png_compression, jpg_quality, atlas_output_format
           );

    int num_paths = 0;
    char **paths = list_files_with_filter(IMAGES_DIR, &num_paths, true, is_image_file_and_not_atlas);
    if (paths == NULL)
        return exit_with_error("Could not list files.\n");
    if (num_paths == 0) {
        return exit_with_error("No image found.\n");
    }

    VipsImage *in_array[num_paths];
    VipsImage *out_array[num_paths];

    int last_image_width = 0, last_image_height = 0, warning_for_different_size_emitted = 0;
    for(int i = 0; i < num_paths; i++) {
        // TODO: define these in config
        int sample_pixel_x = 10;
        int sample_pixel_y = 10;

        if(!(in_array[i] = vips_image_new_from_file( paths[i], NULL )))
            return exit_with_error("Could not create image from file %s.\n", paths[i]);

        double minIn, maxIn;
        if(vips_min( in_array[i], &minIn, NULL))
            return exit_with_error("Could not find minIn value for image from file %s.\n", paths[i]);
        if(vips_max( in_array[i], &maxIn, NULL))
            return exit_with_error("Could not find maxIn value for image from file %s.\n", paths[i]);
        VipsInterpretation guessed_interpretation = vips_image_guess_interpretation(in_array[i]);
        printf("Sample Before Interpretation => "); print_pixel(in_array[i], sample_pixel_x, sample_pixel_y);

        // TODO: make this conditional in config
        if(vips_colourspace(in_array[i], &in_array[i], vips_interpretation, NULL)) {
            return exit_with_error("Could not set interpretation %d for file %s.\n", vips_interpretation, paths[i]);
        } // VIPS_INTERPRETATION_sRGB

        printf("Sample After Interpretation  => "); print_pixel(in_array[i], sample_pixel_x, sample_pixel_y);

        double minOut, maxOut;
        if(vips_min( in_array[i], &minOut, NULL))
            return exit_with_error("Could not find minOut value for image from file %s.\n", paths[i]);
        if(vips_max( in_array[i], &maxOut, NULL))
            return exit_with_error("Could not find maxOut value for image from file %s.\n", paths[i]);
        VipsInterpretation applied_interpretation = vips_image_guess_interpretation (in_array[i]);

        if (i > 0
        && (in_array[i]->Xsize != last_image_width || in_array[i]->Ysize != last_image_height)) {
            warning_for_different_size_emitted = 1;
        }
        last_image_width = in_array[i]->Xsize;
        last_image_height = in_array[i]->Ysize;

        if (false && ends_with_extension(paths[i], ".exr")) {
            printf("Normalizing %s\n", paths[i]);
            hdr_to_ldr_naive_linear_processing(in_array[i], &in_array[i]);
        }

        int h_shrinked = 0, v_shrinked = 0;
        zoom_out(in_array[i], &(out_array[i]), shrink, &h_shrinked, &v_shrinked);
        if (out_array[i] == NULL)
            return exit_with_error("Could not zoom_out image from file %s.\n", paths[i]);

        double finalMinOut, finalMaxOut;
        if(vips_min( out_array[i], &finalMinOut, NULL))
            return exit_with_error("Could not find finalMinOut value for image from file %s.\n", paths[i]);
        if(vips_max( out_array[i], &finalMaxOut, NULL))
            return exit_with_error("Could not find finalMaxOut value for image from file %s.\n", paths[i]);

        printf("Sample After Processing      => "); print_pixel(out_array[i], sample_pixel_x, sample_pixel_y);
        printf("Found %d bands image: %s\n", in_array[i]->Bands, paths[i]);
        printf("\tguessed_interpretation %d, applied_interpretation: %d\n", guessed_interpretation, applied_interpretation);
        printf("\t[%d * %d] shrinked to [%d * %d]\n",in_array[i]->Xsize, in_array[i]->Ysize, h_shrinked, v_shrinked);
        printf("\t[minIn %f, maxIn %f], after setting interpretation [minOut %f, maxOut %f], final result [finalMinOut %f, finalMaxOut %f]\n", minIn, maxIn, minOut, maxOut, finalMinOut, finalMaxOut);

        free(paths[i]);
        g_object_unref(in_array[i]);
    }

    free(paths);

    if (warning_for_different_size_emitted)
        printf("\nWarning: At least one image does not match the size of other images.\n");

    VipsImage *atlas = NULL;
    if (vips_arrayjoin(out_array, &atlas,  num_paths, NULL))
        return exit_with_error("vips_arrayjoin failed 1.\n");
    if(atlas == NULL) // might be redundant, but just in case
        return exit_with_error("vips_arrayjoin failed 2.\n");

    for(int i = 0; i < num_paths; i++) g_object_unref(out_array[i]);

    // https://www.libvips.org/API/current/VipsForeignSave.html#VipsSaveable
    // https://www.libvips.org/API/current/VipsForeignSave.html
    // saves tested so far: png jpg tiff
    if (strcmp(atlas_output_format, "png") == 0) {
        if (vips_pngsave(atlas, atlas_file_name, "compression", png_compression, NULL))
            return exit_with_error("vips_pngsave failed.\n");
    } else if (strcmp(atlas_output_format, "jpg") == 0) {
        if(vips_jpegsave(atlas, atlas_file_name, "Q", jpg_quality, NULL))
            return exit_with_error("vips_jpegsave failed.\n");
    } else if (strcmp(atlas_output_format, "tiff") == 0) {
        if(vips_tiffsave(atlas, atlas_file_name, NULL))
            return exit_with_error("vips_tiffsave failed.\n");
    } else {
        if(vips_image_write_to_file(atlas, atlas_file_name, NULL))
            return exit_with_error("vips_image_write_to_file failed.\n");
    }

    g_object_unref(atlas);

    int uv_help_lines_num = 0;
    char **uv_help_lines = uv_help(num_paths, cols, shrink, &uv_help_lines_num);
    if (uv_help_lines == NULL) return exit_with_error("Could not get uv_help_lines.\n");

    if (WRITE_LINES_ERROR == write_lines(ATLAS_UV_HELP_FILE, (const char**)uv_help_lines, uv_help_lines_num, false))
        return exit_with_error("Could not write UV help lines.\n");

    printf("\nUV help:\n");
    for (int i = 0; i < uv_help_lines_num; i++) {
        printf(uv_help_lines[i]);
        free(uv_help_lines[i]);
    }
    free(uv_help_lines);

    vips_shutdown();
    printf("\n\n\n");
    printf("====================== DONE ===========================\n");
    printf("Type something then press enter ...\n");
    getchar();
    return 0;
}