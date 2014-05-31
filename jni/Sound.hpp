#ifndef _SOUND_HPP_
#define _SOUND_HPP_

class SoundService;

#include "Resource.hpp"
#include "Types.hpp"

	/*
	 *  Класс, реализующий игровой звук
	 */

    class Sound {
    public:
        Sound(android_app* pApplication, const char* pPath);

        const char* getPath();

        status load();
        status unload();

    private:
        friend class SoundService;

    private:
        Resource mResource;
        uint8_t* mBuffer; off_t mLength;
    };

#endif
