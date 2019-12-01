#include "Canvas.h"

Canvas::Canvas() : _window(NULL), _renderer(NULL), _background(NULL), _buffer(NULL) {}

bool Canvas::init() {
	/*
	 * Initialize whole canvas.
	 * Returns true if successful, false if not.
	 */

	// Try to initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		return false;
	}

	// Initialize window and check if successful
	_window = SDL_CreateWindow("CHESS", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, CANVAS_WIDTH, CANVAS_HEIGHT, SDL_WINDOW_SHOWN);
	if (_window == NULL) {
		return false;
	}

	// Initialize renderer and check if successful
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_PRESENTVSYNC);
	if (_renderer == NULL) {
		return false;
	}

	// Initialize background and check if successful
	_background = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, CANVAS_WIDTH, CANVAS_HEIGHT);
	if (_background == NULL) {
		return false;
	}

	// Initialize sprite textures
	IMG_Init(IMG_INIT_PNG);

	_spriteTextures.push_back(IMG_LoadTexture(_renderer, "blackBishop.png"));
	_spriteTextures.push_back(IMG_LoadTexture(_renderer, "blackKing.png"));
	_spriteTextures.push_back(IMG_LoadTexture(_renderer, "blackKnight.png"));
	_spriteTextures.push_back(IMG_LoadTexture(_renderer, "blackPawn.png"));
	_spriteTextures.push_back(IMG_LoadTexture(_renderer, "blackQueen.png"));
	_spriteTextures.push_back(IMG_LoadTexture(_renderer, "blackRook.png"));
	_spriteTextures.push_back(IMG_LoadTexture(_renderer, "whiteBishop.png"));
	_spriteTextures.push_back(IMG_LoadTexture(_renderer, "whiteKing.png"));
	_spriteTextures.push_back(IMG_LoadTexture(_renderer, "whiteKnight.png"));
	_spriteTextures.push_back(IMG_LoadTexture(_renderer, "whitePawn.png"));
	_spriteTextures.push_back(IMG_LoadTexture(_renderer, "whiteQueen.png"));
	_spriteTextures.push_back(IMG_LoadTexture(_renderer, "whiteRook.png"));
	_spriteTextures.push_back(IMG_LoadTexture(_renderer, "blueSquare.png"));
	_spriteTextures.push_back(IMG_LoadTexture(_renderer, "greenSquare.png"));
	_spriteTextures.push_back(IMG_LoadTexture(_renderer, "blackWins.png"));
	_spriteTextures.push_back(IMG_LoadTexture(_renderer, "whiteWins.png"));


	// Initialize buffer and copy to background
	_buffer = new Uint32[CANVAS_WIDTH*CANVAS_HEIGHT];
	initBackgroundBuffer();
	SDL_UpdateTexture(_background, NULL, _buffer, CANVAS_WIDTH*sizeof(Uint32));

	// Initialize spriteRect
	_spriteRect.w = CANVAS_WIDTH/8;
	_spriteRect.h = CANVAS_HEIGHT/8;


	return true;
}

void Canvas::close() {
	/*
	 * Free all memory allocated for canvas.
	 */

	for (auto texture : _spriteTextures) {
		// Check if sprite textures already deallocated to free them
		if (texture != NULL) {
			SDL_DestroyTexture(texture);
		}
	}
	// Free memory allocated for sprite textures' vector
	_spriteTextures.clear();
	_spriteTextures.shrink_to_fit();

	// Check if background already deallocated to free it
	if (_background != NULL) {
		SDL_DestroyTexture(_background);
	}

	// Check if renderer already deallocated to free it
	if (_renderer != NULL) {
		SDL_DestroyRenderer(_renderer);
	}

	// Check if renderer already deallocated to free it
	if (_window != NULL) {
		SDL_DestroyWindow(_window);
	}

	// Quit IMG
	IMG_Quit();

	// Quit SDL
	SDL_Quit();
	delete [] _buffer;
}

