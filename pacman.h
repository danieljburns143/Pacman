// pacman.h
// Daniel Burns / Sean Howley

#ifndef PACMAN_H
#define PACMAN_H

class Pacman {
public:
	Pacman(); // default constructor
	Pacman(int); // constructor with argument to specify ghost
	~Pacman(); // destructor
	double getXPos(); // prototype to return pacman's x coordinate
	double getYPos(); // prototype to return pacman's y coordinate
	double getXCenter(); // prototype to return pacman's x center
	double getYCenter(); // prototype to return pacman's y center
	double getXVel(); // prototype to return pacman's x velocity
	double getYVel(); // prototype to return pacman's y velocity
	int getRow(); // prototype to return pacman's row
	int getColumn(); // prototype to return pacman's column
	int getOrientation(); // prototype to return pacman's orientation
	int getAnimation(); // prototype to return pacman's animation
	void setAnimation(int); // prototype to return pacman's animation
	void setRow(double); // prototype to set pacman's row
	void setColumn(double); // prototype to set pacman's column
	void setXVel(double); // prototype to set pacman's x velocity
	void setYVel(double); // prototype to set pacman's y velocity
	void setXPos(double); // prototype to set pacman's x coordinate
	void setYPos(double); // prototype to set pacman's y coordinate
	void setXCenter(double); // prototype to set pacman's x center
	void setYCenter(double); // prototype to set pacman's y center
	void drawPacman(double, double, int, int); // prototype to draw pacman
	void correctPath(); // prototype to correct the character's path
	int getTimesCollided(); // prototype to return how many times a ghost has collided
	void incrementTimesCollided(); // prototype to return how many times a ghost has collided
	void resetTimesCollided(); // prototype to reset the ghosts stats when pacman dies
	void outOfGhostHouse(); // prototype to set ghosthouse = 0
	bool getGhostHouse(); // prototype to return if ghost is in ghosthouse or not
	void backInGhostHouse(); // prorotype to set ghosthouse = 1
	int getCharacter(); // return which character you are referring to
	int getLastTurnXCenter(); // return x center of last ghost turn (different direction)
	int getLastTurnYCenter(); // return y center of last ghost turn (different direction)
	void setLastTurnXCenter(int); // set x center of last ghost turn
	void setLastTurnYCenter(int); // set y center of last ghost turn
private:
	double xvel;
	double yvel;
	double xpos;
	double ypos;
	double xcenter;
	double ycenter;
	int row;
	int column;
	int orientation;
	int animation;
	int character;
	int timescollided;
	int lastturnxcenter;
	int lastturnycenter;
	bool ghosthouse{1};
};

#endif
