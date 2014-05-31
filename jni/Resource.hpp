#ifndef _RESOURCE_HPP_
#define _RESOURCE_HPP_

#include "Types.hpp"

#include <android_native_app_glue.h>

struct ResourceDescriptor {
        int32_t mDescriptor;
        off_t mStart;
        off_t mLength;
    };

    class Resource {
    public:
        Resource(android_app* pApplication, const char* pPath);

        status open();
        void close();
        status read(void* pBuffer, size_t pCount);

        const char* getPath();
        off_t getLength();
        const void* bufferize();

        ResourceDescriptor descript();

    private:
        const char* mPath;
        AAssetManager* mAssetManager;
        AAsset* mAsset;
    };

#endif
