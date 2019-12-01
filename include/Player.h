#ifndef PLAYER_H_
#define PLAYER_H_

#include <vector>
#include "Position.h"
#include "Move.h"
#include "util.h"

class Player {
private:
	bool _color;
	int _score;
	std::vector<Position> _positions;

public:
	Player();
	Player(const bool color);
	Player(const Player &other);
	Player& operator=(const Player &other);
	void init(const bool color);
	std::vector<Position> getPositions() const;
	void killPiece(const Position position);
	void updatePosition(const Move move);
	std::string toString() const;
};

#endif /* PLAYER_H_ */
