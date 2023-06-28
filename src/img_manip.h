
#ifndef ATLAS_IMG_MANIP_H
#define ATLAS_IMG_MANIP_H
void zoom_out(VipsImage *in, VipsImage **out, double h_shrink, double v_shrink, int *h_shrinked, int *v_shrinked);
void hdr_to_ldr_naive_linear_processing(VipsImage *in, VipsImage **out);
#endif //ATLAS_IMG_MANIP_H
