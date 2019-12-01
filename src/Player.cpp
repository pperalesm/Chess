#include "Player.h"

Player::Player() : _color(BLACK), _score(-1) {}

/* Parameterized constructor */
Player::Player(const bool color) : _color(color), _score(0) {
	// Reserve memory for 16 positions in vector
	_positions.reserve(16);

	// Initialize 16 positions depending on color
	if (color == WHITE) {
		for (int i = 6; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				_positions.push_back(Position(j, i));
			}
		}
	} else {
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 8; j++) {
				_positions.push_back(Position(j, i));
			}
		}
	}
}

/* Copy constructor */
Player::Player(const Player &other) : _color(other._color), _score(other._score), _positions(other._positions) {}

Player& Player::operator=(const Player &other) {
	/*
	 * Assignment operator.
	 * Returns pointer to modified object.
	 */

	// Member-wise assignment
	_color = other._color;
	_score = other._score;
	_positions = other._positions;

	return *this;
}

void Player::init(const bool color) {
	/*
	 * Initialize a player object.
	 */

	// Initialize color and score
	_color = color;
	_score = 0;

	// Reserve memory for 16 positions in vector
	_positions.clear();
	_positions.reserve(16);

	// Initialize 16 positions depending on color
	if (color == WHITE) {
		for (int i = 6; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				_positions.push_back(Position(j, i));
			}
		}
	} else {
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 8; j++) {
				_positions.push_back(Position(j, i));
			}
		}
	}
}

std::vector<Position> Player::getPositions() const {
	/*
	 * Returns vector of positions.
	 */

	return _positions;
}

void Player::killPiece(const Position position) {
	for (unsigned int i = 0; i < _positions.size(); i++) {
		if (_positions[i] == position) {
			_positions.erase(_positions.begin() + i);
			_positions.shrink_to_fit();
			break;
		}
	}
}

void Player::updatePosition(const Move move) {
	for (unsigned int i = 0; i < _positions.size(); i++) {
		if (_positions[i] == move.getInitial()) {
			_positions[i] = move.getFinal();
			break;
		}
	}
}

std::string Player::toString() const {
	/*
	 * Returns object's string version.
	 */

	// Add color and score to the result string
	std::string result = "Color: ";

	if (_color == WHITE) {
		result += "White\n";
	} else {
		result += "Black\n";
	}

	result += "Score: " + std::to_string(_score) + "\n";

	// Add each position to the result string, except the last one
	for (unsigned int i = 0; i < _positions.size() - 1; i++) {
		result += "(" + _positions[i].toString() + ") - ";
	}

	// Add last position to the result string
	result += "(" + _positions[_positions.size() - 1].toString() + ")";

	return result;
}
