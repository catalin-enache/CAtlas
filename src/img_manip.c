#include "img_manip.h"

PixelChannel PIXEL_CHANNELS[] = {RedPixelChannel, GreenPixelChannel, BluePixelChannel, BlackPixelChannel, AlphaPixelChannel};
int PIXEL_CHANNELS_NUM = sizeof(PIXEL_CHANNELS) / sizeof(PIXEL_CHANNELS[0]);

char* colorSpaceMapping(ColorspaceType colorspace_type) {
    int str_size = 50;
    char *str = malloc(str_size * sizeof(char));
    snprintf(str, sizeof(str), "UndefinedColorspace(%d)", colorspace_type);

    switch (colorspace_type) {
        case UndefinedColorspace:
            snprintf(str, str_size, "UndefinedColorspace(%d)", colorspace_type); break;
        case RGBColorspace:
            snprintf(str, str_size, "RGBColorspace(%d)", colorspace_type); break;
        case sRGBColorspace:
            snprintf(str, str_size, "sRGBColorspace(%d)", colorspace_type); break;
        case GRAYColorspace:
            snprintf(str, str_size, "GRAYColorspace(%d)", colorspace_type); break;
        case LinearGRAYColorspace:
            snprintf(str, str_size, "LinearGRAYColorspace(%d)", colorspace_type); break;
        case CMYColorspace:
            snprintf(str, str_size, "CMYColorspace(%d)", colorspace_type); break;
        case CMYKColorspace:
            snprintf(str, str_size, "CMYKColorspace(%d)", colorspace_type); break;
        default:
            snprintf(str, str_size, "Other(%d)", colorspace_type);
    }

    return str;
}

// map ImageType to string
char* imageTypeMapping(ImageType image_type) {
    int str_size = 50;
    char *str = malloc(str_size * sizeof(char));

    switch (image_type) {
        case UndefinedType:
            snprintf(str, str_size, "UndefinedType(%d)", image_type); break;
        case GrayscaleType:
            snprintf(str, str_size, "GrayscaleType(%d)", image_type); break;
        case GrayscaleAlphaType:
            snprintf(str, str_size, "GrayscaleAlphaType(%d)", image_type); break;
        case TrueColorType:
            snprintf(str, str_size, "TrueColorType(%d)", image_type); break;
        case TrueColorAlphaType:
            snprintf(str, str_size, "TrueColorAlphaType(%d)", image_type); break;
        case ColorSeparationType:
            snprintf(str, str_size, "ColorSeparationType(%d)", image_type); break;
        case ColorSeparationAlphaType:
            snprintf(str, str_size, "ColorSeparationAlphaType(%d)", image_type); break;
        case PaletteType:
            snprintf(str, str_size, "PaletteType(%d)", image_type); break;
        case PaletteAlphaType:
            snprintf(str, str_size, "PaletteAlphaType(%d)", image_type); break;
        case BilevelType:
            snprintf(str, str_size, "BilevelType(%d)", image_type); break;
        case PaletteBilevelAlphaType:
            snprintf(str, str_size, "PaletteBilevelAlphaType(%d)", image_type); break;
        case OptimizeType:
            snprintf(str, str_size, "OptimizeType(%d)", image_type); break;
        default:
            snprintf(str, str_size, "Other(%d)", image_type);
    }

    return str;
}

char* channelTypeMapping(ChannelType channel) {
    int str_size = 50;
    char *str = malloc(str_size * sizeof(char));

    switch(channel) {
        case RedChannel:
            snprintf(str, str_size, "Red/GrayChannel(%d)", channel); break;
        case GreenChannel:
            snprintf(str, str_size, "GreenChannel(%d)", channel); break;
        case BlueChannel:
            snprintf(str, str_size, "BlueChannel(%d)", channel); break;
        case AlphaChannel:
            snprintf(str, str_size, "AlphaChannel(%d)", channel); break;
        case UndefinedChannel:
            snprintf(str, str_size, "UndefinedChannel(%d)", channel); break;
        default:
            snprintf(str, str_size, "OtherChannel(%d)", channel); break;
    }

    return str;
}

char * compositeOperatorMapping(CompositeOperator op) {
    int str_size = 50;
    char *str = malloc(str_size * sizeof(char));
    switch(op) {
        case CopyRedCompositeOp:
            snprintf(str, str_size, "CopyRed/GrayCompositeOp(%d)", op); break;
        case CopyGreenCompositeOp:
            snprintf(str, str_size, "CopyGreenCompositeOp(%d)", op); break;
        case CopyBlueCompositeOp:
            snprintf(str, str_size, "CopyBlueCompositeOp(%d)", op); break;
        case CopyAlphaCompositeOp:
            snprintf(str, str_size, "CopyAlphaCompositeOp(%d)", op); break;
        default:
            snprintf(str, str_size, "Other(%d)", op); break;
    }
    return str;
}

