#include <Game.h>

Game::Game() : _turn(0), _mode(BOTVBOT), _noKillTurns(0), _simulatedTurns(0) {}

/* Parameterized constructor */
Game::Game(const int mode) : _turn(1), _mode(mode), _noKillTurns(0), _simulatedTurns(0) {
	// Initialize both white and black players
	_players[BLACK].init(BLACK);
	_players[WHITE].init(WHITE);

	// Initialize game state
	initState();
}

Game& Game::operator=(const Game &other) {
	/*
	 * Assignment operator to create a simulation-oriented copy.
	 * Returns pointer to modified object.
	 */

	_turn = other._turn;
	_mode = BOTVBOT;
	_noKillTurns = other._noKillTurns;
	_simulatedTurns = other._simulatedTurns + 1;

	_players[0] = other._players[0];
	_players[1] = other._players[1];

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			_state[i][j] = other._state[i][j];
		}
	}

	return *this;
}

void Game::init(const int mode) {
	/*
	 * Initialize game object.
	 * int mode: game mode to be played (BOTVBOT, HUMANVBOT, HUMANVHUMAN)
	 */

	// Initialize turn and mode
	_turn = 1;
	_mode = mode;
	_noKillTurns = 0;

	// Initialize both white and black players
	_players[BLACK].init(BLACK);
	_players[WHITE].init(WHITE);

	// Initialize game state
	initState();
}

void Game::initState() {
	/*
	 * Initializes game state.
	 */

	// Initialize black pieces
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 8; j++) {
			if (i == 1) {
				_state[i][j].init(Piece::ALIVE, BLACK, PAWN);
			} else if (j == 0 || j == 7) {
				_state[i][j].init(Piece::ALIVE, BLACK, ROOK);
			} else if (j == 1 || j == 6) {
				_state[i][j].init(Piece::ALIVE, BLACK, KNIGHT);
			} else if (j == 2 || j == 5) {
				_state[i][j].init(Piece::ALIVE, BLACK, BISHOP);
			} else if (j == 3) {
				_state[i][j].init(Piece::ALIVE, BLACK, QUEEN);
			} else if (j == 4) {
				_state[i][j].init(Piece::ALIVE, BLACK, KING);
			}
		}
	}

	// Initialize empty cells
	for (int i = 2; i < 6; i++) {
		for (int j = 0; j < 8; j++) {
			_state[i][j].init(Piece::DEAD);
		}
	}

	// Initialize white pieces
	for (int i = 6; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (i == 6) {
				_state[i][j].init(Piece::ALIVE, WHITE, PAWN);
			} else if (j == 0 || j == 7) {
				_state[i][j].init(Piece::ALIVE, WHITE, ROOK);
			} else if (j == 1 || j == 6) {
				_state[i][j].init(Piece::ALIVE, WHITE, KNIGHT);
			} else if (j == 2 || j == 5) {
				_state[i][j].init(Piece::ALIVE, WHITE, BISHOP);
			} else if (j == 3) {
				_state[i][j].init(Piece::ALIVE, WHITE, QUEEN);
			} else if (j == 4) {
				_state[i][j].init(Piece::ALIVE, WHITE, KING);
			}
		}
	}
}

bool Game::run() {
	/*
	 * Runs a chess game until a king is dead or the noKillTurns counter reaches 50.
	 * Returns true if the game ended successfully, false if the quit button is pressed.
	 */

	Canvas canvas;
	Move move(Position(-1, -1), Position(-1, -1));
	Position click(-1, -1);
	std::vector<Position> positions;
	std::chrono::time_point<std::chrono::system_clock> clickTime;
	bool timing = false;

	// Seed random generator
	srand(time(0));

	// Initialize canvas
	if (!canvas.init()) {
		std::cout << "Error initializing canvas!" << std::endl;
	}

	// Game main loop
	do {
		// Check if "quit" button pressed
		if (!canvas.processEvents(click)) {
			canvas.close();
			return false;
		}

		// Check if it's a human turn
		if (_mode == HUMANVHUMAN || (_mode == HUMANVBOT && _turn % 2 == WHITE)) {
			// Check if there was a mouse click
			if (click != Position(-1, -1)) {
				// Process the mouse click
				if (!processMouseClick(move, click)) {
					getLegalPositions(positions, move.getInitial());
					clickTime = std::chrono::system_clock::now();
					timing = true;
				}
			}
		} else {
			// Make the bot choose a move
			move = std::get<0>(botChoice());
		}

		// Check if the move is conformed
		if (move.getFinal() != Position(-1, -1)) {
			// Check if the game is over
			if (isGameOver(move)) {
				playTurn(move);
				canvas.displayWinner(_noKillTurns == 50, (_turn - 1) % 2);
				canvas.close();
				return true;
			}

			// Perform the move
			playTurn(move);
			move = Move(Position(-1, -1), Position(-1, -1));
		}

		// Check timer to erase hints
		if (timing && (std::chrono::system_clock::now() - clickTime).count() > 500000000) {
			positions.clear();
			positions.shrink_to_fit();
			timing = false;
		}

		// Update canvas to show game state
		canvas.update(toString(), move.getInitial(), positions);

		click.setBoth(-1, -1);

	} while (true);
}