bool Canvas::processEvents(Position &click) const {
	/*
	 * Process events within canvas.
	 * Returns false if "Quit" button is clicked, true if not.
	 */

	SDL_Event event;

	// Check if events in queue
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			return false;
		} else if (event.type == SDL_MOUSEBUTTONUP) {
			click.setBoth(event.button.x, event.button.y);
		}
	}

	return true;
}

void Canvas::update(const std::string boardString, const Position initialPosition, const std::vector<Position> finalPositions) {
	/*
	 * Updates drawings in canvas.
	 * string boardString: Current board disposition.
	 */

	int textureIndex;

	// Clear renderer
	SDL_RenderClear(_renderer);

	// Copy background to renderer
	SDL_RenderCopy(_renderer, _background, NULL, NULL);

	// Copy initial position square to renderer if needed
	if (initialPosition != Position(-1, -1)) {
		_spriteRect.x = initialPosition.getX()*CANVAS_WIDTH/8;
		_spriteRect.y = initialPosition.getY()*CANVAS_HEIGHT/8;
		SDL_RenderCopy(_renderer, _spriteTextures[BLUE_SQUARE_INDEX], NULL, &_spriteRect);
	}

	// Copy final position squares to renderer if needed
	for (unsigned int i = 0; i < finalPositions.size(); i++) {
		_spriteRect.x = finalPositions[i].getX()*CANVAS_WIDTH/8;
		_spriteRect.y = finalPositions[i].getY()*CANVAS_HEIGHT/8;
		SDL_RenderCopy(_renderer, _spriteTextures[GREEN_SQUARE_INDEX], NULL, &_spriteRect);
	}

	// Iterate through boardString
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			// Set piece position in canvas
			_spriteRect.x = j*CANVAS_WIDTH/8;
			_spriteRect.y = i*CANVAS_HEIGHT/8;

			// Read piece type from boardString
			textureIndex = boardString[2*j + 16*i + 1] - '0';

			// Read piece color from boardString and copy sprite texture to renderer
			if (boardString[2*j + 16*i] == WHITE + '0') {
				textureIndex += 6;
				SDL_RenderCopy(_renderer, _spriteTextures[textureIndex], NULL, &_spriteRect);
			} else if (boardString[2*j + 16*i] == BLACK + '0') {
				SDL_RenderCopy(_renderer, _spriteTextures[textureIndex], NULL, &_spriteRect);
			}
		}
	}

	// Draw renderer into the window
	SDL_RenderPresent(_renderer);
}

