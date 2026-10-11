#include <cstdlib>
#include <miniz/miniz.h>

static unsigned char* myStbiCompress(const unsigned char* data,
                                     const int            data_len,
                                     int*                 out_len,
                                     const int            quality) {
    const auto source_len = static_cast<uLong>(data_len);
    uLongf capacity       = compressBound(source_len);

    const auto output     = static_cast<unsigned char*>(malloc((size_t)capacity));
    if (output == nullptr)
        return nullptr;

    const int result = compress2(output, &capacity, data, source_len, quality);

    if (result != Z_OK) {
        free(output);
        return nullptr;
    }

    *out_len = static_cast<int>(capacity);

    return output;
}

#define STBIW_ZLIB_COMPRESS myStbiCompress

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"

#undef STBIW_ZLIB_COMPRESS
#undef STB_IMAGE_IMPLEMENTATION
#undef STB_IMAGE_WRITE_IMPLEMENTATION
