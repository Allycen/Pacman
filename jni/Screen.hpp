#ifndef _SCREEN_HPP_
#define _SCREEN_HPP_

#include "GraphicsService.hpp"
#include "TimeService.hpp"
#include "InputService.hpp"
#include "SoundService.hpp"
#include "Types.hpp"
#include "ActivityHandler.hpp"



	// Абстрактный класс
	class Screen {
	public:
		virtual void init(ActivityHandler* pPackmanActivity,
						  GraphicsService* pGraphicsService,
						  TimeService* pTimeService,
						  InputService* pInputService,
						  SoundService* pSoundService) = 0;

	    virtual void update(float pDeltaTime) = 0;
	    virtual void present() = 0;

	    virtual void setConfigs() = 0;

	};

#endif

	// В игре есть определенное количество экранов:
	// 1) главное меню
	// 2) рекорды,
	// 3) экран, в котором происходит игровой процесс.
	// 4) ...
	//
	//  Все они отображают разную информацию
	// и имеют различные состояния, но объединены некоторыми
	// свойствами ( обновление, представление, реакция на паузу
	// и т.д)
