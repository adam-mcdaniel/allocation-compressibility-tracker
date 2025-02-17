#pragma once

#include <config.hpp>
#include <stack_string.hpp>
#include <interval_test.hpp>
#include <timer.hpp>
#include <stack_csv.hpp>

#ifdef CHECK_DYNAMIC_LIBRARIES
#include <dlfcn.h>
#endif

#ifdef USE_ZLIB_COMPRESSION
#include <zlib.h>
#endif

#ifdef USE_LZ4_COMPRESSION
#include <lz4.h>
#endif

#ifdef USE_LZO_COMPRESSION
#include <lzo/lzoconf.h>
#include <lzo/lzo1x.h>
#ifndef LZO_ALIGN
#define LZO_ALIGN(x) __attribute__((aligned(x)))
#endif
#ifndef __LZO_MMODEL
#define __LZO_MMODEL
#endif
// Declare a scratch memory area for LZO
LZO_ALIGN(16) unsigned char __LZO_MMODEL lzo_work[LZO1X_1_MEM_COMPRESS];
#endif

#ifdef USE_SNAPPY_COMPRESSION
#include <snappy-c.h>
#endif

#ifdef USE_ZSTD_COMPRESSION
#include <zstd.h>
#endif

#ifdef USE_LZF_COMPRESSION
#include <liblzf/lzf.h>
#endif

#ifdef USE_LZ4HC_COMPRESSION
#include <lz4hc.h>
#endif


#define min(a, b) ((a) < (b) ? (a) : (b))

// Path: src/compression_test.cpp
#define MAX_COMPRESSED_SIZE 0x100000
#define MAX_PAGES 0x10000

#ifdef CHECK_DYNAMIC_LIBRARIES
void check_dynamic_libraries() {
    static bool checked = false;
    if (checked) {
        return;
    }
    stack_infof("Checking dynamic libraries...\n");

    #ifdef USE_ZLIB_COMPRESSION
    void *zlib_handle = dlopen("libz.so", RTLD_LAZY);
    if (zlib_handle == NULL) {
        stack_errorf("libz.so not found\n");
        exit(1);
    } else {
        stack_debugf("libz.so found\n");
        dlclose(zlib_handle);
    }
    #endif

    #ifdef USE_LZ4_COMPRESSION
    void *lz4_handle = dlopen("liblz4.so", RTLD_LAZY);
    if (lz4_handle == NULL) {
        stack_errorf("liblz4.so not found\n");
        exit(1);
    } else {
        stack_debugf("liblz4.so found\n");
        dlclose(lz4_handle);
    }
    #endif

    #ifdef USE_LZO_COMPRESSION
    void *lzo_handle = dlopen("liblzo2.so", RTLD_LAZY);
    if (lzo_handle == NULL) {
        stack_errorf("liblzo2.so not found\n");
        exit(1);
    } else {
        stack_debugf("liblzo2.so found\n");
        dlclose(lzo_handle);
    }
    #endif

    #ifdef USE_SNAPPY_COMPRESSION
    void *snappy_handle = dlopen("libsnappy.so", RTLD_LAZY);
    if (snappy_handle == NULL) {
        stack_errorf("libsnappy.so not found\n");
        exit(1);
    } else {
        stack_debugf("libsnappy.so found\n");
        dlclose(snappy_handle);
    }
    #endif

    #ifdef USE_ZSTD_COMPRESSION
    void *zstd_handle = dlopen("libzstd.so", RTLD_LAZY);
    if (zstd_handle == NULL) {
        stack_errorf("libzstd.so not found\n");
        exit(1);
    } else {
        stack_debugf("libzstd.so found\n");
        dlclose(zstd_handle);
    }
    #endif

    #ifdef USE_LZF_COMPRESSION
    void *lzf_handle = dlopen("liblzf.so", RTLD_LAZY);
    if (lzf_handle == NULL) {
        stack_errorf("liblzf.so not found\n");
        exit(1);
    } else {
        stack_debugf("liblzf.so found\n");
        dlclose(lzf_handle);
    }
    #endif

    #ifdef USE_LZ4HC_COMPRESSION
    void *lz4hc_handle = dlopen("liblz4hc.so", RTLD_LAZY);
    if (lz4hc_handle == NULL) {
        stack_errorf("liblz4hc.so not found\n");
        exit(1);
    } else {
        stack_debugf("liblz4hc.so found\n");
        dlclose(lz4hc_handle);
    }
    #endif
    stack_infof("Dynamic libraries check succeeded!\n");
    checked = true;
}
#else
void check_dynamic_libraries() {}
#endif


