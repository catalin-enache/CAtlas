#include <stdio.h>
#include <vips/vips.h>
#include <string.h>

#include "./src/extract_config.h"
#include "./src/errors.h"
#include "./src/img_manip.h"
#include "./src/dir_files_utils.h"

#define IMAGES_DIR "./atlas/"
#define ATLAS_FILE "./atlas/atlas.png"
#define ATLAS_CONFIG_FILE "./atlas/config.txt"

int
main( int argc, char **argv )
{
//    if( argc != 3 )
//        vips_error_exit( "usage: %s infile outfile", argv[0] );
//    for (int i = 0; i < argc; i++) {
//        printf("argc[%d] = %s\n", i, argv[i]);
//    }

    if(VIPS_INIT(argv[0]))
        return exit_with_error("Could not initialize VIPS");

    const char *lines[] = {"shrink 0.92\n", "cols 2\n", "compression 6\n"};
    write_lines(ATLAS_CONFIG_FILE, lines, sizeof(lines)/sizeof(lines[0]), true);

    int num_entries;
    KeyValue* kv_arr = extract_config(ATLAS_CONFIG_FILE, &num_entries);
    if (kv_arr == NULL)
        return exit_with_error("Could not extract kv_arr config\n");

    printf("Config lines found: %d\n", num_entries);

    for (int i = 0; i < num_entries; i++) {
        printf(" - ");
        print_key_value(kv_arr[i]);
    }

    double shrink = config_find(kv_arr, num_entries, "shrink")->value.d;
    int cols = config_find(kv_arr, num_entries, "cols")->value.i;
    int compression = config_find(kv_arr, num_entries, "compression")->value.i;
    free(kv_arr);
    printf("Config values: shrink: %lf, cols %d, compression %d \n", shrink, cols, compression);

    int num_paths;
    char **paths = list_files(IMAGES_DIR, &num_paths, true);
    if (paths == NULL) {
        printf("Could not list files\n");
        return 1;
    }

    VipsImage *in_array[num_paths];
    VipsImage *out_array[num_paths];

    for(int i = 0; i < num_paths; i++) {
        if( !(in_array[i] = vips_image_new_from_file( paths[i], NULL )) )
            vips_error_exit( NULL );
        int h_shrinked, v_shrinked;
        zoom_out(in_array[i], &(out_array[i]), shrink, shrink, &h_shrinked, &v_shrinked);
        printf("Found image: %s [%d * %d] => [%d * %d]\n", paths[i], in_array[i]->Xsize, in_array[i]->Ysize, h_shrinked, v_shrinked);
        free(paths[i]);
        g_object_unref(in_array[i]);
    }
    free(paths);

    VipsImage *atlas = NULL;
    if (vips_arrayjoin(out_array, &atlas,  num_paths, "across", cols, NULL))
        vips_error_exit(NULL);

    for(int i = 0; i < num_paths; i++) g_object_unref(out_array[i]);

    if (vips_pngsave(atlas, ATLAS_FILE, "compression", compression, NULL))
        vips_error_exit(NULL);

    g_object_unref(atlas);

    vips_shutdown();
    printf("\n\n\n");
    printf("====================== DONE ===========================\n");
    printf("Type something then press enter\n");
    getchar();
    return 0;
}
