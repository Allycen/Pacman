#include "PacmanActivity.hpp"
#include "EventLoop.hpp"
#include "GraphicsService.hpp"
#include "TimeService.hpp"
#include "SoundService.hpp"
#include "InputService.hpp"

	/*
	 *   android_main - точка входа в программу. Здесь создаются необходимые
	 *  объекты, поддерживающие определенные сервисы, Activity, которое в
	 *  дальнейшем будем обрабатываться в цикле событий.
	 */

void android_main(android_app* pApplication) {

    TimeService lTimeService;
    GraphicsService lGraphicsService(pApplication);

    InputService lInputService(pApplication,
							   lGraphicsService.getWidth(),
							   lGraphicsService.getHeight());
    SoundService lSoundService(pApplication);


    EventLoop lEventLoop(pApplication);

    PacmanActivity lPacmanActivity( pApplication,
									&lGraphicsService,
									&lTimeService,
									&lSoundService,
									&lInputService);

    lEventLoop.run(&lPacmanActivity, &lInputService);

}
