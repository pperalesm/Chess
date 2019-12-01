#ifndef CHESSGAME_H_
#define CHESSGAME_H_

#include <string>
#include <limits.h>
#include <tuple>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include "Player.h"
#include "Piece.h"
#include "Canvas.h"
#include "Move.h"
#include "util.h"

class Game {
public:
	const static int BOTVBOT = 0;
	const static int HUMANVBOT = 1;
	const static int HUMANVHUMAN = 2;
	const static int MAX_SIMULATED_TURNS = 3;

private:
	int _turn;
	int _mode;
	int _noKillTurns;
	int _simulatedTurns;
	Player _players[2];
	Piece _state[8][8];

public:
	Game();
	Game(const int mode);
	Game& operator=(const Game &other);
	void init(const int mode);
	void initState();
	bool run();
	void playTurn(const Move move);
	bool processMouseClick(Move &move, Position click) const;
	std::tuple<Move, int> botChoice() const;
	bool isGameOver(const Move move) const;
	bool isLegalMove(const Move move) const;
	void getLegalPositions(std::vector<Position> &positions, const Position initial) const;
	void followPawnDirection(std::vector<Position> &positions, const Position initial, const Position direction) const;
	void followDirection(std::vector<Position> &positions, const Position initial, const Position direction) const;
	std::string toString() const;
};

#endif /* CHESSGAME_H_ */
