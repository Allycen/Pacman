#ifndef _GRAPHICSSERVICE_HPP_
#define _GRAPHICSSERVICE_HPP_

#include "Texture.hpp"
#include "Types.hpp"
#include "Assets.hpp"

#include <android_native_app_glue.h>
#include <EGL/egl.h>


    class GraphicsService {
    public:
        GraphicsService(android_app* pApplication);
        ~GraphicsService();

        const int32_t& getHeight();
        const int32_t& getWidth();
        android_app* getApplication();
        Assets* getAssets();
        void setAssets(Assets* pAssets);
        int32_t getScreenResolution();

        status start();
        void stop();
        status update();

        void setupGlConfigs();
        void updateGlConfigs();


    private:
        android_app* mApplication;

        int32_t mWidth, mHeight;
        EGLDisplay mDisplay;
        EGLSurface mSurface;
        EGLContext mContext;

        Assets* mAssets;

        int32_t mScreenResolution;
    };

#endif
