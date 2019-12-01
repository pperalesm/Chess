#ifndef CANVAS_H_
#define CANVAS_H_

#include <iostream>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "Position.h"
#include "util.h"

class Canvas {
public:
	const static int CANVAS_WIDTH = 800;
	const static int CANVAS_HEIGHT = 800;
	const static int SPRITE_WIDTH = 50;
	const static int SPRITE_HEIGHT = 50;
	const static int BLUE_SQUARE_INDEX = 12;
	const static int GREEN_SQUARE_INDEX = 13;
	const static int BLACK_WINS_INDEX = 14;
	const static int WHITE_WINS_INDEX = 15;

private:
	SDL_Window *_window;
	SDL_Renderer *_renderer;
	SDL_Texture* _background;
	Uint32 *_buffer;
	std::vector<SDL_Texture*> _spriteTextures;
	SDL_Rect _spriteRect;

public:
	Canvas();
	bool init();
	void close();
	bool processEvents(Position &click) const;
	void update(const std::string boardString, const Position initialPosition, const std::vector<Position> finalPositions);
	void initBackgroundBuffer();
	void displayWinner(const bool draw, const bool winner);
};

#endif /* CANVAS_H_ */
