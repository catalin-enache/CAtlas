
#include <vips/vips.h>

void zoom_out(VipsImage *in, VipsImage **out, double hShrink, double vShrink) {
    printf( "in XSize = %d YSize = %d\n", in->Xsize, in->Ysize );

    int width = vips_image_get_width(in);
    int height = vips_image_get_height(in);

    VipsImage *resized = NULL;
    if(vips_reduce( in, &resized, 1/hShrink, 1/vShrink, "kernel", VIPS_KERNEL_LANCZOS3, NULL ) )
        vips_error_exit( NULL );

    int resized_width = vips_image_get_width(resized);
    int resized_height = vips_image_get_height(resized);

    printf( "resized XSize = %d YSize = %d\n", resized_width, resized_height );

    VipsImage *replicated = NULL;
    if (vips_replicate(resized, &replicated, 3, 3, NULL))
        vips_error_exit(NULL);

    int offsetX = resized_width - (width - resized_width) / 2;
    int offsetY = resized_height - (height - resized_height) / 2;

    VipsImage *cropped = NULL;
    if (vips_crop(replicated, &cropped, offsetX, offsetY, width, height, NULL))
        vips_error_exit(NULL);

    *out = cropped;
    g_object_unref( resized );
    g_object_unref( replicated );
    // no need to unref cropped since it is passed out.
}