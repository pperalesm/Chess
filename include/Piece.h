#ifndef PIECE_H_
#define PIECE_H_

#include <string>
#include <vector>
#include "Position.h"
#include "util.h"

class Piece {
public:
	const static bool DEAD = 0;
	const static bool ALIVE = 1;
	const static bool KING_VALUE = 1000;
	const static bool QUEEN_VALUE = 96;
	const static bool ROOK_VALUE = 52;
	const static bool BISHOP_VALUE = 33;
	const static bool KNIGHT_VALUE = 32;
	const static bool PAWN_VALUE = 10;

private:
	bool _alive;
	bool _color;
	int _type;
	int _value;

public:
	Piece();
	Piece(const bool alive, const bool color, const int type);
	Piece& operator=(const Piece &other);
	void init(const bool alive, const bool color = BLACK, const int type = PAWN);
	int getType() const;
	bool getColor() const;
	int getValue() const;
	bool isAlive() const;
	void setDead();
	void getDirections(std::vector<Position> &directions) const;
	void assignValue();
	void toQueen();
	std::string toString() const;
};

#endif /* PIECE_H_ */
