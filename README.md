
## Input Output

#### Working (tested)
 - input images as: jpg png tiff exr
 - output as: jpg png tiff exr


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


#### ImageMagick ColorspaceType
```C
typedef enum
{
UndefinedColorspace,     /* 0 */
CMYColorspace,           /* 1 negated linear RGB colorspace */
CMYKColorspace,          /* 2 CMY with Black separation */
GRAYColorspace,          /* 3 Single Channel greyscale (non-linear) image */
HCLColorspace,           /* 4 */
HCLpColorspace,          /* 5 */
HSBColorspace,           /* 6 */
HSIColorspace,           /* 7 */
HSLColorspace,           /* 8 */
HSVColorspace,           /* 9 alias for HSB */
HWBColorspace,           /* 10 */
LabColorspace,           /* 11 */
LCHColorspace,           /* 12 alias for LCHuv */
LCHabColorspace,         /* 13 Cylindrical (Polar) Lab */
LCHuvColorspace,         /* 14 Cylindrical (Polar) Luv */
LogColorspace,           /* 15 */
LMSColorspace,           /* 16 */
LuvColorspace,           /* 17 */
OHTAColorspace,          /* 18 */
Rec601YCbCrColorspace,   /* 19 */
Rec709YCbCrColorspace,   /* 20 */
RGBColorspace,           /* 21 Linear RGB colorspace */
scRGBColorspace,         /* 22 ??? */
sRGBColorspace,          /* 23 Default: non-linear sRGB colorspace */
TransparentColorspace,   /* 24 */
xyYColorspace,           /* 25 */
XYZColorspace,           /* 26 IEEE Color Reference colorspace */
YCbCrColorspace,         /* 27 */
YCCColorspace,           /* 28 */
YDbDrColorspace,         /* 29 */
YIQColorspace,           /* 30 */
YPbPrColorspace,         /* 31 */
YUVColorspace,           /* 32 */
LinearGRAYColorspace,    /* 33 Single Channel greyscale (linear) image */
JzazbzColorspace,        /* 34 */
DisplayP3Colorspace,     /* 35 */
Adobe98Colorspace,       /* 36 */
ProPhotoColorspace,      /* 37 */
OklabColorspace,         /* 38 */
OklchColorspace          /* 39 */
} ColorspaceType;
```

#### ImageMagick ImageType
```C
typedef enum
{
UndefinedType,               /* 0 */
BilevelType,                 /* 1 */
GrayscaleType,               /* 2 */
GrayscaleAlphaType,          /* 3 */
PaletteType,                 /* 4 */
PaletteAlphaType,            /* 5 */
TrueColorType,               /* 6 */
TrueColorAlphaType,          /* 7 */
ColorSeparationType,         /* 8 */
ColorSeparationAlphaType,    /* 9 */
OptimizeType,                /* 10 */
PaletteBilevelAlphaType      /* 11 */
} ImageType;
```

#### ImageMagick ChannelType
```C
typedef enum
{
UndefinedChannel = 0x0000,
RedChannel = 0x0001,
GrayChannel = 0x0001,
CyanChannel = 0x0001,
LChannel = 0x0001,
GreenChannel = 0x0002,
MagentaChannel = 0x0002,
aChannel = 0x0002,
BlueChannel = 0x0004,
bChannel = 0x0002,
YellowChannel = 0x0004,
BlackChannel = 0x0008,
AlphaChannel = 0x0010,
OpacityChannel = 0x0010,
IndexChannel = 0x0020,             /* Color Index Table? */
ReadMaskChannel = 0x0040,          /* Pixel is Not Readable? */
WriteMaskChannel = 0x0080,         /* Pixel is Write Protected? */
MetaChannel = 0x0100,              /* not used */
CompositeMaskChannel = 0x0200,     /* SVG mask */
CompositeChannels = 0x001F,
AllChannels = 0x7ffffff,
/*
Special purpose channel types.
FUTURE: are these needed any more - they are more like hacks
SyncChannels for example is NOT a real channel but a 'flag'
It really says -- "User has not defined channels"
Though it does have extra meaning in the "-auto-level" operator
*/
TrueAlphaChannel = 0x0100, /* extract actual alpha channel from opacity */
RGBChannels = 0x0200,      /* set alpha from grayscale mask in RGB */
GrayChannels = 0x0400,
SyncChannels = 0x20000,    /* channels modified as a single unit */
DefaultChannels = AllChannels
}  ;  /* must correspond to PixelChannel */
```

