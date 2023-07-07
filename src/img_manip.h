
#ifndef ATLAS_IMG_MANIP_H
#define ATLAS_IMG_MANIP_H
void zoom_out(VipsImage *in, VipsImage **out, double shrink, int *h_shrinked, int *v_shrinked);
void hdr_to_ldr_naive_linear_processing(VipsImage *in, VipsImage **out);
double** get_min_max_for_each_band(VipsImage *in);
void print_pixel(VipsImage *in, int x, int y);
#endif //ATLAS_IMG_MANIP_H
