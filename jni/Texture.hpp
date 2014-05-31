#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_


#include "Resource.hpp"
#include "Types.hpp"

#include <android_native_app_glue.h>
#include <GLES/gl.h>
#include <png.h>

	/*
	 *   Texture - класс, объекты которого загружают тектуру, при необходимости
	 *  присоеденияют ее к OpenGl или удаляют информацию о ней.
	 */

	class Texture {
    public:
        Texture(android_app* pApplication, const char* pPath);
        ~Texture();

        const char* getPath(); 	// получаем путь к изображению
        int32_t getHeight();    // получаем высоту текстуры
        int32_t getWidth();     // получаем ширину текстуры
        GLuint getTextureId();  // получаем текстуру
        android_app* getApplication();

        status load();  		// загружаем текстуру ( 1. создаем новую текстуру,
								//					    2. устанавливаем основные параметры,
								//					    3. загружаем информацию об изображении)

        void bind();			// присоеденяем текстуру к openGl

        void dispose(); 		// удаляем текстуру, отсоединяем от openGl

        // устанавливаем основные параметры текстуры
        void setParams(GLint pMinFilter, // поведение текстуры при уменьшении
        		GLint pMagFilter, // поведение текстуры при увеличении
        		GLint pWrapS, 	  // поведение текстуры по коорд. S
        		GLint pWrapT); 	  // поведение текстуры по коорд. T

    protected:
        uint8_t* loadImage(); // считывание буффера изображения
							  // с помощью библиотеки libpng
    private:
        // вспомогательная ф-я, вызываемая при loadImage()
        static void callback_read(png_structp pStruct,
								  png_bytep pData,
								  png_size_t pSize);

    private:
        // 4 основных параметра текстуры
        GLuint mMinFilter;  // повединие текстуры при уменьшении
        GLuint mMagFilter;  // поведение текстуры при увеличении
        GLuint mWrapS; 		// поведение текстуры по координате S
        GLuint mWrapT; 		// поведение текстуры по координате T

        Resource mResource; // путь к изображению
        GLuint mTextureId;  // текстура
        int32_t mWidth,		// ширина текстуры
				mHeight;    // высота текстуры
        GLint mFormat;
        android_app* mApplication;
    };

#endif
