#include "SoundService.hpp"
#include "Resource.hpp"
#include "Log.hpp"


    SoundService::SoundService(android_app* pApplication) :
        mApplication(pApplication),
        mEngineObj(NULL), mEngine(NULL),
        mOutputMixObj(NULL),
        mBGMPlayerObj(NULL), mBGMPlayer(NULL), mBGMPlayerSeek(NULL),
        mPlayerObj(NULL), mPlayer(NULL), mPlayerQueue(NULL),
        mSounds(), mSoundCount(0) {
        //Log::info("Creating SoundService.");

        mSoundEnabled = true;
        mMusicEnabled = true;


    }

    SoundService::~SoundService() {
        //Log::info("Destroying SoundService.");

        for (int32_t i = 0; i < mSoundCount; ++i) {
            delete mSounds[i];
            mSoundCount = 0;
        }
        mApplication = NULL;
        mAssets = NULL;
    }

    status SoundService::start() {
       // Log::info("Starting SoundService.");

        // загружаем звуки
             mAssets->loadSounds(this);

        SLresult lRes;
        const SLuint32      lEngineMixIIDCount = 1;
        const SLInterfaceID lEngineMixIIDs[]   = {SL_IID_ENGINE};
        const SLboolean     lEngineMixReqs[]   = {SL_BOOLEAN_TRUE};
        const SLuint32      lOutputMixIIDCount = 0;
        const SLInterfaceID lOutputMixIIDs[]   = {};
        const SLboolean     lOutputMixReqs[]   = {};


        lRes = slCreateEngine(&mEngineObj, 0, NULL,
            lEngineMixIIDCount, lEngineMixIIDs, lEngineMixReqs);
        if (lRes != SL_RESULT_SUCCESS) goto ERROR;
        lRes = (*mEngineObj)->Realize(mEngineObj,SL_BOOLEAN_FALSE);
        if (lRes != SL_RESULT_SUCCESS) goto ERROR;
        lRes = (*mEngineObj)->GetInterface(mEngineObj, SL_IID_ENGINE,
            &mEngine);
        if (lRes != SL_RESULT_SUCCESS) goto ERROR;


        lRes = (*mEngine)->CreateOutputMix(mEngine, &mOutputMixObj,
            lOutputMixIIDCount, lOutputMixIIDs, lOutputMixReqs);
        lRes = (*mOutputMixObj)->Realize(mOutputMixObj,
            SL_BOOLEAN_FALSE);


        if (startSoundPlayer() != STATUS_OK) goto ERROR;


        for (int32_t i = 0; i < mSoundCount; ++i) {
            if (mSounds[i]->load() != STATUS_OK) goto ERROR;
        }





        return STATUS_OK;

    ERROR:
        Log::error("Error while starting SoundService");
        stop();
        return STATUS_KO;
    }

    void SoundService::stop() {
        //Log::info("Stopping SoundService.");
        stopBGM();


        if (mPlayerObj != NULL) {
            (*mPlayerObj)->Destroy(mPlayerObj);
            mPlayerObj = NULL; mPlayer = NULL; mPlayerQueue = NULL;
        }


        if (mOutputMixObj != NULL) {
            (*mOutputMixObj)->Destroy(mOutputMixObj);
            mOutputMixObj = NULL;
        }
        if (mEngineObj != NULL) {
            (*mEngineObj)->Destroy(mEngineObj);
            mEngineObj = NULL; mEngine = NULL;
        }


        for (int32_t i = 0; i < mSoundCount; ++i) {
            mSounds[i]->unload();
        }
    }

    status SoundService::startSoundPlayer() {
        //Log::info("Starting sound player.");
        SLresult lRes;


        SLDataLocator_AndroidSimpleBufferQueue lDataLocatorIn;
        lDataLocatorIn.locatorType =
            SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE;

        lDataLocatorIn.numBuffers = 1;

        SLDataFormat_PCM lDataFormat;
        lDataFormat.formatType = SL_DATAFORMAT_PCM;
        lDataFormat.numChannels = 1; // Mono sound.
        lDataFormat.samplesPerSec = SL_SAMPLINGRATE_44_1;
        lDataFormat.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
        lDataFormat.containerSize = SL_PCMSAMPLEFORMAT_FIXED_16;
        lDataFormat.channelMask = SL_SPEAKER_FRONT_CENTER;
        lDataFormat.endianness = SL_BYTEORDER_LITTLEENDIAN;

        SLDataSource lDataSource;
        lDataSource.pLocator = &lDataLocatorIn;
        lDataSource.pFormat = &lDataFormat;

        SLDataLocator_OutputMix lDataLocatorOut;
        lDataLocatorOut.locatorType = SL_DATALOCATOR_OUTPUTMIX;
        lDataLocatorOut.outputMix = mOutputMixObj;

        SLDataSink lDataSink;
        lDataSink.pLocator = &lDataLocatorOut;
        lDataSink.pFormat = NULL;


        const SLuint32 lSoundPlayerIIDCount = 2;
        const SLInterfaceID lSoundPlayerIIDs[] =
            { SL_IID_PLAY, SL_IID_BUFFERQUEUE };
        const SLboolean lSoundPlayerReqs[] =
            { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };

        lRes = (*mEngine)->CreateAudioPlayer(mEngine, &mPlayerObj,
            &lDataSource, &lDataSink, lSoundPlayerIIDCount,
            lSoundPlayerIIDs, lSoundPlayerReqs);
        if (lRes != SL_RESULT_SUCCESS) goto ERROR;
        lRes = (*mPlayerObj)->Realize(mPlayerObj, SL_BOOLEAN_FALSE);
        if (lRes != SL_RESULT_SUCCESS) goto ERROR;

        lRes = (*mPlayerObj)->GetInterface(mPlayerObj, SL_IID_PLAY,
            &mPlayer);
        if (lRes != SL_RESULT_SUCCESS) goto ERROR;
        lRes = (*mPlayerObj)->GetInterface(mPlayerObj,
            SL_IID_BUFFERQUEUE, &mPlayerQueue);
        if (lRes != SL_RESULT_SUCCESS) goto ERROR;


        lRes = (*mPlayer)->SetPlayState(mPlayer,
            SL_PLAYSTATE_PLAYING);
        if (lRes != SL_RESULT_SUCCESS) goto ERROR;



        return STATUS_OK;
    ERROR:
        Log::error("Error while starting SoundPlayer");
        return STATUS_KO;
    }



    status SoundService::playBGM(const char* pPath) {
        SLresult lRes;
       // Log::info("Opening BGM %s", pPath);


        Resource lResource(mApplication, pPath);
        ResourceDescriptor lDescriptor = lResource.descript();
        if (lDescriptor.mDescriptor < 0) {
            Log::info("Could not open BGM file");
            return STATUS_KO;
        }

        SLDataLocator_AndroidFD lDataLocatorIn;
        lDataLocatorIn.locatorType = SL_DATALOCATOR_ANDROIDFD;
        lDataLocatorIn.fd          = lDescriptor.mDescriptor;
        lDataLocatorIn.offset      = lDescriptor.mStart;
        lDataLocatorIn.length      = lDescriptor.mLength;

        SLDataFormat_MIME lDataFormat;
        lDataFormat.formatType    = SL_DATAFORMAT_MIME;
        lDataFormat.mimeType      = NULL;
        lDataFormat.containerType = SL_CONTAINERTYPE_UNSPECIFIED;

        SLDataSource lDataSource;
        lDataSource.pLocator = &lDataLocatorIn;
        lDataSource.pFormat  = &lDataFormat;

        SLDataLocator_OutputMix lDataLocatorOut;
        lDataLocatorOut.locatorType = SL_DATALOCATOR_OUTPUTMIX;
        lDataLocatorOut.outputMix   = mOutputMixObj;

        SLDataSink lDataSink;
        lDataSink.pLocator = &lDataLocatorOut;
        lDataSink.pFormat  = NULL;


        const SLuint32 lBGMPlayerIIDCount = 2;
        const SLInterfaceID lBGMPlayerIIDs[] =
            { SL_IID_PLAY, SL_IID_SEEK };
        const SLboolean lBGMPlayerReqs[] =
            { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };

        lRes = (*mEngine)->CreateAudioPlayer(mEngine,
            &mBGMPlayerObj, &lDataSource, &lDataSink,
            lBGMPlayerIIDCount, lBGMPlayerIIDs, lBGMPlayerReqs);
        if (lRes != SL_RESULT_SUCCESS) goto ERROR;
        lRes = (*mBGMPlayerObj)->Realize(mBGMPlayerObj,
            SL_BOOLEAN_FALSE);
        if (lRes != SL_RESULT_SUCCESS) goto ERROR;

        lRes = (*mBGMPlayerObj)->GetInterface(mBGMPlayerObj,
            SL_IID_PLAY, &mBGMPlayer);
        if (lRes != SL_RESULT_SUCCESS) goto ERROR;
        lRes = (*mBGMPlayerObj)->GetInterface(mBGMPlayerObj,
            SL_IID_SEEK, &mBGMPlayerSeek);
        if (lRes != SL_RESULT_SUCCESS) goto ERROR;


        lRes = (*mBGMPlayerSeek)->SetLoop(mBGMPlayerSeek,
            SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);
        if (lRes != SL_RESULT_SUCCESS) goto ERROR;
        lRes = (*mBGMPlayer)->SetPlayState(mBGMPlayer,
            SL_PLAYSTATE_PLAYING);
        if (lRes != SL_RESULT_SUCCESS) goto ERROR;

        return STATUS_OK;

    ERROR:
        return STATUS_KO;
    }

    void SoundService::stopBGM() {
        if (mBGMPlayer != NULL) {
            SLuint32 lBGMPlayerState;
            (*mBGMPlayerObj)->GetState(mBGMPlayerObj,
                &lBGMPlayerState);
            if (lBGMPlayerState == SL_OBJECT_STATE_REALIZED) {
                (*mBGMPlayer)->SetPlayState(mBGMPlayer,
                    SL_PLAYSTATE_PAUSED);

                (*mBGMPlayerObj)->Destroy(mBGMPlayerObj);
                mBGMPlayerObj = NULL;
                mBGMPlayer = NULL; mBGMPlayerSeek = NULL;
            }
        }
    }

    Sound* SoundService::registerSound(const char* pPath) {

        for (int32_t i = 0; i < mSoundCount; ++i) {
            if (strcmp(pPath, mSounds[i]->getPath()) == 0) {
                return mSounds[i];
            }
        }

        Sound* lSound = new Sound(mApplication, pPath);
        mSounds[mSoundCount++] = lSound;
        return lSound;
    }

    void SoundService::playSound(Sound* pSound) {
        SLresult lRes;
        SLuint32 lPlayerState;
        (*mPlayerObj)->GetState(mPlayerObj, &lPlayerState);
        if (lPlayerState == SL_OBJECT_STATE_REALIZED) {
            int16_t* lBuffer = (int16_t*) pSound->mBuffer;
            off_t    lLength = pSound->mLength;


            lRes = (*mPlayerQueue)->Clear(mPlayerQueue);
            if (lRes != SL_RESULT_SUCCESS) goto ERROR;


            lRes = (*mPlayerQueue)->Enqueue(mPlayerQueue, lBuffer,
                lLength);
            if (lRes != SL_RESULT_SUCCESS) goto ERROR;
        }
        return;

    ERROR:
        Log::error("Error trying to play sound");
    }

    bool SoundService::soundEnabled() {
    	return mSoundEnabled;
    }

    bool SoundService::musicEnabled() {
    	return mMusicEnabled;
    }

    void SoundService::setSoundState(bool pState) {
    	mSoundEnabled = pState;
    }

    void SoundService::setMusicState(bool pState) {
    	mMusicEnabled = pState;
    }

    void SoundService::setAssets(Assets* pAssets) {
    	mAssets = pAssets;
    }

    Assets* SoundService::getAssets() {
    	return mAssets;
    }

