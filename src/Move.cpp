#include "Move.h"

Move::Move() {}

/* Parameterized constructor */
Move::Move(const Position initial, const Position final) : _initial(initial), _final(final) {}

/* Copy constructor */
Move::Move(const Move &other) : _initial(other._initial), _final(other._final) {}

Move& Move::operator=(const Move &other) {
	/*
	 * Assignment operator.
	 * Returns pointer to modified object.
	 */

	// Member-wise assignment
	_initial = other._initial;
	_final = other._final;

	return *this;
}

bool Move::operator==(const Move &other) const {
	/*
	 * Checks if two moves are equal.
	 * Returns true if both initial and final positions are equal, false if any is different.
	 */

	return other._initial == _initial && other._final == _final;
}

bool Move::operator!=(const Move &other) const {
	/*
	 * Checks if two moves are different.
	 * Returns true if any position is different, false if both are equal.
	 */

	return other._initial != _initial || other._final != _final;
}

Position Move::getInitial() const {
	/*
	 * Returns initial position.
	 */

	return _initial;
}

Position Move::getFinal() const {
	/*
	 * Returns final position.
	 */

	return _final;
}

void Move::setInitial(const Position newInitial) {
	/*
	 * Sets initial position to its new values.
	 * Position newInitial: new coordinates for initial position.
	 */

	_initial = newInitial;
}

void Move::setFinal(const Position newFinal) {
	/*
	 * Sets final position to its new values.
	 * Position newFinal: new coordinates for final position.
	 */

	_final = newFinal;
}

void Move::setBoth(const Position newInitial, const Position newFinal) {
	/*
	 * Sets initial and final positions to their new values.
	 * Position newInitial: new coordinates for initial position.
	 * Position newFinal: new coordinates for final position.
	 */

	_initial = newInitial;
	_final = newFinal;
}

std::string Move::toString() const {
	/*
	 * Returns object's string version.
	 */

	std::string result;

	result = "(" + _initial.toString() + ") -> (" + _final.toString() + ")";

	return result;
}