typedef enum {
    #ifdef USE_ZLIB_COMPRESSION
    COMPRESS_ZLIB = 1,
    #endif
    #ifdef USE_LZ4_COMPRESSION
    COMPRESS_LZ4 = 2,
    #endif
    #ifdef USE_LZO_COMPRESSION
    COMPRESS_LZO = 3,
    #endif
    #ifdef USE_SNAPPY_COMPRESSION
    COMPRESS_SNAPPY = 4,
    #endif
    #ifdef USE_ZSTD_COMPRESSION
    COMPRESS_ZSTD = 5,
    #endif
    #ifdef USE_LZF_COMPRESSION
    COMPRESS_LZF = 6,
    #endif
    #ifdef USE_LZ4HC_COMPRESSION
    COMPRESS_LZ4HC = 7
    #endif
} CompressionType;

CSVString compression_to_string(CompressionType type) {
    switch (type) {
        #ifdef USE_ZLIB_COMPRESSION
        case COMPRESS_ZLIB:
            return "zlib";
        #endif
        #ifdef USE_LZ4_COMPRESSION
        case COMPRESS_LZ4:
            return "lz4";
        #endif
        #ifdef USE_LZO_COMPRESSION
        case COMPRESS_LZO:
            return "lzo";
        #endif
        #ifdef USE_SNAPPY_COMPRESSION
        case COMPRESS_SNAPPY:
            return "snappy";
        #endif
        #ifdef USE_ZSTD_COMPRESSION
        case COMPRESS_ZSTD:
            return "zstd";
        #endif
        #ifdef USE_LZF_COMPRESSION
        case COMPRESS_LZF:
            return "lzf";
        #endif
        #ifdef USE_LZ4HC_COMPRESSION
        case COMPRESS_LZ4HC:
            return "lz4hc";
        #endif
    }
    return "unknown";
}

void init_compression() {
    #ifdef USE_LZO_COMPRESSION
    static bool has_lzo_init = false;
    if (has_lzo_init) {
        return;
    }
    // Call this function at the beginning of your program
    if (lzo_init() != LZO_E_OK) {
        printf("LZO initialization error\n");
        exit(1);
    }
    stack_infof("LZO initialized\n");
    has_lzo_init = true;
    #endif
}

#ifdef USE_ZLIB_COMPRESSION
const CompressionType DEFAULT_COMPRESSION_TYPE = COMPRESS_ZLIB;
#elif defined(USE_LZ4_COMPRESSION)
const CompressionType DEFAULT_COMPRESSION_TYPE = COMPRESS_LZ4;
#elif defined(USE_LZO_COMPRESSION)
const CompressionType DEFAULT_COMPRESSION_TYPE = COMPRESS_LZO;
#elif defined(USE_SNAPPY_COMPRESSION)
const CompressionType DEFAULT_COMPRESSION_TYPE = COMPRESS_SNAPPY;
#elif defined(USE_ZSTD_COMPRESSION)
const CompressionType DEFAULT_COMPRESSION_TYPE = COMPRESS_ZSTD;
#elif defined(USE_LZF_COMPRESSION)
const CompressionType DEFAULT_COMPRESSION_TYPE = COMPRESS_LZF;
#elif defined(USE_LZ4HC_COMPRESSION)
const CompressionType DEFAULT_COMPRESSION_TYPE = COMPRESS_LZ4HC;
#else
#error "No compression library defined"
#endif


static double total_compressed_sizes = 0;
static double total_uncompressed_sizes = 0;
static Stopwatch compression_overhead_timer;

template<size_t MaxUncompressedSize=0x1000000, bool CreateInternalBuffer = true>
class Compressor {
public:
    Compressor() : type(DEFAULT_COMPRESSION_TYPE) {
        check_dynamic_libraries();
        init_compression();
        stack_infof("Intialized compressor with %s\n", compression_to_string(type));
        if constexpr (CreateInternalBuffer) {
            stack_infof("   Internal buffer size: %d\n", max_compressed_size());
        }
    }

