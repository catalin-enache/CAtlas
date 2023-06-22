
#include <vips/vips.h>

#include "img_manip.h"

void zoom_out(VipsImage *in, VipsImage **out, double h_shrink, double v_shrink, int *h_shrinked, int *v_shrinked) {
    int width = vips_image_get_width(in);
    int height = vips_image_get_height(in);

    VipsImage *resized = NULL;
    if(vips_reduce(in, &resized, 1 / h_shrink, 1 / v_shrink, "kernel", VIPS_KERNEL_LANCZOS3, NULL ) )
        vips_error_exit( NULL );

    int resized_width = vips_image_get_width(resized);
    int resized_height = vips_image_get_height(resized);
    *h_shrinked = resized_width;
    *v_shrinked = resized_height;

    VipsImage *replicated = NULL;
    if (vips_replicate(resized, &replicated, 3, 3, NULL))
        vips_error_exit(NULL);

    int offset_x = resized_width - (width - resized_width) / 2;
    int offset_y = resized_height - (height - resized_height) / 2;

    VipsImage *cropped = NULL;
    if (vips_crop(replicated, &cropped, offset_x, offset_y, width, height, NULL))
        vips_error_exit(NULL);

    *out = cropped;
    g_object_unref( resized );
    g_object_unref( replicated );
    // no need to unref cropped since it is passed out.
}