void Canvas::initBackgroundBuffer() {
	/*
	 * Sets buffer values to match a chess board colors
	 */

	// Initialize white color
	Uint32 color = 0xFFFFFFFF;

	// Set the default color to gray
	memset(_buffer, 50, CANVAS_WIDTH*CANVAS_HEIGHT*sizeof(Uint32));

	// Draw white cells to buffer
	for (int y = 0; y < CANVAS_HEIGHT/8; y++) {
		// Row 1
		memset(_buffer + y*CANVAS_WIDTH, color, sizeof(Uint32)*CANVAS_WIDTH/8);
		memset(_buffer + y*CANVAS_WIDTH + CANVAS_WIDTH/4, color, sizeof(Uint32)*CANVAS_WIDTH/8);
		memset(_buffer + y*CANVAS_WIDTH + CANVAS_WIDTH/2, color, sizeof(Uint32)*CANVAS_WIDTH/8);
		memset(_buffer + y*CANVAS_WIDTH + 3*CANVAS_WIDTH/4, color, sizeof(Uint32)*CANVAS_WIDTH/8);

		// Row 2
		memset(_buffer + y*CANVAS_WIDTH + CANVAS_WIDTH*CANVAS_HEIGHT/8 + CANVAS_WIDTH/8, color, sizeof(Uint32)*CANVAS_WIDTH/8);
		memset(_buffer + y*CANVAS_WIDTH + CANVAS_WIDTH*CANVAS_HEIGHT/8 + CANVAS_WIDTH/4 + CANVAS_WIDTH/8, color, sizeof(Uint32)*CANVAS_WIDTH/8);
		memset(_buffer + y*CANVAS_WIDTH + CANVAS_WIDTH*CANVAS_HEIGHT/8 + CANVAS_WIDTH/2 + CANVAS_WIDTH/8, color, sizeof(Uint32)*CANVAS_WIDTH/8);
		memset(_buffer + y*CANVAS_WIDTH + CANVAS_WIDTH*CANVAS_HEIGHT/8 + 3*CANVAS_WIDTH/4 + CANVAS_WIDTH/8, color, sizeof(Uint32)*CANVAS_WIDTH/8);

		// Row 3
		memset(_buffer + y*CANVAS_WIDTH + CANVAS_WIDTH*CANVAS_HEIGHT/4, color, sizeof(Uint32)*CANVAS_WIDTH/8);
		memset(_buffer + y*CANVAS_WIDTH + CANVAS_WIDTH*CANVAS_HEIGHT/4 + CANVAS_WIDTH/4, color, sizeof(Uint32)*CANVAS_WIDTH/8);
		memset(_buffer + y*CANVAS_WIDTH + CANVAS_WIDTH*CANVAS_HEIGHT/4 + CANVAS_WIDTH/2, color, sizeof(Uint32)*CANVAS_WIDTH/8);
		memset(_buffer + y*CANVAS_WIDTH + CANVAS_WIDTH*CANVAS_HEIGHT/4 + 3*CANVAS_WIDTH/4, color, sizeof(Uint32)*CANVAS_WIDTH/8);

		// Row 4
		memset(_buffer + y*CANVAS_WIDTH + 3*CANVAS_WIDTH*CANVAS_HEIGHT/8 + CANVAS_WIDTH/8, color, sizeof(Uint32)*CANVAS_WIDTH/8);
		memset(_buffer + y*CANVAS_WIDTH + 3*CANVAS_WIDTH*CANVAS_HEIGHT/8 + CANVAS_WIDTH/4 + CANVAS_WIDTH/8, color, sizeof(Uint32)*CANVAS_WIDTH/8);
		memset(_buffer + y*CANVAS_WIDTH + 3*CANVAS_WIDTH*CANVAS_HEIGHT/8 + CANVAS_WIDTH/2 + CANVAS_WIDTH/8, color, sizeof(Uint32)*CANVAS_WIDTH/8);
		memset(_buffer + y*CANVAS_WIDTH + 3*CANVAS_WIDTH*CANVAS_HEIGHT/8 + 3*CANVAS_WIDTH/4 + CANVAS_WIDTH/8, color, sizeof(Uint32)*CANVAS_WIDTH/8);

		// Row 5
		memset(_buffer + y*CANVAS_WIDTH + CANVAS_WIDTH*CANVAS_HEIGHT/2, color, sizeof(Uint32)*CANVAS_WIDTH/8);
		memset(_buffer + y*CANVAS_WIDTH + CANVAS_WIDTH*CANVAS_HEIGHT/2 + CANVAS_WIDTH/4, color, sizeof(Uint32)*CANVAS_WIDTH/8);
		memset(_buffer + y*CANVAS_WIDTH + CANVAS_WIDTH*CANVAS_HEIGHT/2 + CANVAS_WIDTH/2, color, sizeof(Uint32)*CANVAS_WIDTH/8);
		memset(_buffer + y*CANVAS_WIDTH + CANVAS_WIDTH*CANVAS_HEIGHT/2 + 3*CANVAS_WIDTH/4, color, sizeof(Uint32)*CANVAS_WIDTH/8);

		// Row 6
		memset(_buffer + y*CANVAS_WIDTH + 5*CANVAS_WIDTH*CANVAS_HEIGHT/8 + CANVAS_WIDTH/8, color, sizeof(Uint32)*CANVAS_WIDTH/8);
		memset(_buffer + y*CANVAS_WIDTH + 5*CANVAS_WIDTH*CANVAS_HEIGHT/8 + CANVAS_WIDTH/4 + CANVAS_WIDTH/8, color, sizeof(Uint32)*CANVAS_WIDTH/8);
		memset(_buffer + y*CANVAS_WIDTH + 5*CANVAS_WIDTH*CANVAS_HEIGHT/8 + CANVAS_WIDTH/2 + CANVAS_WIDTH/8, color, sizeof(Uint32)*CANVAS_WIDTH/8);
		memset(_buffer + y*CANVAS_WIDTH + 5*CANVAS_WIDTH*CANVAS_HEIGHT/8 + 3*CANVAS_WIDTH/4 + CANVAS_WIDTH/8, color, sizeof(Uint32)*CANVAS_WIDTH/8);

		// Row 7
		memset(_buffer + y*CANVAS_WIDTH + 3*CANVAS_WIDTH*CANVAS_HEIGHT/4, color, sizeof(Uint32)*CANVAS_WIDTH/8);
		memset(_buffer + y*CANVAS_WIDTH + 3*CANVAS_WIDTH*CANVAS_HEIGHT/4 + CANVAS_WIDTH/4, color, sizeof(Uint32)*CANVAS_WIDTH/8);
		memset(_buffer + y*CANVAS_WIDTH + 3*CANVAS_WIDTH*CANVAS_HEIGHT/4 + CANVAS_WIDTH/2, color, sizeof(Uint32)*CANVAS_WIDTH/8);
		memset(_buffer + y*CANVAS_WIDTH + 3*CANVAS_WIDTH*CANVAS_HEIGHT/4 + 3*CANVAS_WIDTH/4, color, sizeof(Uint32)*CANVAS_WIDTH/8);

		// Row 8
		memset(_buffer + y*CANVAS_WIDTH + 7*CANVAS_WIDTH*CANVAS_HEIGHT/8 + CANVAS_WIDTH/8, color, sizeof(Uint32)*CANVAS_WIDTH/8);
		memset(_buffer + y*CANVAS_WIDTH + 7*CANVAS_WIDTH*CANVAS_HEIGHT/8 + CANVAS_WIDTH/4 + CANVAS_WIDTH/8, color, sizeof(Uint32)*CANVAS_WIDTH/8);
		memset(_buffer + y*CANVAS_WIDTH + 7*CANVAS_WIDTH*CANVAS_HEIGHT/8 + CANVAS_WIDTH/2 + CANVAS_WIDTH/8, color, sizeof(Uint32)*CANVAS_WIDTH/8);
		memset(_buffer + y*CANVAS_WIDTH + 7*CANVAS_WIDTH*CANVAS_HEIGHT/8 + 3*CANVAS_WIDTH/4 + CANVAS_WIDTH/8, color, sizeof(Uint32)*CANVAS_WIDTH/8);
	}
}

void Canvas::displayWinner(const bool draw, const bool winner) {
	/*
	 * Displays game over screen.
	 * bool draw: true if there was a draw.
	 * bool winner: winner of the game.
	 */

	if (!draw) {
		_spriteRect.x = CANVAS_WIDTH/2 - 375;
		_spriteRect.y = CANVAS_HEIGHT/2 - 100;
		_spriteRect.w = 750;
		_spriteRect.h = 200;
		if (winner == BLACK) {
			SDL_RenderCopy(_renderer, _spriteTextures[BLACK_WINS_INDEX], NULL, &_spriteRect);
		} else {
			SDL_RenderCopy(_renderer, _spriteTextures[WHITE_WINS_INDEX], NULL, &_spriteRect);
		}

		_spriteRect.w = 50;
		_spriteRect.h = 50;
	}

	SDL_RenderPresent(_renderer);

	SDL_Delay(1000);
}