    Compressor(CompressionType type) : type(type) {
        check_dynamic_libraries();
        init_compression();
    }

    static StackVec<CompressionType, 20> supported_compression_types() {
        auto types = StackVec<CompressionType, 20>();
        #ifdef USE_ZLIB_COMPRESSION
        types.push(COMPRESS_ZLIB);
        #endif
        #ifdef USE_LZ4_COMPRESSION
        types.push(COMPRESS_LZ4);
        #endif
        #ifdef USE_LZO_COMPRESSION
        types.push(COMPRESS_LZO);
        #endif
        #ifdef USE_SNAPPY_COMPRESSION
        types.push(COMPRESS_SNAPPY);
        #endif
        #ifdef USE_ZSTD_COMPRESSION
        types.push(COMPRESS_ZSTD);
        #endif
        #ifdef USE_LZF_COMPRESSION
        types.push(COMPRESS_LZF);
        #endif
        #ifdef USE_LZ4HC_COMPRESSION
        types.push(COMPRESS_LZ4HC);
        #endif
        return types;
    }
    
    size_t max_compressed_size(size_t uncompressed_size=MaxUncompressedSize) {
        switch (type) {
            #ifdef USE_ZLIB_COMPRESSION
            case COMPRESS_ZLIB:
                return compressBound(uncompressed_size);
            #endif
            #ifdef USE_LZ4_COMPRESSION
            case COMPRESS_LZ4:
                return LZ4_COMPRESSBOUND(uncompressed_size);
            #endif
            #ifdef USE_LZO_COMPRESSION
            case COMPRESS_LZO:
                return uncompressed_size + uncompressed_size / 16 + 64 + 3;
            #endif
            #ifdef USE_SNAPPY_COMPRESSION
            case COMPRESS_SNAPPY:
                return snappy_max_compressed_length(uncompressed_size);
            #endif
            #ifdef USE_ZSTD_COMPRESSION
            case COMPRESS_ZSTD:
                return ZSTD_compressBound(uncompressed_size);
            #endif
            #ifdef USE_LZF_COMPRESSION
            case COMPRESS_LZF:
                return uncompressed_size + uncompressed_size / 16 + 64 + 3;
            #endif
            #ifdef USE_LZ4HC_COMPRESSION
            case COMPRESS_LZ4HC:
                return LZ4_COMPRESSBOUND(uncompressed_size);
            #endif
        }
        return 0;
    }

    size_t max_uncompressed_size() const {
        return MaxUncompressedSize;
    }

    size_t compress(const uint8_t *input_buffer, size_t uncompressed_size) {
        return compress(input_buffer, uncompressed_size, internal_buffer, max_compressed_size());
    }