void Game::playTurn(const Move move) {
	/*
	 * Plays the current turn, updating player positions and game state.
	 */

	// Check if a piece was killed and update noKillTurns counter
	if (_state[move.getFinal().getY()][move.getFinal().getX()].isAlive()) {
		// Update victim player positions
		_players[!(_turn % 2)].killPiece(move.getFinal());
		_noKillTurns = 0;
	} else {
		_noKillTurns++;
	}

	// Update moving player positions
	_players[_turn % 2].updatePosition(move);

	// Update game state
	_state[move.getFinal().getY()][move.getFinal().getX()] = _state[move.getInitial().getY()][move.getInitial().getX()];
	_state[move.getInitial().getY()][move.getInitial().getX()].setDead();

	// Check if a pawn must turn into a queen
	if (_state[move.getFinal().getY()][move.getFinal().getX()].getType() == PAWN &&
			(move.getFinal().getY() == 0 || move.getFinal().getY() == 7)) {
		_state[move.getFinal().getY()][move.getFinal().getX()].toQueen();
	}

	_turn++;
}

bool Game::processMouseClick(Move &move, Position click) const {
	/*
	 * Processes a mouse click during a human turn to check validness.
	 */

	// Re-scale click from window to board size
	click.scale(Canvas::CANVAS_WIDTH, 8, Canvas::CANVAS_HEIGHT, 8);

	if (_state[click.getY()][click.getX()].isAlive() && _state[click.getY()][click.getX()].getColor() == _turn % 2) {
		// Active player clicked one of his alive pieces
		move.setInitial(click);
	} else if (move.getInitial() != Position(-1, -1)) {
		if (isLegalMove(Move(move.getInitial(), click))) {
			// Active player clicked on a legal final position
			move.setFinal(click);
		} else {
			// Incorrect final position
			return false;
		}
	}

	return true;
}

std::tuple<Move, int> Game::botChoice() const {
	/*
	 * Chooses the best move to take depending on the score of every single move.
	 * Returns a tuple containing the chosen move and the score associated with it.
	 */

	std::vector<Position> positions;
	std::vector<Move> bestMoves;
	Game simulation;
	int score;
	int maxScore = INT_MIN;

	// Iterate through all active player current positions
	for (auto &initial : _players[_turn % 2].getPositions()) {
		// Get all the legal positions for the specific piece
		getLegalPositions(positions, initial);

		//Iterate through all the legal positions of the piece
		for (unsigned int i = 0; i < positions.size(); i++) {
			if (_state[positions[i].getY()][positions[i].getX()].isAlive()) {
				// Update score if an enemy is killed
				score = _state[positions[i].getY()][positions[i].getX()].getValue();
			} else {
				score = 0;
			}

			// Start a simulation if maximum simulated turns not reached
			if (_simulatedTurns < MAX_SIMULATED_TURNS && !isGameOver(Move(initial, positions[i]))) {
				// Create a copy of the game for simulation purposes
				simulation = *this;

				// Perform the move being evaluated
				simulation.playTurn(Move(initial, positions[i]));

				// Update score with simulation results
				score -= std::get<1>(simulation.botChoice());
			}

			// Update list of best moves and the best score achieved
			if (score > maxScore) {
				bestMoves.clear();
				bestMoves.push_back(Move(initial, positions[i]));
				maxScore = score;
			} else if (score == maxScore) {
				bestMoves.push_back(Move(initial, positions[i]));
			}
		}

		positions.clear();
	}

	// Return a random move from the best moves list
	return {bestMoves[rand() % bestMoves.size()], maxScore};
}

bool Game::isGameOver(const Move move) const {
	/*
	 * Checks if the move would end the game by killing the king or by reaching 50 turns without a kill.
	 * Returns true if the game would end, false if not.
	 * Move move: move to be checked.
	 */

	return (_state[move.getFinal().getY()][move.getFinal().getX()].isAlive() &&
			_state[move.getFinal().getY()][move.getFinal().getX()].getType() == KING) ||
			(!_state[move.getFinal().getY()][move.getFinal().getX()].isAlive() && _noKillTurns == 49);
}