ChannelType getChannelFromPixelChannel(PixelChannel pixelChannel) {
    switch (pixelChannel) {
        case RedPixelChannel:
            return RedChannel;
        case GreenPixelChannel:
            return GreenChannel;
        case BluePixelChannel:
            return BlueChannel;
        case BlackPixelChannel:
            return BlackChannel;
        case AlphaPixelChannel:
            return AlphaChannel;
        default:
            return UndefinedChannel;
    }
}

PixelChannel* get_image_channels(MagickWand *wand, int *size_out) {
    // Internal ImageMagick approach
    Image *image = GetImageFromMagickWand(wand);
    if (image == NULL) {
        printf("Could not GetImageFromMagickWand (get_image_channels).\n");
        return NULL;
    }
    size_t channels_num = GetPixelChannels(image);
    *size_out = channels_num;
    PixelChannel *_pixel_channels = malloc(channels_num * sizeof(PixelChannel));
    for (int j = 0; j < channels_num; ++j) {
        PixelChannel channel = GetPixelChannelChannel(image,j);
        if (channel == IndexPixelChannel) { // cannot be used in stats as index
            *size_out -= 1;
            continue;
        }
        _pixel_channels[j] = channel;
    }
    return _pixel_channels;

    // My heuristic approach
    ChannelStatistics *stats = MagickGetImageStatistics(wand);
    (*size_out) = 0;
    for (int i = 0; i < PIXEL_CHANNELS_NUM; i++) {
//         printf("stats[%d]: %f, %f\n",  PIXEL_CHANNELS[i], stats[PIXEL_CHANNELS[i]].minima, stats[PIXEL_CHANNELS[i]].maxima);
        if (stats[PIXEL_CHANNELS[i]].minima == DBL_MAX || stats[PIXEL_CHANNELS[i]].maxima == DBL_MAX) continue;
        (*size_out) += 1;
    }
    // printf("size_out: %d\n", *size_out);
    PixelChannel *pixel_channels = malloc((*size_out) * sizeof(PixelChannel));
    if (pixel_channels == NULL) {
        printf("Memory allocation failed (get_image_channels).\n");
        return NULL;
    }
    int count = 0;
    for (int i = 0; i < PIXEL_CHANNELS_NUM; i++) {
        if (stats[PIXEL_CHANNELS[i]].minima == MagickMaximumValue || stats[PIXEL_CHANNELS[i]].maxima == MagickMaximumValue) { // DBL_MAX
            continue;
        }
        // printf("pixel_channels[%d] = %d\n", count, PIXEL_CHANNELS[i]);
        pixel_channels[count] = PIXEL_CHANNELS[i];
        count += 1;
    }
    MagickRelinquishMemory(stats);
    return pixel_channels;
}

double** get_min_max_for_each_band(MagickWand *wand) {
    int channels_num = 0;
    PixelChannel* channels = get_image_channels(wand, &channels_num);
    if(channels == NULL) {
        printf("Could not get channels (get_min_max_for_each_band).\n");
        return NULL;
    }
    double **channels_min_max = (double**)malloc(channels_num * sizeof(double*));
    if(channels_min_max == NULL) {
        printf("Memory allocation failed (get_min_max_for_each_band).\n");
        return NULL;
    }

    ChannelStatistics *stats = MagickGetImageStatistics(wand);

    for (int i = 0; i < channels_num; i++) {
        channels_min_max[i] = (double*)malloc(2 * sizeof(double));
        if(channels_min_max[i] == NULL) {
            printf("Memory allocation failed (get_min_max_for_each_band).\n");
            return NULL;
        }
        channels_min_max[i][0] = stats[channels[i]].minima;
        channels_min_max[i][1] = stats[channels[i]].maxima;
    }
    return channels_min_max;
}

char* print_min_max_for_each_band(MagickWand *wand) {
    int channels_num = 0;
    PixelChannel *channels = get_image_channels(wand, &channels_num);
    if (channels == NULL) {
        printf("Could not get channels (print_min_max_for_each_band).\n");
        return NULL;
    }
    double **bands_min_max = get_min_max_for_each_band(wand);

    if (bands_min_max == NULL) {
        printf("Could not get min max for each band (print_min_max_for_each_band_m).\n");
        return NULL;
    }
    char *res = print_2d_array_of_double(bands_min_max, channels_num, 2);
    if (res == NULL) {
        printf("Could not print min max for each band.\n");
        return NULL;
    }
    for (int i = 0; i < channels_num; i++) {
        free(bands_min_max[i]);
    }
    free(bands_min_max);
    return res;
}


