#include "img_manip.h"

PixelChannel PIXEL_CHANNELS[] = {RedPixelChannel, GreenPixelChannel, BluePixelChannel, BlackPixelChannel, AlphaPixelChannel};
int PIXEL_CHANNELS_NUM = sizeof(PIXEL_CHANNELS) / sizeof(PIXEL_CHANNELS[0]);

char* colorSpaceAsString(ColorspaceType colorspace_type) {
    int str_size = 50;
    char *str = (char *)malloc(str_size * sizeof(char));
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
char* imageTypeAsString(ImageType image_type) {
    int str_size = 50;
    char *str = (char *)malloc(str_size * sizeof(char));

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

char* channelTypeAsString(ChannelType channel) {
    int str_size = 50;
    char *str = (char *)malloc(str_size * sizeof(char));

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

char * compositeOperatorAsString(CompositeOperator op) {
    int str_size = 50;
    char *str = (char *)malloc(str_size * sizeof(char));
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

char * tifSampleFormatAsString(uint16_t sampleFormat) {
    int str_size = 60;
    char *str = (char *)malloc(str_size * sizeof(char));

    switch (sampleFormat) {
        case SAMPLEFORMAT_UINT:
            snprintf(str, str_size, "Unsigned integer data"); break;
        case SAMPLEFORMAT_INT:
            snprintf(str, str_size, "Signed integer data"); break;
        case SAMPLEFORMAT_IEEEFP:
            snprintf(str, str_size, "IEEE floating point data"); break;
        case SAMPLEFORMAT_VOID:
            snprintf(str, str_size, "Undefined data format"); break;
        case SAMPLEFORMAT_COMPLEXINT:
            snprintf(str, str_size, "Complex signed integer"); break;
        case SAMPLEFORMAT_COMPLEXIEEEFP:
            snprintf(str, str_size, "Complex IEEE floating"); break;
        default:
            snprintf(str, str_size, "Unknown"); break;
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
    PixelChannel *_pixel_channels = (PixelChannel *)malloc(channels_num * sizeof(PixelChannel));
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
    // ChannelStatistics *stats = MagickGetImageStatistics(wand);
    // (*size_out) = 0;
    // for (int i = 0; i < PIXEL_CHANNELS_NUM; i++) {
    //     // printf("stats[%d]: %f, %f\n",  PIXEL_CHANNELS[i], stats[PIXEL_CHANNELS[i]].minima, stats[PIXEL_CHANNELS[i]].maxima);
    //     if (stats[PIXEL_CHANNELS[i]].minima == DBL_MAX || stats[PIXEL_CHANNELS[i]].maxima == DBL_MAX) continue;
    //     (*size_out) += 1;
    // }
    // // printf("size_out: %d\n", *size_out);
    // PixelChannel *pixel_channels = malloc((*size_out) * sizeof(PixelChannel));
    // if (pixel_channels == NULL) {
    //     printf("Memory allocation failed (get_image_channels).\n");
    //     return NULL;
    // }
    // int count = 0;
    // for (int i = 0; i < PIXEL_CHANNELS_NUM; i++) {
    //     if (stats[PIXEL_CHANNELS[i]].minima == MagickMaximumValue || stats[PIXEL_CHANNELS[i]].maxima == MagickMaximumValue) { // DBL_MAX
    //         continue;
    //     }
    //     // printf("pixel_channels[%d] = %d\n", count, PIXEL_CHANNELS[i]);
    //     pixel_channels[count] = PIXEL_CHANNELS[i];
    //     count += 1;
    // }
    // MagickRelinquishMemory(stats);
    // return pixel_channels;
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

void (*pixel_set_operation)(PixelWand *, const double);

double** get_min_max_for_each_band_2(MagickWand *wand) {
    size_t width = MagickGetImageWidth(wand);
    size_t height = MagickGetImageHeight(wand);

    int channels_num = 0;
    PixelChannel* channels = get_image_channels(wand, &channels_num);
    if (channels == NULL) {
        printf("Could not get channels (print_pixel).\n");
        return NULL;
    }

    double **channels_min_max = (double**)malloc(channels_num * sizeof(double*));
    if(channels_min_max == NULL) {
        printf("Could not allocate memory for channels_min_max).\n");
        return NULL;
    }

    for (int i = 0; i < channels_num; i++) {
        channels_min_max[i] = (double*)malloc(2 * sizeof(double));
        channels_min_max[i][0] = DBL_MAX;
        channels_min_max[i][1] = DBL_MIN;
    }

    PixelIterator *iterator = NewPixelIterator(wand);

    for (ssize_t y = 0; y < height; y++) {
        // Get the row of pixels from wand
        // width gets updated to the actual row width. Probably it doesn't change, but just in case
        PixelWand **pixels = PixelGetNextIteratorRow(iterator, &width);
        for (ssize_t x = 0; x < width; x++) {
            for (int i = 0; i < channels_num; i++) {
                double value = channels[i] == RedPixelChannel ? PixelGetRed(pixels[x]) :
                               channels[i] == GreenPixelChannel ? PixelGetGreen(pixels[x]) :
                               channels[i] == BluePixelChannel ? PixelGetBlue(pixels[x]) :
                               channels[i] == AlphaPixelChannel ? PixelGetAlpha(pixels[x]) :
                               PixelGetBlack(pixels[x]);
                if (value < channels_min_max[i][0]) channels_min_max[i][0] = value;
                if (value > channels_min_max[i][1]) channels_min_max[i][1] = value;
                // prove that we iterated correctly by painting pixels
                // pixel_set_operation = channels[i] == RedChannel ? PixelSetRed :
                //                       channels[i] == GreenChannel ? PixelSetGreen :
                //                       channels[i] == BlueChannel ? PixelSetBlue :
                //                       channels[i] == AlphaChannel ? PixelSetAlpha :
                //                       PixelSetBlack;
                //
                // pixel_set_operation(pixels[x], (double)x/(double)width);
            }
        }
        // apply changes
        // PixelSyncIterator(iterator);
    }

    DestroyPixelIterator(iterator);
    return channels_min_max;
}

uint16_t get_tiff_sample_format(const char *image_file_name) {
    uint16_t sampleFormat = 0;
    TIFF* tif = NULL; tif = TIFFOpen(image_file_name, "r");
    if (!tif) { printf("Failed to open TIFF file.\n"); return sampleFormat; }
    TIFFGetField(tif, TIFFTAG_SAMPLEFORMAT, &sampleFormat);
    TIFFClose(tif);
    return sampleFormat;
}

char* print_min_max_for_each_band(MagickWand *wand) {
    int channels_num = 0;
    PixelChannel *channels = get_image_channels(wand, &channels_num);
    if (channels == NULL) {
        printf("Could not get channels (print_min_max_for_each_band).\n");
        return NULL;
    }
    double **bands_min_max = get_min_max_for_each_band(wand);
    double **bands_min_max_2 = get_min_max_for_each_band_2(wand);

    if (bands_min_max == NULL) {
        printf("Could not get min max for each band (print_min_max_for_each_band_m).\n");
        return NULL;
    }
    char *res = (char *)malloc(2048 * sizeof(char));
    res[0] = '\0';
    int bands_min_max_dimensions[] = {channels_num, 2};
    arrayToString(res, bands_min_max, bands_min_max_dimensions, 2, 0, "double", sizeof(double), "%.8f", 64);
    strcat(res, "\n");
    arrayToString(res, bands_min_max_2, bands_min_max_dimensions, 2, 0, "double", sizeof(double), "%.8f", 64);
    if (res == NULL) {
        printf("Could not print min max for each band.\n");
        return NULL;
    }
    for (int i = 0; i < channels_num; i++) {
        free(bands_min_max[i]);
        free(bands_min_max_2[i]);
    }
    free(bands_min_max);
    free(bands_min_max_2);

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
    char * image_file_name = MagickGetImageFilename(wand);
    char *info = print_min_max_for_each_band(wand);
    replace_first_substring(&info, "\n", "\n\t");
    if (info == NULL) {
        printf("Could not print_min_max_for_each_band (print_info).\n");
        return;
    }
    Image *image = GetImageFromMagickWand(wand);
    ExceptionInfo *exception = AcquireExceptionInfo();
    const char *quantum_format = GetImageProperty(image, "quantum:format", exception);

    // char **property_list;
    // size_t number_properties;
    // property_list = MagickGetImageProperties(wand, "tiff:*", &number_properties);
    // for (int i = 0; i < number_properties; i++) {
    //     printf("property_list[%d]: %s\n", i, property_list[i]);
    // }

    char *sampleFormatAsString = NULL;
    if (
            strcmp(MagickGetImageFormat(wand), "TIFF") == 0 ||
            strcmp(MagickGetImageFormat(wand), "TIF") == 0 &&
            ends_with_extension(image_file_name, "tif") &&
            ends_with_extension(image_file_name, "tiff") &&
            file_exists(image_file_name)
        ) {
        uint16_t sampleFormat = get_tiff_sample_format(image_file_name);
        if (sampleFormat) {
            sampleFormatAsString = tifSampleFormatAsString(sampleFormat);
        }
    }

    char* imageColorspace = colorSpaceAsString(MagickGetImageColorspace(wand));
    char* colorSpace = colorSpaceAsString(MagickGetColorspace(wand));
    char* imageType = imageTypeAsString(MagickGetImageType(wand));
    char* type = imageTypeAsString(MagickGetType(wand));
    size_t width = MagickGetImageWidth(wand);
    size_t height = MagickGetImageHeight(wand);

    printf("Image info for %s:\n", image_file_name);
    printf("\t%s\n", info);
    printf("\t");
    printf("Sample pixel (%d, %d): ", sample_pixel_x, sample_pixel_y);
    print_pixel(wand, sample_pixel_x, sample_pixel_y);
    printf("\tquantum:format: %s\n", quantum_format);
    if (sampleFormatAsString) {
        printf("\tTiffTagSampleFormat: %s\n", sampleFormatAsString);
        free(sampleFormatAsString);
    }
    printf("\tMagickGetImageWidth: %zu\n", width);
    printf("\tMagickGetImageHeight: %zu\n", height);
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

    char profiles[][8] = {"ICC", "IPTC", "XMP"};
    for (int p = 0; p < 3; p++) {
        size_t profile_length;
        unsigned char *profile = MagickGetImageProfile(wand, profiles[p], &profile_length); // ICC, IPTC, XMP
        printf("\t%s profile length: %zu\n", profiles[p], profile_length);
        // print profile chunk
        if (profile_length > 0) {
            printf("\t%s profile chunk: ", profiles[p]);
            for(int i = 0; i < profile_length && i < 10; i++) printf("%02x ", profile[i]);
            printf("... \n");
        }
    }
}

void transform_color_space_and_set_image_depth(MagickWand *wand, ColorspaceType colorspace, size_t depth) {
    // srgb to linear rgb formula:
    // <C-srgb> means each RGB channel is transformed separately
    // <C-srgb>/12.92 if <C-srgb> <= 0.04045
    // ((<C-srgb> + 0.055)/1.055)**2.4 if <C-srgb> > 0.04045
    MagickSetDepth(wand, depth);
    MagickSetImageDepth(wand, depth);
    MagickTransformImageColorspace(wand, colorspace);
    MagickSetColorspace(wand, colorspace);
    MagickSetImageColorspace(wand, colorspace);
}

ColorspaceType get_linear_color_space(ColorspaceType colorspace) {
    switch (colorspace) {
        case sRGBColorspace:
            return RGBColorspace;
        case GRAYColorspace:
            return LinearGRAYColorspace;
        default:
            return colorspace;
    }
}

MagickBooleanType resize(MagickWand *wand, size_t width, size_t height, FilterType filter_type, bool make_linear_color_space, bool debug_resizing) {
    printf("Resizing %s to %zux%zu\n\n", MagickGetImageFilename(wand), width, height);

    if (debug_resizing) print_info(wand, 0, 0);

    size_t original_depth = MagickGetImageDepth(wand);
    ColorspaceType original_colorspace = MagickGetImageColorspace(wand);
    ImageType original_image_type = MagickGetImageType(wand);
    ColorspaceType temp_colorspace = get_linear_color_space(original_colorspace);

    MagickBooleanType hasAlpha = MagickGetImageAlphaChannel(wand);
    MagickWand *alphaWand = CloneMagickWand(wand);
    // splitting alpha channel from the rest in order to not affect the other channels when resizing and alpha is 0
    // (alpha gets premultiplied into other channels setting them to 0 where alpha is 0)
    // resizing them separately then combining them back
    hasAlpha && MagickSeparateImage(alphaWand, AlphaChannel); // alphaWand now contains just alpha channel
    hasAlpha && MagickSetImageAlphaChannel(wand, OffAlphaChannel); // wand now contains just the other channels
    if (make_linear_color_space && temp_colorspace != original_colorspace) {
        printf("Setting colorspace from %s to %s only for non alpha channels during resize\n", colorSpaceAsString(original_colorspace), colorSpaceAsString(temp_colorspace));
        transform_color_space_and_set_image_depth(wand, temp_colorspace, 32);
        // we don't convert alpha channel to linear color space because alpha channel is already in linear space
    }

    if (debug_resizing) printf("After setting linear color space\n");
    if (debug_resizing) print_info(wand, 0, 0);
    if (debug_resizing && hasAlpha) printf("After setting linear color space (Alpha channel)\n");
    if (debug_resizing && hasAlpha) print_info(alphaWand, 0, 0);

    MagickResizeImage(wand, width, height, filter_type);
    hasAlpha && MagickResizeImage(alphaWand, width, height, filter_type);
    if(make_linear_color_space && temp_colorspace != original_colorspace) {
        printf("Reverting colorspace from %s to %s only for non alpha channels after resize\n", colorSpaceAsString(temp_colorspace), colorSpaceAsString(original_colorspace));
        transform_color_space_and_set_image_depth(wand, original_colorspace, original_depth);
    }
    hasAlpha && MagickCompositeImage(wand, alphaWand, CopyAlphaCompositeOp, (MagickBooleanType)1, 0, 0);
    // hasAlpha && MagickSetImageAlphaChannel(wand, OnAlphaChannel);
    // if (hasAlpha) copy_channel(wand, alphaWand, AlphaChannel, GrayChannel);
    MagickSetImageType(wand, original_image_type);

    if (debug_resizing) printf("After resize and re-compositing with alpha channel if existed\n");
    if (debug_resizing) print_info(wand, 0, 0);

    DestroyMagickWand(alphaWand);
    printf("\n");
    return MagickTrue;
}

MagickWand * zoom_out(MagickWand *wand, double scaleFactor, FilterType filter_type, bool make_linear_color_space, bool debug_zooming_out, bool debug_resizing) {

    size_t originalWidth = MagickGetImageWidth(wand);
    size_t originalHeight = MagickGetImageHeight(wand);
    MagickWand *resizedWand = CloneMagickWand(wand);
    printf("Zooming out %s having size %zux%zu by applying scaling factor %f\n", MagickGetImageFilename(wand), originalWidth, originalHeight, scaleFactor);

    resize(resizedWand, originalWidth * scaleFactor, originalHeight * scaleFactor, filter_type, make_linear_color_space, debug_resizing);

    size_t tileWidth = MagickGetImageWidth(resizedWand);
    size_t tileHeight = MagickGetImageHeight(resizedWand);

    int repeat = 3; // scaleFactor is assumed to be >= 0.5

    MagickWand *rowWand = NewMagickWand();
    for (int col = 0; col < repeat; col++) {
        MagickAddImage(rowWand, resizedWand);
    }

    DestroyMagickWand(resizedWand);
    MagickResetIterator(rowWand);
    MagickWand *rowWandAppended = MagickAppendImages(rowWand, MagickFalse); // false => horizontally, true => vertically
    DestroyMagickWand(rowWand);

    MagickWand *rowsWand = NewMagickWand();
    for (int row = 0; row < repeat; row++) {
        MagickAddImage(rowsWand, rowWandAppended);
    }

    DestroyMagickWand(rowWandAppended);
    MagickResetIterator(rowsWand);
    MagickWand *outputWand = MagickAppendImages(rowsWand, MagickTrue); // false => horizontally, true => vertically
    DestroyMagickWand(rowsWand);

    int offsetX = (tileWidth * repeat - originalWidth) / 2;
    int offsetY = (tileHeight * repeat - originalHeight) / 2;
    MagickCropImage(outputWand, originalWidth, originalHeight, offsetX, offsetY);
    MagickResetImagePage(outputWand, "0x0+0+0"); // Fix tiff images reporting a bigger size than it should be after cropping.

    if (debug_zooming_out) printf("After zoom out\n");
    if (debug_zooming_out) print_info(outputWand, 0, 0);

    return outputWand;
}



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

void draw_UVCorners_on_atlas(MagickWand *wand, UVCorners *uvCorners, int num_images, const char* uv_rect_color) {
    size_t atlas_width = MagickGetImageWidth(wand);
    size_t atlas_height = MagickGetImageHeight(wand);

    DrawingWand *drawing_wand = NewDrawingWand();

    // Set the fill color
    PixelWand *fill_color = NewPixelWand();
    PixelSetColor(fill_color, uv_rect_color);
    DrawSetFillColor(drawing_wand, fill_color);

    PixelWand *stroke_color = NewPixelWand();
    PixelSetColor(stroke_color, "black");
    // DrawSetStrokeColor(drawing_wand, stroke_color);
    // DrawSetStrokeWidth(drawing_wand, 2);

    for (int i = 0; i < num_images; i++) {
        UVCorners corners = uvCorners[i];
        double top_left_x = corners.topLeft[0] * atlas_width;
        double top_left_y = corners.topLeft[1] * atlas_height;
        double bottom_left_x = corners.bottomLeft[0] * atlas_width;
        double bottom_left_y = corners.bottomLeft[1] * atlas_height;
        double top_right_x = corners.topRight[0] * atlas_width;
        double top_right_y = corners.topRight[1] * atlas_height;
        double bottom_right_x = corners.bottomRight[0] * atlas_width;
        double bottom_right_y = corners.bottomRight[1] * atlas_height;
        size_t rect_width = top_right_x - top_left_x;
        size_t rect_height = bottom_left_y - top_left_y;
        DrawRectangle(drawing_wand, top_left_x, top_left_y, top_left_x + rect_width, top_left_y + rect_height);
    }

    // Render the drawing onto the image
    MagickDrawImage(wand, drawing_wand);

    DestroyDrawingWand(drawing_wand);
    DestroyPixelWand(fill_color);
    DestroyPixelWand(stroke_color);
}