#include "GraphicsService.hpp"
#include "Log.hpp"
#include "Resource.hpp"

#include <GLES/gl.h>
#include <GLES/glext.h>


    GraphicsService::GraphicsService(android_app* pApplication) {
    	//Log::info("Creating GraphicsService.");
    	 mApplication = pApplication;
    	 mWidth = 0;
    	 mHeight = 0;
    	 mDisplay = EGL_NO_DISPLAY;
    	 mSurface = EGL_NO_CONTEXT;
    	 mContext = EGL_NO_SURFACE;
    }

    GraphicsService::~GraphicsService() {
      //  Log::info("Destructing GraphicsService.");
        mAssets = NULL;
        mApplication = NULL;
    }

    const int32_t& GraphicsService::getHeight() {
        return mHeight;
    }

    const int32_t& GraphicsService::getWidth() {
        return mWidth;
    }

    android_app* GraphicsService::getApplication() {
    	return mApplication;
    }

    Assets* GraphicsService::getAssets() {
    	return mAssets;
    }

    void GraphicsService::setAssets(Assets* pAssets) {
    	mAssets = pAssets;
    }

    int32_t GraphicsService::getScreenResolution() {
    	return mScreenResolution;
    }

    status GraphicsService::start() {
      //  Log::info("Starting GraphicsService.");

        /*
         *   Команды OpenGL ES требуют контекст рендеринга и поверхности
         *  для рисования. API OpenGl не занимается созданием этих объектов.
         *  За это отвечает EGL являясь как бы мостом между API OpenGl и
         *  той системой на которой работают(в данном случае AndroidNDK)
         */


         EGLint lFormat, lErrorResult;
         EGLBoolean passOk;
        /*
         *   Количество конфигураций может быть не одно. В нашем случае
         *  достаточна одна конфигурация.
         */
         EGLint lNumConfigs;

        /*
         *   EGLConfig хранит информации о поверхности EGL, информацию о
         *  свете, буффере и некоторых др. характиристиках
         */
         EGLConfig lConfig;


         /*
          *  Чтобы поддерживать формат RGB565 при рисовании необходимо
          *  определить слд. атрибуты (lAttributes)
          */

          const EGLint lAttributes[] = {
        		  EGL_RENDERABLE_TYPE, EGL_OPENGL_ES_BIT,
                  EGL_BLUE_SIZE, 5, EGL_GREEN_SIZE, 6, EGL_RED_SIZE, 5,
                  EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                  EGL_NONE
			  };


          Log_debug("Connecting to the display.");
          // Создаем и определяем соединение с внутренним EGL экраном
          mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
          if (mDisplay == EGL_NO_DISPLAY) goto ERROR; // если нет экрана
          if(!eglInitialize(mDisplay, NULL, NULL)) goto ERROR;




          Log_debug("Selecting a display config.");
          //  После необходимо задать конфигурацию для системы для
          // корректной отрисовки OpenGl
          if(!eglChooseConfig(mDisplay, lAttributes, &lConfig, 1, &lNumConfigs)  || (lNumConfigs <= 0)) goto ERROR;


          Log_debug("Configuring window format.");

         /*
          *   EGL_NATIVE_VISUAL_ID - атрибут EGLConfig который гарантирует
          *  доступ к ANativeWindow. В ф-и eglGetConfigAttrib мы реконфигурикуем
          *  буфферы EGL так, чтобы они подходили ANativeWindow.
          */

          if(!eglGetConfigAttrib(mDisplay, lConfig, EGL_NATIVE_VISUAL_ID, &lFormat))  goto ERROR;
          ANativeWindow_setBuffersGeometry(mApplication->window, 0, 0, lFormat);

          Log_debug("Initializing the display.");
          //  Создаем поверхность на которой будем рисовать
          mSurface = eglCreateWindowSurface(mDisplay, lConfig, mApplication->window, NULL);
          if (mSurface == EGL_NO_SURFACE) goto ERROR;

          // Создаем контекст
          mContext = eglCreateContext(mDisplay, lConfig, EGL_NO_CONTEXT, NULL);
          if (mContext == EGL_NO_CONTEXT) goto ERROR;



          Log_debug("Activating the display.");

          // Активируем поверхность для отрисовки

         /*
          *   Приложение может иметь несколько контекстов. Именно по-этому
          *  мы должны связать созданный нами контекст с поверхностью EGL
          *  с помощью ф-и eglMakeCurrent()
          *
          *  С помощью ф-и eglQuerySurface() мы получаем разрешение экрана в пикселях
          *  соответственно по ширене и по высоте
          */

          if (!eglMakeCurrent(mDisplay, mSurface, mSurface, mContext)
           || !eglQuerySurface(mDisplay, mSurface, EGL_WIDTH, &mWidth)
           || !eglQuerySurface(mDisplay, mSurface, EGL_HEIGHT, &mHeight)
           || (mWidth <= 0) || (mHeight <= 0)) goto ERROR;

          // задаем окно отрисовки на полный экран
          glViewport(0, 0, mWidth, mHeight);


          // Выводим в логи информацию об openGl
          Log::info("Viewport : %d x %d", mWidth, mHeight);

          /*
           *   Работа с методами openGl можно применять только после
           *  работы EGL! Следовательно, только с этого момента
           *  загружаем все необходимые текстуры и устанавливаем
           *  начальные настройки openGl.
           */
          mAssets->loadTextures(mApplication);
          setupGlConfigs();

          return STATUS_OK;

          // сюда приходим оператором goto по возникновению ошибок
              ERROR:
                Log::error("Error while starting GraphicsService");
                stop();
                return STATUS_KO;
    }

    void GraphicsService::setupGlConfigs() {
    	glEnable(GL_TEXTURE_2D);
    	glClearColor(0.16078f, 0.15294f, 0.14509f, 0);
    	glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
    }

    void GraphicsService::stop() {
       // Log::info("Stopping GraphicsService.");
        mAssets->disposeTextures(); // освобождаем память выделенную
									// на текстурные объекты

        // Уничтожаем контекст OpenGl
        if (mDisplay != EGL_NO_DISPLAY) {
            eglMakeCurrent(mDisplay,
						   EGL_NO_SURFACE,
						   EGL_NO_SURFACE,
                           EGL_NO_CONTEXT);
            if (mContext != EGL_NO_CONTEXT) {
                eglDestroyContext(mDisplay, mContext);
                mContext = EGL_NO_CONTEXT;
            }
            if (mSurface != EGL_NO_SURFACE) {
                eglDestroySurface(mDisplay, mSurface);
                mSurface = EGL_NO_SURFACE;
            }
            eglTerminate(mDisplay);
            mDisplay = EGL_NO_DISPLAY;
        }
    }

    status GraphicsService::update() {
        // Обновляем отображаемую поверхность
        if (eglSwapBuffers(mDisplay, mSurface) != EGL_TRUE) {
            Log::error("Error %d swapping buffers.", eglGetError());
            return STATUS_KO;
        }
        return STATUS_OK;
    }

    void GraphicsService::updateGlConfigs() {
    	glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
    	glOrthof(0.0f, mWidth, 0.0f, mHeight, 1, 0);
    	glMatrixMode(GL_MODELVIEW);
    	glLoadIdentity();
    }