bool Game::isLegalMove(const Move move) const {
	/*
	 * Checks if a move is legal.
	 * Returns true if the move is legal, false if not.
	 * Move move: move to be checked.
	 */

	std::vector<Position> positions;

	// Get list of legal final positions
	getLegalPositions(positions, move.getInitial());

	// Iterate through the legal positions to find the move being checked
	for (unsigned int i = 0; i < positions.size(); i++) {
		if (move.getFinal() == positions[i]) {
			// The move being checked was found
			return true;
		}
	}

	return false;
}

void Game::getLegalPositions(std::vector<Position> &positions, const Position initial) const {
	/*
	 * Gets a list of all legal positions given an initial one.
	 * vector<Position> positions: vector where legal positions will be added to.
	 * Position initial: piece initial position.
	 */

	std::vector<Position> directions;

	// Get a list of legal directions the piece could take
	_state[initial.getY()][initial.getX()].getDirections(directions);

	// Iterate through the legal directions
	for (unsigned int i = 0; i < directions.size(); i++) {
		if (_state[initial.getY()][initial.getX()].getType() == PAWN) {
			// Pawns are treated differently due to their unique movement patterns
			followPawnDirection(positions, initial, directions[i]);
		} else {
			followDirection(positions, initial, directions[i]);
		}
	}
}

void Game::followPawnDirection(std::vector<Position> &positions, const Position initial, const Position direction) const {
	/*
	 * Follows a pawn direction and checks its legality.
	 * vector<Position> positions: vector where legal positions will be added to.
	 * Position initial: pawn initial position.
	 * Position direction: direction being followed.
	 */
	if (initial.getY() + direction.getY() >= 0 && initial.getY() + direction.getY() < 8 &&
			initial.getX() + direction.getX() >= 0 && initial.getX() + direction.getX() < 8) {
		if (direction.getX() == 0) {
			// Straight move
			if (!_state[initial.getY() + direction.getY()][initial.getX()].isAlive()) {
				// Front cell is empty
				positions.push_back(Position(initial.getX(), initial.getY() + direction.getY()));
				if (!_state[initial.getY() + direction.getY()*2][initial.getX()].isAlive() &&
						((_state[initial.getY()][initial.getX()].getColor() == BLACK && initial.getY() == 1) ||
						(_state[initial.getY()][initial.getX()].getColor() == WHITE && initial.getY() == 6))) {
					// It's the first move the pawn would take and the second front cell is empty
					positions.push_back(Position(initial.getX(), initial.getY() + direction.getY()*2));
				}
			}
		} else if (_state[initial.getY() + direction.getY()][initial.getX() + direction.getX()].isAlive() &&
				_state[initial.getY() + direction.getY()][initial.getX() + direction.getX()].getColor() != _turn % 2) {
			// Diagonal move
			// There is an enemy piece at the final position
			positions.push_back(Position(initial.getX() + direction.getX(), initial.getY() + direction.getY()));
		}
	}
}

void Game::followDirection(std::vector<Position> &positions, const Position initial, const Position direction) const {
	/*
	 * Follows a piece direction and checks its legality.
	 * vector<Position> positions: vector where legal positions will be added to.
	 * Position initial: piece initial position.
	 * Position direction: direction being followed.
	 */

	int step = 1;

	// Kings and knights will loop only once, the rest will loop until a piece is found or the board ends
	do {
		if (initial.getY() + direction.getY()*step < 0 || initial.getY() + direction.getY()*step >= 8 ||
				initial.getX() + direction.getX()*step < 0 || initial.getX() + direction.getX()*step >= 8) {
			break;
		}
		if (_state[initial.getY() + direction.getY()*step][initial.getX() + direction.getX()*step].isAlive()) {
			// A piece is found on the way
			if (_state[initial.getY() + direction.getY()*step][initial.getX() + direction.getX()*step].getColor() != _turn % 2) {
				// The piece found is an enemy piece
				positions.push_back(Position(initial.getX() + direction.getX()*step, initial.getY() + direction.getY()*step));
			}

			break;
		}

		positions.push_back(Position(initial.getX() + direction.getX()*step, initial.getY() + direction.getY()*step));

		step++;
	} while (_state[initial.getY()][initial.getX()].getType() != KING && _state[initial.getY()][initial.getX()].getType() != KNIGHT);
}

std::string Game::toString() const {
	/*
	 * Returns object's string version.
	 */

	std::string result;

	result = "";

	// Add every piece string in the game state
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			result += _state[i][j].toString();
		}
	}

	return result;
}