    size_t compress(const uint8_t *input_buffer, size_t uncompressed_size, uint8_t *output_buffer, size_t output_size) {
        compression_overhead_timer.start();
        size_t compressed_size;
        if constexpr (CreateInternalBuffer) {
            compressed_size = max_compressed_size();
        } else {
            compressed_size = max_compressed_size(uncompressed_size);
        }
        total_uncompressed_sizes += uncompressed_size;

        [[maybe_unused]] int64_t err = 0;

        switch (type) {
            #ifdef USE_ZLIB_COMPRESSION
            case COMPRESS_ZLIB:
                err = compress2((Bytef*)output_buffer, (uLongf*)&compressed_size, (const Bytef*)input_buffer, uncompressed_size, Z_DEFAULT_COMPRESSION);
                // if (err != Z_OK) {
                //     stack_errorf("Zlib compression failed\n");
                //     return 0;
                // }
                switch ((int)err) {
                case Z_OK:
                    break;
                case Z_MEM_ERROR:
                    stack_errorf("Zlib compression failed: Z_MEM_ERROR\n");
                    compression_overhead_timer.stop();
                    return 0;
                case Z_BUF_ERROR:
                    stack_errorf("Zlib compression failed: Z_BUF_ERROR\n");
                    compression_overhead_timer.stop();
                    return 0;
                case Z_STREAM_ERROR:
                    stack_errorf("Zlib compression failed: Z_STREAM_ERROR\n");
                    compression_overhead_timer.stop();
                    return 0;
                case Z_DATA_ERROR:
                    stack_errorf("Zlib compression failed: Z_DATA_ERROR\n");
                    compression_overhead_timer.stop();
                    return 0;
                case Z_VERSION_ERROR:
                    stack_errorf("Zlib compression failed: Z_VERSION_ERROR\n");
                    compression_overhead_timer.stop();
                    return 0;
                default:
                    stack_errorf("Zlib compression failed: %d\n", err);
                    compression_overhead_timer.stop();
                    return 0;
                }
                break;
            #endif
            #ifdef USE_LZ4_COMPRESSION
            case COMPRESS_LZ4:
                compressed_size = LZ4_compress_default((const char*)input_buffer, (char*)output_buffer, uncompressed_size, compressed_size);
                if (compressed_size == 0) {
                    stack_errorf("LZ4 compression failed\n");
                    compression_overhead_timer.stop();
                    return 0;
                }
                break;
            #endif
            #ifdef USE_LZO_COMPRESSION
            case COMPRESS_LZO:
                err = lzo1x_1_compress((const unsigned char*)input_buffer, uncompressed_size, (unsigned char*)output_buffer, &compressed_size, lzo_work);
                if (compressed_size == 0) {
                    stack_warnf("LZO compression failed\n");
                    compression_overhead_timer.stop();
                    return 0;
                }
                break;
            #endif
            #ifdef USE_SNAPPY_COMPRESSION
            case COMPRESS_SNAPPY:
                err = snappy_compress((const char*)input_buffer, uncompressed_size, (char*)output_buffer, &compressed_size);
                if (err != SNAPPY_OK) {
                    stack_errorf("Snappy compression failed\n");
                    compression_overhead_timer.stop();
                    return 0;
                }
                break;
            #endif
            #ifdef USE_ZSTD_COMPRESSION
            case COMPRESS_ZSTD:
                compressed_size = ZSTD_compress(output_buffer, compressed_size, input_buffer, uncompressed_size, 1);
                if (ZSTD_isError(compressed_size)) {
                    stack_errorf("Zstd compression failed\n");
                    compression_overhead_timer.stop();
                    return 0;
                }
                break;
            #endif
            #ifdef USE_LZF_COMPRESSION
            case COMPRESS_LZF:
                compressed_size = lzf_compress(input_buffer, uncompressed_size, output_buffer, compressed_size);
                if (compressed_size == 0) {
                    stack_errorf("LZF compression failed\n");
                    compression_overhead_timer.stop();
                    return 0;
                }
                break;
            #endif
            #ifdef USE_LZ4HC_COMPRESSION
            case COMPRESS_LZ4HC:
                compressed_size = LZ4_compress_HC((const char*)input_buffer, (char*)output_buffer, uncompressed_size, compressed_size, LZ4HC_CLEVEL_MAX);
                if (compressed_size == 0) {
                    stack_errorf("LZ4HC compression failed\n");
                    compression_overhead_timer.stop();
                    return 0;
                }
                break;
            #endif
        }
        stack_debugf("Compressed buffer at %p of %d bytes to fit in %d bytes\n", input_buffer, uncompressed_size, compressed_size);
        total_compressed_sizes += compressed_size;
        compression_overhead_timer.stop();
        return compressed_size;
    }

    size_t compress_object(const Allocation &alloc) {
        return compress(alloc.ptr, alloc.size);
    }

    static void summary() {
        stack_infof("Compression overhead: %d ms\n", compression_overhead_timer.elapsed_milliseconds());
        stack_infof("Total uncompressed size: %f\n", total_uncompressed_sizes);
        stack_infof("Total compressed size: %f\n", total_compressed_sizes);
        stack_infof("Overall compression ratio: %f\n", total_compressed_sizes / total_uncompressed_sizes);
    }

    template<size_t MaxPhysicalPages=10000>
    size_t compress_physical_pages(const Allocation &alloc, size_t &uncompressed_size) {
        auto pages = alloc.physical_pages<MaxPhysicalPages>();
        size_t total_compressed_size = 0;
        for (size_t i = 0; i < pages.size(); i++) {
            total_compressed_size += compress(pages[i].ptr, pages[i].size);
        }
        return total_compressed_size;
    }
private:
    CompressionType type;
    
