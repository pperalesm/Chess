#include "Piece.h"

Piece::Piece() : _alive(DEAD), _color(BLACK), _type(0), _value(0) {}

/* Parameterized constructor */
Piece::Piece(const bool alive, const bool color, const int type) : _alive(alive), _color(color), _type(type) {

	// Assign value depending on the piece type
	assignValue();
}

Piece& Piece::operator=(const Piece &other) {
	/*
	 * Assignment operator.
	 * Returns pointer to modified object.
	 */

	// Member-wise assignment
	_alive = other._alive;
	_color = other._color;
	_type = other._type;
	_value = other._value;

	return *this;
}

void Piece::init(const bool alive, const bool color, const int type) {
	_alive = alive;
	_color = color;
	_type = type;

	// Assign value depending on the piece type
	assignValue();
}

int Piece::getType() const {
	/*
	 * Returns the piece type.
	 */

	return _type;
}

bool Piece::getColor() const {
	/*
	 * Returns the piece color.
	 */

	return _color;
}

int Piece::getValue() const {
	/*
	 * Returns the piece value.
	 */

	return _value;
}

bool Piece::isAlive() const {
	/*
	 * Returns true if alive, false if dead.
	 */

	return _alive;
}

void Piece::setDead() {
	/*
	 * Sets the piece to be dead.
	 */

	_alive = false;
}

void Piece::getDirections(std::vector<Position> &directions) const {
	/*
	 * Gets the legal directions that the piece can take.
	 * vector<Position> directions: vector in which the directions will be saved.
	 */

	if (_type == KNIGHT) {
		// Add knight directions
		directions.push_back(Position(2, 1));
		directions.push_back(Position(2, -1));
		directions.push_back(Position(1, 2));
		directions.push_back(Position(1, -2));
		directions.push_back(Position(-2, 1));
		directions.push_back(Position(-2, -1));
		directions.push_back(Position(-1, 2));
		directions.push_back(Position(-1, -2));
	} else {
		if (_type != BISHOP) {
			// Add straight directions (if pawn, only add front directions)
			if (_type != PAWN || _color == BLACK) {
				directions.push_back(Position(0, 1));
			}

			if (_type != PAWN || _color == WHITE) {
				directions.push_back(Position(0, -1));
			}

			if (_type != PAWN) {
				directions.push_back(Position(1, 0));
				directions.push_back(Position(-1, 0));
			}
		}

		if (_type != ROOK) {
			// Add diagonal directions (if pawn, only add front directions)
			if (_type != PAWN || _color == BLACK) {
				directions.push_back(Position(1, 1));
				directions.push_back(Position(-1, 1));
			}

			if (_type != PAWN || _color == WHITE) {
				directions.push_back(Position(1, -1));
				directions.push_back(Position(-1, -1));
			}
		}
	}
}

void Piece::assignValue() {
	/*
	 * Assigns piece value depending on the type.
	 */

	if (_type == PAWN) {
		_value = PAWN_VALUE;
	} else if (_type == KNIGHT) {
		_value = KNIGHT_VALUE;
	} else if (_type == BISHOP) {
		_value = BISHOP_VALUE;
	} else if (_type == ROOK) {
		_value = ROOK_VALUE;
	} else if (_type == QUEEN) {
		_value = QUEEN_VALUE;
	} else if (_type == KING) {
		_value = KING_VALUE;
	}
}

void Piece::toQueen() {
	/*
	 * Turns the piece into a queen.
	 */

	_type = QUEEN;
	_value = QUEEN_VALUE;
}

std::string Piece::toString() const {
	/*
	 * Returns object's string version.
	 */

	std::string result;

	if (_alive) {
		result = std::to_string(_color);
	} else {
		result = "2";
	}

	result += std::to_string(_type);

	return result;
}