void print_pixel(MagickWand *wand, int x, int y) {
    PixelWand *pixelWand = NewPixelWand();
    if (MagickGetImagePixelColor(wand, x, y, pixelWand) == MagickFalse) {
        printf("Could not MagickGetImagePixelColor (print_pixel).\n");
        return;
    }
    int channels_num = 0;
    PixelChannel* channels = get_image_channels(wand, &channels_num);
    if (channels == NULL) {
        printf("Could not get channels (print_pixel).\n");
        return;
    }

    if (channels_num == 1) {
        printf("Gray: [%.6f]\n", PixelGetRed(pixelWand));
    } else if (channels_num == 2) {
        printf("Gray/A: [%.6f, %.6f]\n", PixelGetRed(pixelWand), PixelGetAlpha(pixelWand));
    } else if (channels_num == 3) {
        printf("RGB: [%.6f, %.6f, %.6f]\n", PixelGetRed(pixelWand), PixelGetGreen(pixelWand), PixelGetBlue(pixelWand));
    } else if (channels_num == 4) {
        if (channels[3] == AlphaPixelChannel) {
            printf("RGBA: [%.6f, %.6f, %.6f, %.6f]\n", PixelGetRed(pixelWand), PixelGetGreen(pixelWand), PixelGetBlue(pixelWand), PixelGetAlpha(pixelWand));
        } else {
            printf("CMYK: [%.6f, %.6f, %.6f, %.6f]\n", PixelGetRed(pixelWand), PixelGetGreen(pixelWand), PixelGetBlue(pixelWand), PixelGetBlack(pixelWand));
        }
    } else if (channels_num == 5) {
        printf("CMYKA: [%.6f, %.6f, %.6f, %.6f, %.6f]\n", PixelGetRed(pixelWand), PixelGetGreen(pixelWand), PixelGetBlue(pixelWand), PixelGetBlack(pixelWand), PixelGetAlpha(pixelWand));
    } else {
        printf("The number of channels %d is not standard.\n", channels_num);
    }
    // Clean up
    if(pixelWand) DestroyPixelWand(pixelWand);
}

void print_info(MagickWand *wand, int sample_pixel_x, int sample_pixel_y) {
    char *info = print_min_max_for_each_band(wand);
    if (info == NULL) {
        printf("Could not print_min_max_for_each_band (print_info).\n");
        return;
    }
    char* imageColorspace = colorSpaceMapping(MagickGetImageColorspace(wand));
    char* colorSpace = colorSpaceMapping(MagickGetColorspace(wand));
    char* imageType = imageTypeMapping(MagickGetImageType(wand));
    char* type = imageTypeMapping(MagickGetType(wand));

    printf("\t%s\n", info);
    printf("\t");
    print_pixel(wand, sample_pixel_x, sample_pixel_y);
    printf("\tMagickGetImageFormat: %s\n", MagickGetImageFormat(wand));
    printf("\tMagickGetImageDepth: %zu\n", MagickGetImageDepth(wand));
    printf("\tMagickGetImageColorspace: %s\n", imageColorspace);
    printf("\tMagickGetColorspace: %s\n", colorSpace);
    printf("\tMagickGetImageType: %s\n", imageType);
    printf("\tMagickGetType: %s\n", type);

    free(imageColorspace);
    free(colorSpace);
    free(imageType);
    free(type);

    int channels_num = 0;
    PixelChannel *pixelChannels = get_image_channels(wand, &channels_num);
    printf("\tPixelChannel|ChannelType: %d channels => ", channels_num);
    for (int j = 0; j < channels_num; j++) printf("channel[%d]: %d|%d ", j, pixelChannels[j], getChannelFromPixelChannel(pixelChannels[j]));
    printf("\n");

    size_t input_profile_length;
    unsigned char *input_profile = MagickGetImageProfile(wand, "ICC", &input_profile_length); // ICC, IPTC
    printf("\tprofile length: %zu\n", input_profile_length);
    // print profile chunk
    if (input_profile_length > 0) {
        printf("\tprofile chunk: ");
        for(int i = 0; i < input_profile_length && i < 10; i++) printf("%02x ", input_profile[i]);
        printf("... \n");
    }
}

void transform_color_space_and_set_image_depth(MagickWand *wand, ColorspaceType colorspace, size_t depth) {
    MagickSetDepth(wand, depth);
    MagickSetImageDepth(wand, depth);
    MagickTransformImageColorspace(wand, colorspace);
    MagickSetColorspace(wand, colorspace);
    MagickSetImageColorspace(wand, colorspace);
}

