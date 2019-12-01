#ifndef MOVE_H_
#define MOVE_H_

#include "Position.h"

class Move {
private:
	Position _initial;
	Position _final;

public:
	Move();
	Move(const Position initial, const Position final);
	Move(const Move &other);
	Move& operator=(const Move &other);
	bool operator==(const Move &other) const;
	bool operator!=(const Move &other) const;
	Position getInitial() const;
	Position getFinal() const;
	void setInitial(const Position newInitial);
	void setFinal(const Position newFinal);
	void setBoth(const Position newInitial, const Position newFinal);
	std::string toString() const;
};

#endif /* MOVE_H_ */
