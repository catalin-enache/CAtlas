
#include <vips/vips.h>

#include "img_manip.h"

void zoom_out(VipsImage *in, VipsImage **out, double shrink, int *h_shrinked, int *v_shrinked) {
    int width = vips_image_get_width(in);
    int height = vips_image_get_height(in);

    VipsImage *resized = NULL;
    if(vips_reduce(in, &resized, 1.0 / shrink, 1.0 / shrink, "kernel", VIPS_KERNEL_LANCZOS3, NULL ) ) {
        printf("Could not reduce image\n");
        *out = NULL;
        return;
    }

    int resized_width = vips_image_get_width(resized);
    int resized_height = vips_image_get_height(resized);
    *h_shrinked = resized_width;
    *v_shrinked = resized_height;

    VipsImage *replicated = NULL;
    if (vips_replicate(resized, &replicated, 3, 3, NULL)) {
        printf("Could not replicate image\n");
        *out = NULL;
        return;
    }

    int offset_x = resized_width - (width - resized_width) / 2;
    int offset_y = resized_height - (height - resized_height) / 2;

    VipsImage *cropped = NULL;
    if (vips_crop(replicated, &cropped, offset_x, offset_y, width, height, NULL)) {
        printf("Could not crop image\n");
        *out = NULL;
        return;
    }

    *out = cropped;
    g_object_unref( resized );
    g_object_unref( replicated );
    // no need to unref cropped since it is passed out.
}

void hdr_to_ldr_naive_linear_processing(VipsImage *in, VipsImage **out) {
    // See other ways to convert from HDR to LDR:
    // https://matiascodesal.com/blog/how-convert-hdr-image-ldr-using-python/
    // Following algorithm is something super basic suggested by ChatGPT
    VipsImage *hdr_to_ldr = in;
    // Add a small constant to avoid taking log of 0.
    // https://www.libvips.org/API/8.6/libvips-arithmetic.html#vips-linear1
    vips_linear1(hdr_to_ldr, &hdr_to_ldr, 1, 1e-6, NULL); // multiply with 1 and adds 0.000001

    // https://www.libvips.org/API/8.6/libvips-arithmetic.html#vips-math
    if (vips_math(hdr_to_ldr, &hdr_to_ldr, VIPS_OPERATION_MATH_LOG, NULL)) // apply LOG on each pixel
        return;

    // https://www.libvips.org/API/current/libvips-conversion.html#vips-scale
    /*
    How vips_scale works (explained by ChatGPT):
    The input_range is 200 - 100 = 100
    The output_range is 255 - 0 = 255
    The scale_factor would be 255 / 100 = 2.55
    The offset would be 0 - 100 * 2.55 = -255
    new_pixel_value = old_pixel_value * scale_factor + offset

    For HDR (0 - 1000) to LDR (0 - 255) would be:
    y = ((x - min_input) / (max_input - min_input)) * (max_output - min_output) + min_output
    Here, min_input = 0, max_input = 1000, min_output = 0, and max_output = 255.
    y = ((x - 0) / (1000 - 0)) * (255 - 0) + 0
    y = (x / 1000) * 255
    or
    scale_factor = 255/1000 = 0.255;
    x * scale_factor
    */
    if(vips_scale(hdr_to_ldr, &hdr_to_ldr, NULL))
        return;
    *out = hdr_to_ldr;
}