MagickWand * zoom_out(MagickWand *wand, double scaleFactor, FilterType filter_type) {
    size_t original_depth = MagickGetImageDepth(wand);
    ColorspaceType original_colorspace = MagickGetImageColorspace(wand);
    ColorspaceType temp_colorspace;

    if (original_colorspace == sRGBColorspace) {
        printf("Setting colorspace to RGBColorspace during zoom_out\n");
        temp_colorspace = RGBColorspace;
    } else if (original_colorspace == GRAYColorspace) {
        printf("Setting colorspace to LinearGRAYColorspace during zoom_out\n");
        temp_colorspace = LinearGRAYColorspace;
    } else {
        temp_colorspace = original_colorspace;
    }

    size_t originalWidth = MagickGetImageWidth(wand);
    size_t originalHeight = MagickGetImageHeight(wand);

    MagickWand *resizedWand = CloneMagickWand(wand);
    transform_color_space_and_set_image_depth(resizedWand, temp_colorspace, 32);

    MagickResizeImage(resizedWand, originalWidth * scaleFactor, originalHeight * scaleFactor, filter_type);
    size_t tileWidth = MagickGetImageWidth(resizedWand);
    size_t tileHeight = MagickGetImageHeight(resizedWand);

    MagickWand *rowWand = NewMagickWand();
    for (int col = 0; col < 3; col++) {
        MagickAddImage(rowWand, resizedWand);
    }
    DestroyMagickWand(resizedWand);
    MagickResetIterator(rowWand);
    MagickWand *rowWandAppended = MagickAppendImages(rowWand, MagickFalse); // false => horizontally, true => vertically
    DestroyMagickWand(rowWand);

    MagickWand *rowsWand = NewMagickWand();
    for (int row = 0; row < 3; row++) {
        MagickAddImage(rowsWand, rowWandAppended);
    }
    DestroyMagickWand(rowWandAppended);
    MagickResetIterator(rowsWand);
    MagickWand *outputWand = MagickAppendImages(rowsWand, MagickTrue); // false => horizontally, true => vertically
    DestroyMagickWand(rowsWand);

    int offsetX = (tileWidth * 3 - originalWidth) / 2;
    int offsetY = (tileHeight *3 - originalHeight) / 2;
    MagickCropImage(outputWand, originalWidth, originalHeight, offsetX, offsetY);
    transform_color_space_and_set_image_depth(outputWand, original_colorspace, original_depth);
//    print_info(outputWand, 0, 0);

    return outputWand;
}

void (*pixel_set_operation)(PixelWand *, const double);

void copy_channel(MagickWand * target_wand, MagickWand * source_wand, ChannelType target_channel_type, ChannelType source_channel_type) {
    PixelIterator *target_iterator = NewPixelIterator(target_wand);
    PixelIterator *source_iterator = NewPixelIterator(source_wand);

    size_t target_width = MagickGetImageWidth(target_wand);
    size_t target_height = MagickGetImageHeight(target_wand);

    for (ssize_t y = 0; y < target_height; y++) {
        // Get the row of pixels from target and source
        // target_width gets updated to the actual row width. Probably it doesn't change, but just in case
        PixelWand **target_pixels = PixelGetNextIteratorRow(target_iterator, &target_width);
        PixelWand **source_pixels = PixelGetNextIteratorRow(source_iterator, &target_width);
        for (ssize_t x = 0; x < target_width; x++) {
            // Extract the alpha value from the source pixel
            double value = source_channel_type == RedChannel ? PixelGetRed(source_pixels[x]) :
                           source_channel_type == GreenChannel ? PixelGetGreen(source_pixels[x]) :
                           source_channel_type == BlueChannel ? PixelGetBlue(source_pixels[x]) :
                           source_channel_type == AlphaChannel ? PixelGetAlpha(source_pixels[x]) :
                           PixelGetBlack(source_pixels[x]);

            pixel_set_operation = target_channel_type == RedChannel ? PixelSetRed :
                                  target_channel_type == GreenChannel ? PixelSetGreen :
                                  target_channel_type == BlueChannel ? PixelSetBlue :
                                  target_channel_type == AlphaChannel ? PixelSetAlpha :
                                  PixelSetBlack;

            pixel_set_operation(target_pixels[x], value);
        }
        // apply changes
        PixelSyncIterator(target_iterator);
    }

    DestroyPixelIterator(target_iterator);
    DestroyPixelIterator(source_iterator);
}