#ifndef _WORLDRENDERER_HPP_
#define _WORLDRENDERER_HPP_

#include "SpriteBatcher.hpp"
#include "Camera2D.hpp"
#include "GraphicsService.hpp"
#include "World.hpp"

#include <stdio.h>

	/*
	 *   WorldRenderer - класс, объект которого ответственен за отрисовку игрового мира
	 */

	class WorldRenderer {
	public:
		float FRUSTUM_WIDTH;
		float FRUSTUM_HEIGHT;
		SpriteBatcher mSpriteBatcher;
		Camera2D mCamera;
		GraphicsService* mGraphicsService;
		World* mWorld;


		WorldRenderer (float pFrustumWidth, float pFrustumHeight, World* pWorld, GraphicsService* pGraphicsService);
		 ~WorldRenderer();
		void render();
	private:
		void renderBackground();
		void renderLabyrinth();
		void renderPacman();
		void renderMonsters();
		void renderStars();
		void renderCandies();
		void renderTime();
		void renderScore();
		void renderPacmanLives();
		void renderTimeIsUp();
		void renderGameOver();
		void renderFinalScore();


	};

#endif
