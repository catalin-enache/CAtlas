#ifndef ATLAS_IMG_MANIP_H
#define ATLAS_IMG_MANIP_H

//#define MAGICKCORE_QUANTUM_DEPTH 32
//#define MAGICKCORE_HDRI_ENABLE 1
#include <ImageMagick-7/MagickWand/MagickWand.h>
#include "utils.h"
#include "calculations.h"

size_t get_image_depth(MagickWand *wand);
ChannelType getChannelFromPixelChannel(PixelChannel pixelChannel);
PixelChannel* get_image_channels(MagickWand *wand, int *size_out);
double** get_min_max_for_each_band(MagickWand *wand);
char* print_min_max_for_each_band(MagickWand *wand);
char* colorSpaceAsString(ColorspaceType colorspace_type);
char* imageTypeAsString(ImageType image_type);
char* channelTypeAsString(ChannelType channel);
char * compositeOperatorAsString(CompositeOperator op);
void print_pixel(MagickWand *wand, int x, int y);
void print_info(MagickWand *wand, int sample_pixel_x, int sample_pixel_y);
MagickWand * zoom_out(MagickWand *wand, double scaleFactor, FilterType filter_type);
void copy_channel(MagickWand * target_wand, MagickWand * source_wand, ChannelType target_channel_type, ChannelType source_channel_type);
void draw_UVCorners_on_atlas(MagickWand *wand, UVCorners *uvCorners, int num_images, const char* uv_rect_color);
#endif //ATLAS_IMG_MANIP_H
