#include "Log.hpp"
#include "Texture.hpp"


    Texture::Texture(android_app* pApplication,
					 const char* pPath)
    :  mResource(pApplication, pPath)
    {
    	mTextureId = 0;
    	mWidth = 0;
    	mHeight = 0;
    	mApplication = pApplication;
    }

    Texture::~Texture() {
		/*
		 *  Обнуляем указатели. Удаление объектов займется
		 * класс, который их создал.
		 */
    	mApplication = NULL;
    }


    const char* Texture::getPath() {
        return mResource.getPath();
    }


    int32_t Texture::getHeight() {
        return mHeight;
    }


    int32_t Texture::getWidth() {
        return mWidth;
    }


    GLuint Texture::getTextureId() {
    	return mTextureId;
    }

    android_app* Texture::getApplication() {
    	return mApplication;
    }


    uint8_t* Texture::loadImage() {
    	//Log::info("Loading texture %s", mResource.getPath());


        png_byte lHeader[8];			// заголовок PNG файла
        png_structp lPngPtr = NULL;     // структура PNG файла
        png_infop lInfoPtr = NULL;		// информация о PNG файле
        png_byte* lImageBuffer = NULL; 	// буфер изображения
        png_bytep* lRowPtrs = NULL;     // указатели для каждого ряда буфера изображения
        png_int_32 lRowSize; 			// размер ряда буфера изображения
        bool lTransparency; 			// есть ли прозрачность



        //  Открываем файл и проверяем по первым 8 байтам
        // является ли он PNG
        if (mResource.open() != STATUS_OK) goto ERROR;
        Log_debug("Checking signature.");
        if (mResource.read(lHeader, sizeof(lHeader)) != STATUS_OK)
        goto ERROR;
        if (png_sig_cmp(lHeader, 0, 8) != 0) goto ERROR;



        Log_debug("Creating required structures.");
        // Создаем структуру
        lPngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!lPngPtr) goto ERROR;
        // Создаем информацию и структуре
        lInfoPtr = png_create_info_struct(lPngPtr);
        if (!lInfoPtr) goto ERROR;



        // Считываем PNG файл
        png_set_read_fn(lPngPtr, &mResource, callback_read);
        //  Если произойдет ошибка во время чтения - перейдем к
        // оператору goto
        if (setjmp(png_jmpbuf(lPngPtr))) goto ERROR;



        // Читаем заголовок файла, игнорируем первые 8 байтов
        png_set_sig_bytes(lPngPtr, 8);
        png_read_info(lPngPtr, lInfoPtr);

        // Извлекаем информацию о файле и обновляем структуру
        png_int_32 lDepth, lColorType;
        png_uint_32 lWidth, lHeight;
        png_get_IHDR(lPngPtr, lInfoPtr, &lWidth, &lHeight,
                     &lDepth, &lColorType, NULL, NULL, NULL);
        mWidth = lWidth; mHeight = lHeight;


        /*
        *    PNG файлы могут быть закодированы в различных
        *   форматах (RGB,RGBA) R,G и B каналы могут быть
        *   закодированны до 16 бит. Библиотека libpng
        *   предоставляет функции для транформации необычных
        *   форматов в более классические - 8 бит для канала с
        *   поддержкой прозрачности или нет.
        */

        lTransparency = false;
        if (png_get_valid(lPngPtr, lInfoPtr, PNG_INFO_tRNS)) {
        	png_set_tRNS_to_alpha(lPngPtr);
        	lTransparency = true;
        	goto ERROR;
        }

        // Если нужно, расширяем канал до 8 бит
        if (lDepth < 8) {
        	png_set_packing (lPngPtr);
        // Если нужно, сужаем канал до 8 бит
        } else if (lDepth == 16) {
            png_set_strip_16(lPngPtr);
        }

        // Проверяем нужна ли конвертация в RGBA формат
        switch (lColorType) {
			case PNG_COLOR_TYPE_PALETTE:
				png_set_palette_to_rgb(lPngPtr);
                mFormat = lTransparency ? GL_RGBA : GL_RGB;
                break;
            case PNG_COLOR_TYPE_RGB:
                mFormat = lTransparency ? GL_RGBA : GL_RGB;
                break;
            case PNG_COLOR_TYPE_RGBA:
                mFormat = GL_RGBA;
                break;
            case PNG_COLOR_TYPE_GRAY:
                png_set_expand_gray_1_2_4_to_8(lPngPtr);
                mFormat = lTransparency ? GL_LUMINANCE_ALPHA:GL_LUMINANCE;
                break;
            case PNG_COLOR_TYPE_GA:
                png_set_expand_gray_1_2_4_to_8(lPngPtr);
                mFormat = GL_LUMINANCE_ALPHA;
                break;
        }
        // Сохраняем изменения
        png_read_update_info(lPngPtr, lInfoPtr);



        // Получаем размер ряда
        lRowSize = png_get_rowbytes(lPngPtr, lInfoPtr);
        if (lRowSize <= 0) goto ERROR;
        // Создаем буффер изображения, который потом отправится к Opengl
        lImageBuffer = new png_byte[lRowSize * lHeight];
        if (!lImageBuffer) goto ERROR;

        /*
         *  lRowPtrs - указатели для каждого ряда буфера изображения. Порядок в
         * ряду инвертирован т.к OpenGl и PNGs используют разную систему координат
         * У Open Gl - 1-ый пиксель - нижний левый угол
         * У PNGs -    1-ый пиксель - верхний левый угол
         */
         lRowPtrs = new png_bytep[lHeight];
         if (!lRowPtrs) goto ERROR;
         for (int32_t i = 0; i < lHeight; ++i) {
        	 lRowPtrs[lHeight - (i + 1)] = lImageBuffer + i * lRowSize;
         }

         // Считываем содержание картинки
         png_read_image(lPngPtr, lRowPtrs);

         // Освобождаем память и ресурсы
         mResource.close();
         png_destroy_read_struct(&lPngPtr, &lInfoPtr, NULL);
         delete[] lRowPtrs;
         return lImageBuffer;


         // Сюда приходим оператором goto в случае ошибок
         ERROR:
			 Log::error("Error while reading PNG file");
			 mResource.close();
			 delete[] lRowPtrs;
			 delete[] lImageBuffer;
			 if (lPngPtr != NULL) {
				 png_infop* lInfoPtrP = lInfoPtr != NULL ? &lInfoPtr: NULL;
				 png_destroy_read_struct(&lPngPtr, lInfoPtrP, NULL);
			 }
			 return NULL;
    }


    void Texture::callback_read(png_structp pStruct,
        png_bytep pData, png_size_t pSize) {
        Resource* lResource = ((Resource*) png_get_io_ptr(pStruct));
        if (lResource->read(pData, pSize) != STATUS_OK) {
            lResource->close();
        }
    }


    status Texture::load() {
        uint8_t* lImageBuffer = loadImage();
        if (lImageBuffer == NULL) {
            return STATUS_KO;
        }

        // Создаем новую текстуру
        GLenum lErrorResult;
        glGenTextures(1, &mTextureId);
        glBindTexture(GL_TEXTURE_2D, mTextureId);

        // Задаем для нее основные параметры

        setParams(GL_NEAREST,
				  GL_NEAREST,
				  GL_CLAMP_TO_EDGE,
				  GL_CLAMP_TO_EDGE);



        // Загружаем информацию об изображении
        glTexImage2D(GL_TEXTURE_2D, 0, mFormat, mWidth, mHeight, 0,
        		mFormat, GL_UNSIGNED_BYTE, lImageBuffer);


        // Освобождаем память
        delete[] lImageBuffer;


        if (glGetError() != GL_NO_ERROR) {
            Log::error("Error loading texture into OpenGL.");
            //Log::error("Error type: %d", glGetError());
            dispose();
            return STATUS_KO;
        }
        return STATUS_OK;
    }


    void Texture::setParams(GLint pMinFilter, GLint pMagFilter,
    		GLint pWrapS, GLint pWrapT) {

    	// Сохраняем информацию о параметрах
    	mMinFilter = pMinFilter;
    	mMagFilter = pMagFilter;
    	mWrapS = pWrapS;
    	mWrapT = pWrapT;

    	// Задаем параметры для текстуры

    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, pMinFilter);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, pMagFilter);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, pWrapS);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, pWrapT);




    	/*
    	 * 		1. GL_TEXTURE_MIN_FILTER - поведение текстуры при уменьшении
	     * 	GL_NEAREST - возвращает значение элемента текстуры, который является самым близким.
		 * 	GL_LINEAR - возвращает среднее значение из четырёх ближайших к центру тестируемого пиксела.
		 *
		 *
		 * 		2. GL_TEXTURE_MAG_FILTER - поведение текстуры при увеличении
		 * 	GL_NEAREST - -//-/-
		 * 	GL_LINEAR -  -//-/-
		 *
		 *
		 * 		3. GL_TEXTURE_WRAP_S - возможность повторения текстуры по координате S.
		 * 	GL_CLAMP - запрещает повторение текстуры. Одна её копия будет натянута на весь объект.
		 *	GL_REPEAT - разрешает повторение текстуры.
		 *
		 *
		 * 		4. GL_TEXTURE_WRAP_T - возможность повторения текстуры по координате T.
		 * 	GL_CLAMP - -//-/-
		 *	GL_REPEAT - -//-/-
		 *
    	 */
    }


    void Texture::dispose() {
        if (mTextureId != 0) {
            glDeleteTextures(1, &mTextureId);
            mTextureId = 0;
        }
        mWidth = 0; mHeight = 0; mFormat = 0;
    }


    void Texture::bind() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mTextureId);
    }

