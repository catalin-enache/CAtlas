#include <stdio.h>
#include <vips/vips.h>
#include <string.h>
#include "./src/img_manip.h"
#include "./src/dir_files_utils.h"
#include "./src/extract_config.h"

#define IMAGES_DIR "./atlas/"
#define ATLAS_FILE "./atlas/atlas.png"
#define ATLAS_CONFIG_FILE "./atlas/config.txt"

int
main( int argc, char **argv )
{
//    for (int i = 0; i < argc; i++) {
//        printf("argc[%d] = %s\n", i, argv[i]);
//    }
    if( VIPS_INIT( argv[0] ) )
        vips_error_exit( NULL );

//    int num_lines;
//    char** lines = read_lines(ATLAS_CONFIG_FILE, &num_lines);
//    if (lines == NULL) {
//        printf("Could not read lines\n");
//        return 1;
//    }

    int kvErr;
    int num_entries;
    KeyValue* kvArr = extract_config(ATLAS_CONFIG_FILE, &num_entries, &kvErr);
    printf("num_entries %d\n", num_entries);

    if (kvErr) {
        printf("kvErr %d\n", kvErr); // TODO: whyyyyyy ??? kvErr 32758
    }

//    for (int i = 0; i < num_entries; i++) {
//        printKeyValue(kvArr[i]);
//    }

    KeyValue *shrinkKV;
    if(config_find(kvArr, num_entries, "shrink", &shrinkKV) != 0) {
        printf("Could not find shrinkKV");
        return 1;
    }
    double shrink = shrinkKV->value.d;
    free(shrinkKV);
    printf("shrink: %lf\n", shrink);

//    free(kvArr);


//    if( argc != 3 )
//        vips_error_exit( "usage: %s infile outfile", argv[0] );

    int num_paths;
    char **paths = list_files(IMAGES_DIR, &num_paths, 1);
    if (paths == NULL) {
        printf("Could not list files\n");
        return 1;
    }

    VipsImage *in_array[num_paths];
    VipsImage *out_array[num_paths];

    for(int i = 0; i < num_paths; i++) {
        printf("new image from %s\n", paths[i]);
        if( !(in_array[i] = vips_image_new_from_file( paths[i], NULL )) )
            vips_error_exit( NULL );
        free(paths[i]);
        zoom_out(in_array[i], &(out_array[i]), 0.92, 0.92);
        g_object_unref(in_array[i]);
    }
    free(paths);

    VipsImage *atlas = NULL;
    if (vips_arrayjoin(out_array, &atlas,  num_paths, "across", 2, NULL))
        vips_error_exit(NULL);

    for(int i = 0; i < num_paths; i++) g_object_unref(out_array[i]);

    if (vips_pngsave(atlas, ATLAS_FILE, "compression", 6, NULL))
        vips_error_exit(NULL);

    g_object_unref(atlas);

    vips_shutdown();
//    getchar();
    return 0;
}