#ifndef POSITION_H_
#define POSITION_H_

#include <string>

class Position {
private:
	int _x;
	int _y;

public:
	Position();
	Position(const int x, const int y);
	Position(const Position &other);
	Position& operator=(const Position &other);
	bool operator==(const Position &other) const;
	bool operator!=(const Position &other) const;
	int getX() const;
	int getY() const;
	void setX(const int newX);
	void setY(const int newY);
	void setBoth(const int newX, const int newY);
	void incrementXYBy(const int toX, const int toY);
	void scale(const int currentXScale, const int newXScale, const int currentYScale, const int newYScale);
	void swap();
	std::string toString() const;
};

#endif /* POSITION_H_ */
