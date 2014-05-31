#ifndef _CELL_HPP_
#define _CELL_HPP_

#include "Types.hpp"
#include "Vector2D.hpp"

	/*
	 * Cell - игровая ячейка
	 */
	class Cell {
	public:
		static const int32_t STATE_EMPTY = 0;
		static const int32_t STATE_FILLED_PACKMAN = 1;
		static const int32_t STATE_FILLED_MONSTER = 2;
		static const int32_t STATE_UNREACHABLE = 3;
		int32_t mState;
		Vector2D mCoords;
	};

#endif
