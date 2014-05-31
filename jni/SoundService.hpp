#ifndef _SOUNDSERVICE_HPP_
#define _SOUNDSERVICE_HPP_

#include "Sound.hpp"
#include "Types.hpp"
#include "Assets.hpp"


#include <android_native_app_glue.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>


    class SoundService {
    public:
        SoundService(android_app* pApplication);
        ~SoundService();

        status start();
        void stop();

        status playBGM(const char* pPath);
        void stopBGM();

        Sound* registerSound(const char* pPath);
        void playSound(Sound* pSound);


    public:
        bool soundEnabled();
        bool musicEnabled();
        void setSoundState(bool pState);
        void setMusicState(bool pState);
        void setAssets(Assets* pAssets);
        Assets* getAssets();

    private:
        status startSoundPlayer();

    private:
        android_app* mApplication;

        SLObjectItf mEngineObj; SLEngineItf mEngine;
        SLObjectItf mOutputMixObj;


        SLObjectItf mBGMPlayerObj; SLPlayItf mBGMPlayer;
        SLSeekItf mBGMPlayerSeek;


        SLObjectItf mPlayerObj; SLPlayItf mPlayer;
        SLBufferQueueItf mPlayerQueue;

        Sound* mSounds[32]; int32_t mSoundCount;

        bool mSoundEnabled,
			 mMusicEnabled;
       Assets* mAssets;
    };

#endif
