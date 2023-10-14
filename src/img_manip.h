#ifndef ATLAS_IMG_MANIP_H
#define ATLAS_IMG_MANIP_H

//#define MAGICKCORE_QUANTUM_DEPTH 32
//#define MAGICKCORE_HDRI_ENABLE 1
#include <MagickWand/MagickWand.h>
#include <libtiff/tiffio.h>
#include "utils.h"
#include "calculations.h"
#include "dir_files_utils.h"

size_t get_image_depth(MagickWand *wand);
ChannelType getChannelFromPixelChannel(PixelChannel pixelChannel);
PixelChannel* get_image_channels(MagickWand *wand, int *size_out);
double** get_min_max_for_each_band(MagickWand *wand);
double** get_min_max_for_each_band_2(MagickWand *wand);
uint16_t get_tiff_sample_format(const char *image_file_name);
char* print_min_max_for_each_band(MagickWand *wand);
char* colorSpaceAsString(ColorspaceType colorspace_type);
char* imageTypeAsString(ImageType image_type);
char* channelTypeAsString(ChannelType channel);
char * compositeOperatorAsString(CompositeOperator op);
char * tifSampleFormatAsString(uint16_t sampleFormat);
void print_pixel(MagickWand *wand, int x, int y);
void print_info(MagickWand *wand, int sample_pixel_x, int sample_pixel_y);
MagickWand * zoom_out(MagickWand *wand, double scaleFactor, FilterType filter_type, bool make_linear_color_space, bool debug_zooming_out, bool debug_resizing);
MagickBooleanType resize(MagickWand *wand, size_t width, size_t height, FilterType filter_type, bool make_linear_color_space, bool debug_resizing);
void copy_channel(MagickWand * target_wand, MagickWand * source_wand, ChannelType target_channel_type, ChannelType source_channel_type);
MagickBooleanType are_rgb_channels_equal(MagickWand *wand);
MagickBooleanType has_alpha_channel_in_practice(MagickWand *wand);
void draw_UVCorners_on_atlas(MagickWand *wand, UVCorners *uvCorners, int num_images, const char* uv_rect_color);
#endif //ATLAS_IMG_MANIP_H
