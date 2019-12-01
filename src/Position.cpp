#include "Position.h"

Position::Position() : _x(0), _y(0) {}

/* Parameterized constructor */
Position::Position(const int x, const int y) : _x(x), _y(y) {}

/* Copy constructor */
Position::Position(const Position &other) : _x(other._x), _y(other._y) {}

Position& Position::operator=(const Position &other) {
	/*
	 * Assignment operator.
	 * Returns pointer to modified object.
	 */

	// Member-wise assignment
	_x = other._x;
	_y = other._y;

	return *this;
}

bool Position::operator==(const Position &other) const {
	/*
	 * Checks if two positions are equal.
	 * Returns true if both coordinates are equal, false if any is different.
	 */

	return other._x == _x && other._y == _y;
}

bool Position::operator!=(const Position &other) const {
	/*
	 * Checks if two positions are different.
	 * Returns true if any coordinate is different, false if both are equal.
	 */

	return other._x != _x || other._y != _y;
}

int Position::getX() const {
	/*
	 * Returns x coordinate.
	 */

	return _x;
}

int Position::getY() const {
	/*
	 * Returns y coordinate.
	 */

	return _y;
}

void Position::setX(const int newX) {
	/*
	 * Sets x to its new value.
	 * int newX: new value for x.
	 */

	_x = newX;
}

void Position::setY(const int newY) {
	/*
	 * Sets y to its new value.
	 * int newY: new value for y.
	 */

	_y = newY;
}

void Position::setBoth(const int newX, const int newY) {
	/*
	 * Sets x and y to their new values.
	 * int newX: new value for x.
	 * int newY: new value for y.
	 */

	_x = newX;
	_y = newY;
}

void Position::incrementXYBy(const int toX, const int toY) {
	/*
	 * Increments values of x and y.
	 * int toX: increment to x.
	 * int toY: increment to y.
	 */

	_x += toX;
	_y += toY;
}

void Position::scale(const int currentXScale, const int newXScale, const int currentYScale, const int newYScale) {
	/*
	 * Scales coordinates to a new scale.
	 */

	_x = _x*newXScale/currentXScale;
	_y = _y*newYScale/currentYScale;
}

void Position::swap() {
	/*
	 * Swaps values from x to y and from y to x.
	 */

	int temp;
	temp = _x;
	_x = _y;
	_y = temp;
}

std::string Position::toString() const {
	/*
	 * Returns object's string version.
	 */

	std::string result;

	result = std::to_string(_x) + ", " + std::to_string(_y);

	return result;
}
