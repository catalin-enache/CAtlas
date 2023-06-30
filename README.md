
## Input Output

#### Working (tested)
 - input images as: jpg png tiff exr
 - output as: jpg png tiff

#### Not working
 - output as: exr



## Atlas

#### Default config
shrink 0.92         
cols 2               
png_compression 6    
jpg_quality 100      
atlas_output_format png  
vips_interpretation 22  


shrink              => [0.35 - 1.0]  
cols                => [1 - ...]
png_compression     => [0 - 9] => 0 is no compression (quality is not affected by compression)  
jpg_quality         => [1 - 100] => 100 is best quality  
atlas_output        => [png jpg tiff]  
vips_interpretation => VipsInterpretation

## Notes

Converting HDR images (e.g. exr) to LDR images (e.g. jpg png)
should preferably be avoided because currently there is no
algorithm implemented to convert HDR to LDR.

#### Vips interpretation

typedef enum {
VIPS_INTERPRETATION_ERROR = -1,
VIPS_INTERPRETATION_MULTIBAND = 0,
VIPS_INTERPRETATION_B_W = 1,
VIPS_INTERPRETATION_HISTOGRAM = 10,
VIPS_INTERPRETATION_XYZ = 12,
VIPS_INTERPRETATION_LAB = 13,
VIPS_INTERPRETATION_CMYK = 15,
VIPS_INTERPRETATION_LABQ = 16,
VIPS_INTERPRETATION_RGB = 17,
VIPS_INTERPRETATION_CMC = 18,
VIPS_INTERPRETATION_LCH = 19,
VIPS_INTERPRETATION_LABS = 21,
VIPS_INTERPRETATION_sRGB = 22,
VIPS_INTERPRETATION_YXY = 23,
VIPS_INTERPRETATION_FOURIER = 24,
VIPS_INTERPRETATION_RGB16 = 25,
VIPS_INTERPRETATION_GREY16 = 26,
VIPS_INTERPRETATION_MATRIX = 27,
VIPS_INTERPRETATION_scRGB = 28,
VIPS_INTERPRETATION_HSV = 29,
VIPS_INTERPRETATION_LAST = 30
} VipsInterpretation;