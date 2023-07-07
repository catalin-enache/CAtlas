#include <stdio.h>
#include <vips/vips.h>
#include <string.h>

#include "./src/extract_config.h"
#include "./src/errors.h"
#include "./src/img_manip.h"
#include "./src/dir_files_utils.h"
#include "./src/calculations.h"
#include "./src/utils.h"

#define IMAGES_DIR "./extract_channels/"
#define ATLAS_CONFIG_FILE "./extract_channels/config.txt"

int main(int argc, char **argv) {
    if(VIPS_INIT(argv[0]))
        return exit_with_error("Could not initialize VIPS");

    if(MAKE_DIR_ERROR == make_dir_if_not_exists(IMAGES_DIR))
        return exit_with_error("Dir %s does not exist.\n", IMAGES_DIR);

    const char *lines[] = {"vips_interpretation 22\n"};
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

    int vips_interpretation = config_find(kv_arr, num_entries, "vips_interpretation")->value.i;

    free(kv_arr);

    int num_paths = 0;
    char **paths = list_files_with_filter(IMAGES_DIR, &num_paths, true, is_image_file_and_not_extracted);
    if (paths == NULL)
        return exit_with_error("Could not list files.\n");
    if (num_paths == 0) {
        return exit_with_error("No image found.\n");
    }

    VipsImage *in_array[num_paths];

    for (int i = 0; i < num_paths; i++) {
        // TODO: define these in config
        int sample_pixel_x = 10;
        int sample_pixel_y = 10;

        if(!(in_array[i] = vips_image_new_from_file( paths[i], NULL)))
            return exit_with_error("Could not create image from file %s.\n", paths[i]);

        printf("Sample Before Interpretation => "); print_pixel(in_array[i], sample_pixel_x, sample_pixel_y);

        VipsInterpretation guessed_interpretation = vips_image_guess_interpretation(in_array[i]);
        double ** min_max_1 = get_min_max_for_each_band(in_array[i]);
        if (min_max_1 == NULL) return exit_with_error("Could not get min_max for file before vips interpretation %s.\n", paths[i]);
        char *info_1 = print_2d_array_of_double(min_max_1, in_array[i]->Bands, 2);
        for (int j = 0; j < in_array[i]->Bands; j++) free(min_max_1[j]);
        free(min_max_1);

        // TODO: make this conditional in config
        if(vips_colourspace(in_array[i], &in_array[i], vips_interpretation, NULL)) {
            return exit_with_error("Could not set interpretation %d for file %s.\n", vips_interpretation, paths[i]);
        } // VIPS_INTERPRETATION_sRGB

        printf("Sample After Interpretation  => "); print_pixel(in_array[i], sample_pixel_x, sample_pixel_y);

        VipsInterpretation applied_interpretation = vips_image_guess_interpretation (in_array[i]);
        double ** min_max_2 = get_min_max_for_each_band(in_array[i]);
        if (min_max_2 == NULL) return exit_with_error("Could not get min_max for file after vips interpretation %s.\n", paths[i]);
        char *info_2 = print_2d_array_of_double(min_max_2, in_array[i]->Bands, 2);
        for (int j = 0; j < in_array[i]->Bands; j++) free(min_max_2[j]);
        free(min_max_2);


        printf("Found %d bands image: %s\n", in_array[i]->Bands, paths[i]);
        printf("\tguessed_interpretation %d, applied_interpretation: %d\n", guessed_interpretation, applied_interpretation);
        printf("\tmin_max before interpretation: %s\n", info_1);
        printf("\tmin_max after interpretation: %s\n", info_2);

        free(info_1);
        free(info_2);

        for (int j = 0; j < in_array[i]->Bands; j++) {
            VipsImage *curr_channel = NULL;
            if (vips_extract_band(in_array[i], &curr_channel, j, NULL)) {
                return exit_with_error("Could not extract channel %d from file %s.\n", 0, paths[i]);
            }

            char channel_file_name[MAX_LINE_LENGTH];
            strncpy(channel_file_name, paths[i], sizeof(channel_file_name) - 1);
            strncat(channel_file_name, ".", sizeof(channel_file_name) - strlen(channel_file_name) - 1);
            char channel_file_name_b[MAX_LINE_LENGTH];
            sprintf(channel_file_name_b, "%d", j);
            strncat(channel_file_name, channel_file_name_b, sizeof(channel_file_name) - strlen(channel_file_name) - 1);
            strncat(channel_file_name, ".jpg", sizeof(channel_file_name) - strlen(channel_file_name) - 1);
            printf("Writing to file: %s\n", channel_file_name);
            if (vips_image_write_to_file(curr_channel, channel_file_name, NULL)) {
                return exit_with_error("Could not write to file %s.\n", channel_file_name);
            }
            g_object_unref(curr_channel);
        }

        free(paths[i]);
        g_object_unref(in_array[i]);
    }

    free(paths);
}