#### ImageMagick PixelChannel
```C
typedef enum
{
UndefinedPixelChannel = 0,
RedPixelChannel = 0,
CyanPixelChannel = 0,
GrayPixelChannel = 0,
LPixelChannel = 0,
LabelPixelChannel = 0,
YPixelChannel = 0,
aPixelChannel = 1,
GreenPixelChannel = 1,
MagentaPixelChannel = 1,
CbPixelChannel = 1,
bPixelChannel = 2,
BluePixelChannel = 2,
YellowPixelChannel = 2,
CrPixelChannel = 2,
BlackPixelChannel = 3,
AlphaPixelChannel = 4,
IndexPixelChannel = 5,
ReadMaskPixelChannel = 6,
WriteMaskPixelChannel = 7,
MetaPixelChannel = 8, /* deprecated */
CompositeMaskPixelChannel = 9,
MetaPixelChannels = 10,
IntensityPixelChannel = MaxPixelChannels,  /* ???? */
CompositePixelChannel = MaxPixelChannels,  /* ???? */
SyncPixelChannel = MaxPixelChannels+1      /* not a real channel */
} PixelChannel;  /* must correspond to ChannelType */
```

#### ImageMagick CompressionType
```C
typedef enum
{
  UndefinedCompression,   /* 0 */
  B44ACompression,        /* 1 */
  B44Compression,         /* 2 */
  BZipCompression,        /* 3 */
  DXT1Compression,        /* 4 */
  DXT3Compression,        /* 5 */
  DXT5Compression,        /* 6 */
  FaxCompression,         /* 7 */
  Group4Compression,      /* 8 */
  JBIG1Compression,       /* 9 ISO/IEC std 11544 / ITU-T rec T.82 */
  JBIG2Compression,       /* 10 ISO/IEC std 14492 / ITU-T rec T.88 */
  JPEG2000Compression,    /* 11 ISO/IEC std 15444-1 */
  JPEGCompression,        /* 12 */
  LosslessJPEGCompression,/* 13 */
  LZMACompression,        /* 14 Lempel-Ziv-Markov chain algorithm */
  LZWCompression,         /* 15 */
  NoCompression,          /* 16 */
  PizCompression,         /* 17 */
  Pxr24Compression,       /* 18 */
  RLECompression,         /* 19 */
  ZipCompression,         /* 20 */
  ZipSCompression,        /* 21 */
  ZstdCompression,        /* 22 */
  WebPCompression,        /* 23 */
  DWAACompression,        /* 24 */
  DWABCompression,        /* 25 */
  BC7Compression,         /* 26 */
  BC5Compression          /* 27 */
} CompressionType;
```

#### ImageMagick FilterType
```C
typedef enum
{
  UndefinedFilter,          /* 0 */
  PointFilter,              /* 1 */
  BoxFilter,                /* 2 */
  TriangleFilter,           /* 3 */
  HermiteFilter,            /* 4 */
  HannFilter,               /* 5 */
  HammingFilter,            /* 6 */
  BlackmanFilter,           /* 7 */
  GaussianFilter,           /* 8 */
  QuadraticFilter,          /* 9 */
  CubicFilter,              /* 10 */
  CatromFilter,             /* 11 */
  MitchellFilter,           /* 12 */
  JincFilter,               /* 13 */
  SincFilter,               /* 14 */
  SincFastFilter,           /* 15 */
  KaiserFilter,             /* 16 */
  WelchFilter,              /* 17 */
  ParzenFilter,             /* 18 */
  BohmanFilter,             /* 19 */
  BartlettFilter,           /* 20 */
  LagrangeFilter,           /* 21 */
  LanczosFilter,            /* 22 */
  LanczosSharpFilter,       /* 23 */
  Lanczos2Filter,           /* 24 */
  Lanczos2SharpFilter,      /* 25 */
  RobidouxFilter,           /* 26 */
  RobidouxSharpFilter,      /* 27 */
  CosineFilter,             /* 28 */
  SplineFilter,             /* 29 */
  LanczosRadiusFilter,      /* 30 */
  CubicSplineFilter,        /* 31 */
  SentinelFilter  /* a count of all the filters, not a real filter */
} FilterType;
```

