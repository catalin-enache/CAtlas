
cmake_minimum_required(VERSION 3.25)
project(Atlas CXX) # CXX is required by magick-baseconfig.h => MAGICKCORE_CHANNEL_MASK_DEPTH = 64

set(CMAKE_C_STANDARD 11)

set(SHARED_SOURCES src/img_manip.c src/img_manip.h src/dir_files_utils.c src/dir_files_utils.h src/extract_config.c src/extract_config.h src/errors.c src/errors.h src/definitions.h src/shared_types.h src/calculations.c src/calculations.h src/utils.c src/utils.h)

# these are for CXX compiler. Comment them when/if switching to C compiler
file(GLOB_RECURSE C_SOURCES "*.c")
set_source_files_properties(${C_SOURCES} PROPERTIES LANGUAGE CXX)

# "D:\Projects\CAtlasIM\ImageMagick-7.1.1-Q16-HDRI\magick.exe" identify -verbose atlas.exr
# "D:\Projects\CAtlasIM\ImageMagick-7.1.1-Q16-HDRI\magick.exe" output.tiff -depth 32 atlas.exr
############################ Using ImageMagick ###################

# Add your executable
add_executable(ExtractChannels extract_channels.c ${SHARED_SOURCES})
add_executable(MergeChannels merge_channels.c ${SHARED_SOURCES})
add_executable(Atlas atlas.c ${SHARED_SOURCES})
add_executable(TestUtils src/utils.test.c ${SHARED_SOURCES})

# Find ImageMagick
#find_package(ImageMagick 7 COMPONENTS MagickWand REQUIRED)
set(ImageMagick_INCLUDE_DIRS ./ImageMagick-7.1.1-Q16-HDRI/include)
find_library(ImageMagick_LIBRARIES_MagickWand NAMES CORE_RL_MagickWand_ PATHS "./ImageMagick-7.1.1-Q16-HDRI/lib")
find_library(ImageMagick_LIBRARIES_MagickCore NAMES CORE_RL_MagickCore_ PATHS "./ImageMagick-7.1.1-Q16-HDRI/lib")
find_library(LibTiff_LIB NAMES libtiff PATHS "./ImageMagick-7.1.1-Q16-HDRI/lib")



# Include the headers
target_include_directories(ExtractChannels PUBLIC ${ImageMagick_INCLUDE_DIRS})
target_include_directories(MergeChannels PUBLIC ${ImageMagick_INCLUDE_DIRS})
target_include_directories(Atlas PUBLIC ${ImageMagick_INCLUDE_DIRS})
target_include_directories(TestUtils PUBLIC ${ImageMagick_INCLUDE_DIRS})

# Link the libraries to your project
#link_directories(${ImageMagick_LIBRARIES})

# Link your executable with the libraries
target_link_libraries(ExtractChannels ${ImageMagick_LIBRARIES_MagickWand} ${ImageMagick_LIBRARIES_MagickCore} ${LibTiff_LIB})
target_link_libraries(MergeChannels ${ImageMagick_LIBRARIES_MagickWand} ${ImageMagick_LIBRARIES_MagickCore} ${LibTiff_LIB})
target_link_libraries(Atlas ${ImageMagick_LIBRARIES_MagickWand} ${ImageMagick_LIBRARIES_MagickCore} ${LibTiff_LIB})
target_link_libraries(TestUtils ${ImageMagick_LIBRARIES_MagickWand} ${ImageMagick_LIBRARIES_MagickCore} ${LibTiff_LIB})

message(ImageMagick_INCLUDE_DIRS ${ImageMagick_INCLUDE_DIRS})
message(ImageMagick_LIBRARIES_MagickWand ${ImageMagick_LIBRARIES_MagickWand})
message(ImageMagick_LIBRARIES_MagickCore ${ImageMagick_LIBRARIES_MagickCore})
message(LibTiff_LIB ${LibTiff_LIB})
