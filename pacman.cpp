// pacman.cpp
// Daniel Burns

#include "gfxnew.h"
#include "pacman.h"

// default constructor
Pacman::Pacman() {
	xpos = 330;
	ypos = 390;
	xcenter = xpos + 15;
	ycenter = xpos + 15;
	xvel = 3.5;
	yvel = 0;
	orientation = 1;
	animation = 1;
	character = 0;
	timescollided = 0;
}

// constructor with argument to specify a ghost
Pacman::Pacman(int i) {
	switch (i) {
		case 1:
			xpos = 270;
			xvel = 3.5;
			break;
		case 2:
			xpos = 300;
			xvel = -3.5;
			break;
		case 3:
			xpos = 330;
			xvel = 3.5;
			break;
		case 4:
			xpos = 360;
			xvel = -3.5;
			break;
	}
	character = i;
	ypos = 330;
	xcenter = xpos + 15;
	ycenter = ypos + 15;
	yvel = 0;
	orientation = 1;
	animation = 1;
	timescollided = 0;
}

// destructor
Pacman::~Pacman() {}

// function to reset ghosts stats when pacman dies
void Pacman::resetTimesCollided() {
	timescollided = 0;
}

// function to return x center of last turn
int Pacman::getLastTurnXCenter() {
	return lastturnxcenter;
}

// function to return y center of last turn
int Pacman::getLastTurnYCenter() {
	return lastturnycenter;
}

// function to set x center of last turn
void Pacman::setLastTurnXCenter(int lastturnxcenter) {
	this->lastturnxcenter = lastturnxcenter;
}

// function to set y center of last turn
void Pacman::setLastTurnYCenter(int lastturnycenter) {
	this->lastturnycenter = lastturnycenter;
}

// function to get character you're referring to
int Pacman::getCharacter() {
	return character;
}

// function to set out of ghost house
void Pacman::outOfGhostHouse() {
	ghosthouse = false;
}

// function to set inside of ghost house
void Pacman::backInGhostHouse() {
	ghosthouse = true;
}

// function to return if ghost is in ghost house
bool Pacman::getGhostHouse() {
	return ghosthouse;
}

// function to return how many times a ghost has collided with the wall (helpful in getting out of starting area)
int Pacman::getTimesCollided() {
	return timescollided;
}

// function to increment how many times a ghost has collided with the wall (helpful in getting out of starting area)
void Pacman::incrementTimesCollided() {
	timescollided++;
}

// function to return pacman's x coordinate
double Pacman::getXPos() {
	return xpos;
}

// function to return pacman's y coordinate
double Pacman::getYPos() {
	return ypos;
}

// function to return pacman's x velocity
double Pacman::getXVel() {
	return xvel;
}

// function to return pacman's y velocity
double Pacman::getYVel() {
	return yvel;
}

// function to return pacman's x center
double Pacman::getXCenter() {
	return xcenter;
}

// function to return pacman's y center
double Pacman::getYCenter() {
	return ycenter;
}

// function to return pacman's row
int Pacman::getRow() {
	return row;
}

// fuction to return pacman's column
int Pacman::getColumn() {
	return column;
}

// function to set pacman's row
void Pacman::setRow(double xcenter) {
	row = ycenter / 30;
}

// function to set pacman's column
void Pacman::setColumn(double ycenter) {
	column = xcenter / 30;
}

// function to set pacman's x center
void Pacman::setXCenter(double xcenter) {
	this->xcenter = xcenter;
}

// function to set pacman's y center
void Pacman::setYCenter(double ycenter) {
	this->ycenter = ycenter;
}

// function to set pacman's x velocity
void Pacman::setXVel(double xvel) {
	this->xvel = xvel;
	yvel = 0;
}

// function to set pacman's y velocity
void Pacman::setYVel(double yvel) {
	this->yvel = yvel;
	xvel = 0;
}

// function to set pacman's x coordinate
void Pacman::setXPos(double xpos) {
	this->xpos = xpos;
}

// function to set pacman's y coordinate
void Pacman::setYPos(double ypos) {
	this->ypos = ypos;
}

// function to get pacman's orientation
int Pacman::getOrientation() {
	return orientation;
}

// function to get pacman's animation
int Pacman::getAnimation() {
	return animation;
}

// function to set pacman's animation
void Pacman::setAnimation(int animation) {
	this->animation = animation;
}
// function to draw pacman characters
void Pacman::drawPacman(double x, double y, int orientation, int previousorientation) {
	gfx_color(255, 238, 0);
	switch (character) {
		case 0:
			if (xvel > 0) { orientation = 1; }
			else if (xvel < 0) { orientation = 3; }
			else if (yvel > 0) { orientation = 2; }
			else if (yvel < 0) { orientation = 4; }
			else { orientation = previousorientation; }
			switch (orientation) {
				case 1:
					(animation == 1) ? gfx_fill_arc(x + 5, y + 5, 20, 20, 20, 320) : gfx_fill_arc(x + 5, y + 5, 20, 20, 60, 240);
					break;
				case 2:
					(animation == 1) ? gfx_fill_arc(x + 5, y + 5, 20, 20, -70, 320) : gfx_fill_arc(x + 5, y + 5, 20, 20, -30, 240);
					break;
				case 3:
					(animation == 1) ? gfx_fill_arc(x + 5, y + 5, 20, 20, 200, 320) : gfx_fill_arc(x + 5, y + 5, 20, 20, 240, 240);
					break;
				case 4:
					(animation == 1) ? gfx_fill_arc(x + 5, y + 5, 20, 20, 110, 320) : gfx_fill_arc(x + 5, y + 5, 20, 20, 150, 240);
					break;
				default:
					break;
			}
			break;
		case 1:
			gfx_color(255, 153, 0);
			gfx_fill_circle(x + 15, y + 12, 8);
			gfx_fill_rectangle(x + 8, y + 12, 15, 12);
			break;
		case 2:
			gfx_color(255, 70, 0);
			gfx_fill_circle(x + 15, y + 12, 8);
			gfx_fill_rectangle(x + 8, y + 12, 15, 12);
			break;
		case 3:
			gfx_color(255, 192, 203);
			gfx_fill_circle(x + 15, y + 12, 8);
			gfx_fill_rectangle(x + 8, y + 12, 15, 12);
			break;
		case 4:
			gfx_color(0, 255, 255);
			gfx_fill_circle(x + 15, y + 12, 8);
			gfx_fill_rectangle(x + 8, y + 12, 15, 12);
			break;
	}
}

// function to correct character's path
void Pacman::correctPath() {
	double correctxpos = column * 30;
	double correctypos = row * 30;
	if (xvel > 0 || xvel < 0) {
		ypos = correctypos;
	}
	else if (yvel > 0 || yvel < 0) {
		xpos = correctxpos;
	}
}