    uint8_t internal_buffer[CreateInternalBuffer ? int(MaxUncompressedSize * 1.5) : 1];
};


// void compress(void *input_buffer, uint64_t &uncompressed_size, uint64_t &compressed_size, CompressionType type) {
//     stack_debugf("Compressing buffer at %p\n", input_buffer);
//     stack_debugf("  size: %d\n", uncompressed_size);

//     compressed_size = MAX_COMPRESSED_SIZE;
//     if (uncompressed_size > MAX_COMPRESSED_SIZE) {
//         stack_warnf("Buffer too large to compress, truncating\n");
//         uncompressed_size = MAX_COMPRESSED_SIZE;
//     }

//     switch (type) {
//         #ifdef USE_ZLIB_COMPRESSION
//         case COMPRESS_ZLIB:
//             // Zlib compression
//             compress2((Bytef*)buffer, (uLongf*)&compressed_size, (const Bytef*)input_buffer, uncompressed_size, Z_BEST_COMPRESSION);
//             break;
//         #endif
//         #ifdef USE_LZ4_COMPRESSION
//         case COMPRESS_LZ4:
//             // LZ4 compression
//             // compressed_size = LZ4_compress_default((const char*)input_buffer, (char*)buffer, uncompressed_size, compressed_size);
//             if ((compressed_size = LZ4_compress_default((const char*)input_buffer, (char*)buffer, uncompressed_size, compressed_size)) == 0) {
//                 stack_warnf("LZ4 compression failed\n");
//             }
//             break;
//         #endif
//         #ifdef USE_LZO_COMPRESSION
//         case COMPRESS_LZO:
//             // LZO compression
//             if (lzo1x_1_compress((const unsigned char*)input_buffer, uncompressed_size, (unsigned char*)buffer, &compressed_size, lzo_work) == LZO_E_OK) {
//                 stack_warnf("LZO compression failed\n");
//             }
//             break;
//         #endif
//         #ifdef USE_SNAPPY_COMPRESSION
//         case COMPRESS_SNAPPY:
//             // Snappy compression
//             if (snappy_compress((const char*)input_buffer, uncompressed_size, (char*)buffer, &compressed_size) == SNAPPY_OK) {
//                 stack_warnf("Snappy compression failed\n");
//             }
//             break;
//         #endif
//         #ifdef USE_LZF_COMPRESSION
//         case COMPRESS_LZF:
//             // LZF compression
//             // compressed_size = lzf_compress(input_buffer, uncompressed_size, buffer, compressed_size);
//             if ((compressed_size = lzf_compress(input_buffer, uncompressed_size, buffer, compressed_size)) == 0) {
//                 stack_warnf("LZF compression failed\n");
//             }
//             break;
//         #endif
//         #ifdef USE_LZ4HC_COMPRESSION
//         case COMPRESS_LZ4HC:
//             // LZ4HC compression
//             // compressed_size = LZ4_compress_HC((const char*)input_buffer, (char*)buffer, uncompressed_size, compressed_size, LZ4HC_CLEVEL_MAX);
//             if ((compressed_size = LZ4_compress_HC((const char*)input_buffer, (char*)buffer, uncompressed_size, compressed_size, LZ4HC_CLEVEL_MAX)) == 0) {
//                 stack_warnf("LZ4HC compression failed\n");
//             }
//             break;
//         #endif
//         #ifdef USE_ZSTD_COMPRESSION
//         case COMPRESS_ZSTD:
//             // Zstd compression
//             // compressed_size = ZSTD_compress(buffer, compressed_size, input_buffer, uncompressed_size, 1);
//             if (ZSTD_isError(compressed_size = ZSTD_compress(buffer, compressed_size, input_buffer, uncompressed_size, 1))) {
//                 stack_warnf("Zstd compression failed\n");
//             }
//             break;
//         #endif
//     }

//     stack_debugf("Compressed buffer at %p\n", (void*)input_buffer);
//     stack_debugf("  size: %d\n", uncompressed_size);
//     stack_debugf("  compressed size: %d\n", compressed_size);
// }

// void compress(Allocation &alloc, uint64_t &uncompressed_size, uint64_t &compressed_size, CompressionType type) {
//     compress(alloc.ptr, uncompressed_size, compressed_size, type);
// }
