#include <stdio.h>
#include <vips/vips.h>
#include <string.h>

#include "./src/extract_config.h"
#include "./src/errors.h"
#include "./src/img_manip.h"
#include "./src/dir_files_utils.h"
#include "./src/calculations.h"

#define IMAGES_DIR "./atlas/"
#define ATLAS_FILE "./atlas/atlas.png"
#define ATLAS_CONFIG_FILE "./atlas/config.txt"

int len(char *s) {
    return strlen(s);
}

int
main( int argc, char **argv )
{
//    if( argc != 3 )
//        return exit_with_error("usage: %s infile outfile", argv[0]);
//    for (int i = 0; i < argc; i++) {
//        printf("argc[%d] = %s\n", i, argv[i]);
//    }
//    printf("argv[0] = %s\n", argv[0]);

//    RowCol row_col = get_row_col(3, 2);
//    printf("get_row_col: row => %d, col => %d\n", row_col.row, row_col.col);


    if(VIPS_INIT(argv[0]))
        return exit_with_error("Could not initialize VIPS");

    const char *lines[] = {"shrink 0.92\n", "cols 2\n", "compression 6\n"};
    int config_lines_length = sizeof(lines)/sizeof(lines[0]);
    WriteLinesStatus status = write_lines(ATLAS_CONFIG_FILE, lines, config_lines_length, true);
    if (status == WRITE_LINES_ERROR) return exit_with_error("Could not write config.\n");

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
    int compression = config_find(kv_arr, num_entries, "compression")->value.i;
    free(kv_arr);
    printf("\nConfig values: shrink: %lf, cols %d, compression %d \n\n", shrink, cols, compression);

    int num_paths = 0;
    char **paths = list_files(IMAGES_DIR, &num_paths, true);
    if (paths == NULL)
        return exit_with_error("Could not list files.\n");
    if (num_paths == 0) {
        return exit_with_error("No image found.\n");
    }

    VipsImage *in_array[num_paths];
    VipsImage *out_array[num_paths];

    int last_image_width = 0, last_image_height = 0, warning_for_different_size_emitted = 0;
    for(int i = 0; i < num_paths; i++) {
        if(!(in_array[i] = vips_image_new_from_file( paths[i], NULL )))
            return exit_with_error("Could not create image from file %s.\n", paths[i]);
        if (i > 0
        && (in_array[i]->Xsize != last_image_width || in_array[i]->Ysize != last_image_height)
        && !warning_for_different_size_emitted) {
            warning_for_different_size_emitted = 1;
        }
        last_image_width = in_array[i]->Xsize;
        last_image_height = in_array[i]->Ysize;
        int h_shrinked = 0, v_shrinked = 0;
        zoom_out(in_array[i], &(out_array[i]), shrink, shrink, &h_shrinked, &v_shrinked);
        printf("Found image: %s [%d * %d] => [%d * %d]\n", paths[i], in_array[i]->Xsize, in_array[i]->Ysize, h_shrinked, v_shrinked);
        free(paths[i]);
        g_object_unref(in_array[i]);
    }
    free(paths);
    if (warning_for_different_size_emitted)
        printf("\nWarning: At least one image does not match the size of other images.\n");


    VipsImage *atlas = NULL;
    if (vips_arrayjoin(out_array, &atlas,  num_paths, "across", cols, NULL))
        return exit_with_error("vips_arrayjoin failed.\n");

    for(int i = 0; i < num_paths; i++) g_object_unref(out_array[i]);

    if (vips_pngsave(atlas, ATLAS_FILE, "compression", compression, NULL))
        return exit_with_error("vips_pngsave failed.\n");

    g_object_unref(atlas);

    vips_shutdown();
    printf("\n\n\n");
    printf("====================== DONE ===========================\n");
    printf("Type something then press enter ...\n");
    getchar();
    return 0;
}