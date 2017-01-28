// final.cpp
// Pacman
// Daniel Burns / Sean Howley

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <array>
#include <cstdlib>
#include <cmath>
#include <string>
#include <sstream>
#include "pacman.h"
#include "gfxnew.h"
using namespace std;

void levelStartScreen(); // prototype to display start screen for level
void initiateLevel(array< array< int, 23 >, 25 >&); // prototype to initiate pacman level
void drawLevel(const array< array< int, 23 >, 25>&); // prototype to draw the level
void drawScore(int); // prototype to draw the score
void drawLives(int); // prototype to draw the lives pacman still has
string scoreToString(int); // prototype to use the stringstream function to conver the score (int) to a string
int checkForBounds(Pacman&, const array< array< int, 23 >, 25 >&, int); // prototype to check if pacman hits bounds
void pointsIncrease(Pacman&, array< array< int, 23 >, 25 >&, int&); // prototype to check if pacman eats anything to increase score
void animate(Pacman&, int&);
void updateCharacters(Pacman&, Pacman&, Pacman&, Pacman&, Pacman&, int); // prototype to move the characters
bool spaceBelow(Pacman&, const array< array< int, 23 >, 25 >&); // prototype to check if there is open space below
bool spaceAbove(Pacman&, const array< array< int, 23 >, 25 >&); // prototype to check if there is open space above
bool spaceRight(Pacman&, const array< array< int, 23 >, 25 >&); // prototype to check if there is open space to the right
bool spaceLeft(Pacman&, const array< array< int, 23 >, 25 >&); // prototype to check if there is open space to the left
void ghostHitsBound(Pacman&, array< array< int, 23 >, 25 >&); // prototype to switch direction of ghosts if wall is reached
void pacmanGhostCollision(Pacman&, Pacman&, Pacman&, Pacman&, Pacman&, int&); // prototype to check if pacman has hit any of the ghosts
int chooseGhostDirection(Pacman&); // prototype that assists in switching the ghosts' directions
int whereisSpace(Pacman&, array< array< int, 23 >, 25 >&); // prototype to find where the ghost has space
void getOutOfStartingZone(Pacman&, Pacman&, Pacman&, Pacman&, array< array< int, 23 >, 25 >&); // prototype to get ghosts out of starting zone
void chasePacman(Pacman&, Pacman&, array< array< int, 23 >, 25 >&); // prototype to have ghosts chase pacman if they see him

int main() {
	gfx_open(690, 800, "Pacman");
	gfx_clear();
	levelStartScreen(); // display the start screen
	array< array< int, 23 >, 25 > level; // initiate the array that will hold the level layout
	initiateLevel(level); // call to function that actually initiates the level into the array
	Pacman bob, clyde(1), blinky(2), pinky(3), inky(4); // declare each character of the class 'Pacman' (bob is pacman himself)
	char c{' '}, g{' '};
	int orientation{1}, animation{1}, score{0}, lives{3};
	while (c != 'q' && score < 2330 && g != 'q' && lives > 0) {
		gfx_clear();
		drawLevel(level); // draw the level
		drawScore(score); // draw the score at the bottom of the screen
		drawLives(lives); // draw the number of lives pacman still has (0 means you lose)
		gfx_flush();
		animate(bob, animation); // show bob's mouth moving to mimick eating
		orientation = checkForBounds(bob, level, orientation); // check to see if pacman hits any walls
		pointsIncrease(bob, level, score); // check to see if pacman hits any ball, cherries, etc.
		updateCharacters(bob, clyde, blinky, pinky, inky, orientation); // update the ghosts positions (xcenter, ycenter, row, column)
		pacmanGhostCollision(bob, clyde, blinky, pinky, inky, lives); // check to see if any of the ghosts catch pacman
		getOutOfStartingZone(clyde, blinky, pinky, inky, level); // get ghosts out of starting zone
		chasePacman(bob, clyde, level); // calls to the same function to have the ghosts chase pacman if they see him
		chasePacman(bob, blinky, level); // ^
		chasePacman(bob, pinky, level); // ^^
		chasePacman(bob, inky, level); // ^^^
		ghostHitsBound(clyde, level); // calls to the same function to switch the ghosts direction if they hit a wall
		ghostHitsBound(blinky, level); // ^
		ghostHitsBound(pinky, level); // ^^
		ghostHitsBound(inky, level); // ^^^
		usleep(30000);
		if (gfx_event_waiting()) { // movement for pacman
			c = gfx_wait();
			switch (c) {
				case 'a':
					if (spaceLeft(bob, level)) { bob.setXVel(-3.5); }
					break;
				case 'w':
					if (spaceAbove(bob, level)) { bob.setYVel(-3.5); }
					break;
				case 's':
					if (spaceBelow(bob, level)) { bob.setYVel(3.5); }
					break;
				case 'd':
					if (spaceRight(bob, level)) { bob.setXVel(3.5); }
					break;
				case 'q':
					break;
			}
		}
	}
	gfx_clear();
	gfx_color(255, 255, 255);
	c = ' ';
	while (c != 'y') {
		gfx_text(245, 400, "Thanks for playing!");
		gfx_text(205, 450, "Press 'y' to quit program");
		c = gfx_wait();
	}
}

// function to draw the lives pacman still has
void drawLives(int lives) {
	string myscore{"Lives: "};
	gfx_text(140, 785, myscore.c_str());
	gfx_color(255, 238, 0);
	if (lives == 3) {
		gfx_fill_arc(220, 765, 20, 20, 50, 260);
		gfx_fill_arc(245, 765, 20, 20, 50, 260);
		gfx_fill_arc(270, 765, 20, 20, 50, 260);
	}
	else if (lives == 2) {
		gfx_fill_arc(220, 765, 20, 20, 50, 260);
		gfx_fill_arc(245, 765, 20, 20, 50, 260);
	}
	else if (lives == 1) {
		gfx_fill_arc(220, 765, 20, 20, 50, 260);

	}
}

// function to have the ghost chase pacman if it sees him
void chasePacman(Pacman& bob, Pacman& ghost, array< array< int, 23 >, 25 >& level) {
	if (ghost.getGhostHouse() == false && (bob.getRow() == ghost.getRow())) {
		if ((ghost.getColumn() - bob.getColumn()) > 0 && (ghost.getColumn() - bob.getColumn()) < 5 && spaceLeft(ghost, level) == true) {
			if (level[ghost.getRow()][ghost.getColumn() - 2] != 1 && level[ghost.getRow()][ghost.getColumn() - 3] != 1) {
				ghost.setXVel(-3.5);
				ghost.correctPath();
				ghostHitsBound(ghost, level);
			}
		}
		else if ((ghost.getColumn() - bob.getColumn()) < 0 && (ghost.getColumn() - bob.getColumn()) > -5 && spaceRight(ghost, level) == true) {
			if (level[ghost.getRow()][ghost.getColumn() + 2] != 1 && level[ghost.getRow()][ghost.getColumn() + 3] != 1) {
				ghost.setXVel(3.5);
				ghost.correctPath();
				ghostHitsBound(ghost, level);
			}
		}
	}
	else if (ghost.getGhostHouse() == false && (bob.getColumn() == ghost.getColumn())) {
		if ((ghost.getRow() - bob.getRow()) > 0 && (ghost.getRow() - bob.getRow()) < 5 && spaceAbove(ghost, level) == true) {
			if (level[ghost.getRow() - 2][ghost.getColumn()] != 1 && level[ghost.getRow() - 3][ghost.getColumn()] != 1) { 
				ghost.setYVel(-3.5);
				ghost.correctPath();
				ghostHitsBound(ghost, level);
			}
		}
		else if ((ghost.getRow() - bob.getRow()) < 0 && (ghost.getRow() - bob.getRow()) > -5 && spaceBelow(ghost, level) == true) {
				ghost.setYVel(3.5);
				ghost.correctPath();
				ghostHitsBound(ghost, level);
		}
	}
}

// function to check if pacman has hit any of the ghosts
void pacmanGhostCollision(Pacman& bob, Pacman& clyde, Pacman& blinky, Pacman& pinky, Pacman& inky, int& lives) {
	if ((abs(bob.getXCenter() - clyde.getXCenter()) < 20) && (abs(bob.getYCenter() - clyde.getYCenter()) < 15)) {
		lives--;
		bob.setXPos(330);
		bob.setYPos(390);
		bob.setXVel(3.5);
	}
	else if ((abs(bob.getXCenter() - blinky.getXCenter()) < 20) && (abs(bob.getYCenter() - blinky.getYCenter()) < 15)) {
		lives--;
		bob.setXPos(330);
		bob.setYPos(390);
		bob.setXVel(3.5);
	}
	else if ((abs(bob.getXCenter() - pinky.getXCenter()) < 20) && (abs(bob.getYCenter() - pinky.getYCenter()) < 15)) {
		lives--;
		bob.setXPos(330);
		bob.setYPos(390);
		bob.setXVel(3.5);
	}
	else if ((abs(bob.getXCenter() - inky.getXCenter()) < 20) && (abs(bob.getYCenter() - inky.getYCenter()) < 15)) {
		lives--;
		bob.setXPos(330);
		bob.setYPos(390);
		bob.setXVel(3.5);
	}
}

// function to update data members of characters
void updateCharacters(Pacman& bob, Pacman& clyde, Pacman& blinky, Pacman& pinky, Pacman& inky, int orientation) {
	clyde.setXCenter(clyde.getXPos() + 15);
	clyde.setYCenter(clyde.getYPos() + 15);
	clyde.setRow(clyde.getYCenter());
	clyde.setColumn(clyde.getXCenter());
	blinky.setXCenter(blinky.getXPos() + 15);
	blinky.setYCenter(blinky.getYPos() + 15);
	blinky.setRow(blinky.getYCenter());
	blinky.setColumn(blinky.getXCenter());
	pinky.setXCenter(pinky.getXPos() + 15);
	pinky.setYCenter(pinky.getYPos() + 15);
	pinky.setRow(pinky.getYCenter());
	pinky.setColumn(pinky.getXCenter());
	inky.setXCenter(inky.getXPos() + 15);
	inky.setYCenter(inky.getYPos() + 15);
	inky.setRow(inky.getYCenter());
	inky.setColumn(inky.getXCenter());
	bob.setXPos(bob.getXPos() + bob.getXVel());
	bob.setYPos(bob.getYPos() + bob.getYVel());
	clyde.setXPos(clyde.getXPos() + clyde.getXVel());
	clyde.setYPos(clyde.getYPos() + clyde.getYVel());
	blinky.setXPos(blinky.getXPos() + blinky.getXVel());
	blinky.setYPos(blinky.getYPos() + blinky.getYVel());
	pinky.setXPos(pinky.getXPos() + pinky.getXVel());
	pinky.setYPos(pinky.getYPos() + pinky.getYVel());
	inky.setXPos(inky.getXPos() + inky.getXVel());
	inky.setYPos(inky.getYPos() + inky.getYVel());
	bob.drawPacman(bob.getXPos(), bob.getYPos(), bob.getOrientation(), orientation);
	clyde.drawPacman(clyde.getXPos(), clyde.getYPos(), clyde.getOrientation(), orientation);
	blinky.drawPacman(blinky.getXPos(), blinky.getYPos(), blinky.getOrientation(), orientation);
	pinky.drawPacman(pinky.getXPos(), pinky.getYPos(), pinky.getOrientation(), orientation);
	inky.drawPacman(inky.getXPos(), inky.getYPos(), inky.getOrientation(), orientation);
	bob.correctPath();
	inky.drawPacman(inky.getXPos(), inky.getYPos(), inky.getOrientation(), orientation);
}

// function to check if there is space above
bool spaceAbove(Pacman& bob, const array< array< int, 23 >, 25 >& level) {
	if (level[bob.getRow() - 1][bob.getColumn()] == 1) { return false; }
	else { return true; }
}

// function to check if there is space below
bool spaceBelow(Pacman& bob, const array< array< int, 23 >, 25 >& level) {
	if (level[bob.getRow() + 1][bob.getColumn()] == 1) { return false; }
	else { return true; }
}

// function to check if there is space to the right
bool spaceRight(Pacman& bob, const array< array< int, 23 >, 25 >& level) {
	if (level[bob.getRow()][bob.getColumn() + 1] == 1) { return false; }
	else { return true; }
}

// function to check if there is space to the left
bool spaceLeft(Pacman& bob, const array< array< int, 23 >, 25 >& level) {
	if (level[bob.getRow()][bob.getColumn() - 1] == 1) { return false; }
	else { return true; }
}

// function to animate bob
void animate(Pacman& bob, int& animation) {
	animation++;
	if (animation < 4) { bob.setAnimation(1); }
	else if (animation >= 4 && animation < 8) { bob.setAnimation(2); }
	else { animation = 1; }
}

// function to draw the score
void drawScore(int score) {
	string myscore{"Score: "};
	myscore += scoreToString(score);
	gfx_text(440, 785, myscore.c_str());
}

// function to convert the score to a string
string scoreToString(int score) {
	stringstream thescore;
	thescore << score;
	return thescore.str();
}

// function to check if pacman eats anything on the map that will increase player's score
void pointsIncrease(Pacman& bob, array< array< int, 23 >, 25>& level, int& score) {
	bob.setXCenter(bob.getXPos() + 15);
	bob.setYCenter(bob.getYPos() + 15);
	bob.setRow((double)bob.getXCenter());
	bob.setColumn((double)bob.getYCenter());
	if (level[bob.getRow()][bob.getColumn()] == 2) {
		level[bob.getRow()][bob.getColumn()] = 0;
		score += 10;
	}
	else if (level[bob.getRow()][bob.getColumn()] == 3) {
		level[bob.getRow()][bob.getColumn()] = 0;
		score += 30;
	}
	else if (level[bob.getRow()][bob.getColumn()] == 4) {
		level[bob.getRow()][bob.getColumn()] = 0;
		score += 80;
	}
}

// function to check if pacman hits bounds
int checkForBounds(Pacman& bob, const array< array< int, 23 >, 25>& level, int previousorientation) {
	bob.setXCenter(bob.getXPos() + 15);
	bob.setYCenter(bob.getYPos() + 15);
	bob.setRow((double)bob.getXCenter());
	bob.setColumn((double)bob.getYCenter());
	if ((bob.getXVel() > 0) && (level[bob.getRow()][bob.getColumn() + 1] == 1)) {
		if (bob.getXCenter() >= ((bob.getColumn() + 1) * 30 - 15)) {
			bob.setXVel(0);
			previousorientation = 1;
		}
	}
	else if ((bob.getXVel() < 0) && (level[bob.getRow()][bob.getColumn() - 1] == 1)) {
		if (bob.getXCenter() <= ((bob.getColumn() - 1) * 30 + 45)) {
			bob.setXVel(0);
			previousorientation = 3;
		}
	}
	else if ((bob.getYVel() > 0) && (level[bob.getRow() + 1][bob.getColumn()] == 1)) {
		if (bob.getYCenter() >= ((bob.getRow() + 1) * 30 - 15)) {
			bob.setYVel(0);
			previousorientation = 2;
		}
	}
	else if ((bob.getYVel() < 0) && (level[bob.getRow() - 1][bob.getColumn()] == 1)) {
		if (bob.getYCenter() <= ((bob.getRow() - 1) * 30 + 45)) {
			bob.setYVel(0);
			previousorientation = 4;
		}
	}
	else if (bob.getXPos() > 656) {
		bob.setXPos(4);
		bob.setXVel(3.5);
	}
	else if (bob.getXPos() < 4) {
		bob.setXPos(656);
		bob.setXVel(-3.5);
	}
	return previousorientation;
}

// function to draw the level
void drawLevel(const array < array < int, 23 >, 25>& level) {
	int x{0}, y{0};
	gfx_color(0, 0, 200); // draw the walls
	for (int i{0}; i < 25; i++) {
		for (int j{0}; j < 23; j++) {
			if (level[i][j] == 1) {
				gfx_fill_rectangle(x, y, 30, 30);
				x += 30;
			}
			else {
				x += 30;
			}
		}
		x = 0;
		y += 30;
	}
	x = 0;
	y = 0;
	gfx_color(255, 238, 0); // draw the balls that pacman can eat
	for (int i{0}; i < 25; i++) {
		for (int j{0}; j < 23; j++) {
			if (level[i][j] == 2) {
				gfx_fill_circle(x+15, y+15, 3);
				x += 30;
			}
			else if (level[i][j] == 3) {
				gfx_fill_circle(x+15, y+15, 8);
				x += 30;
			}
			else {
				x += 30;
			}
		}
		x = 0;
		y += 30;
	}
	x = 0;
	y = 0;
	for (int i{0}; i < 25; i++) { // draw the cherries that are worth extra points
		for (int j{0}; j < 23; j++) {
			if (level[i][j] == 4) {
				gfx_color(230, 0, 0);
				gfx_fill_circle(x+11, y + 20, 6);
				gfx_fill_circle(x+19, y + 20, 6);
				gfx_color(0, 220, 0);
				gfx_arc(x + 13, y + 5, 5, 12, 80, 190);
				x += 30;
			}
			else {
				x += 30;
			}
		}
		x = 0;
		y += 30;
	}
	gfx_color(255, 255, 255);
}

// function to initiate pacman level
void initiateLevel(array < array < int, 23>, 25 >& level) {
	// (0's) are open space
	// (1's) are the walls
	// (2's) are the mini balls
	// (3's) are the larger balls
	// (4's) are the cherries
	level = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 1,
		 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1,
		 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1,
		 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
		 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1,
		 1, 2, 2, 2, 2, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 2, 2, 2, 2, 1,
		 1, 1, 1, 1, 1, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 1, 1, 1, 1, 1,
		 0, 0, 0, 0, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 2, 1, 0, 0, 0, 0,
		 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0,
		 1, 1, 1, 1, 1, 2, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 2, 1, 1, 1, 1, 1,
		 0, 0, 0, 0, 4, 2, 2, 0, 1, 0, 0, 0, 0, 0, 1, 0, 2, 2, 4, 0, 0, 0, 0,
		 1, 1, 1, 1, 1, 2, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 2, 1, 1, 1, 1, 1,
		 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 1, 0, 0, 0, 0,
		 0, 0, 0, 0, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 0, 0, 0, 0,
		 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1,
		 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1 ,2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
		 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 2, 1,
		 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 1,
		 1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 2, 1, 1, 1,
		 1, 4, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 1,
		 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1,
		 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1,
		 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
		 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
}

// function to display start screen for level
void levelStartScreen() {
	bool loop{true};
	int pressed{0}, event;
	char c{' '}, s[6] = "12x24";
	gfx_changefont(s); // change to a larger font
	gfx_text(245, 300, "Welcome to Pacman");
	gfx_rectangle(185, 350, 320, 100);
	gfx_text(235, 410, "Press Here to Start");
	gfx_text(310, 500, "- 10 Points");
	gfx_text(310, 550, "- 80 Points");
	gfx_color(255, 238, 0);
	gfx_fill_circle(280, 489, 6);
	gfx_color(230, 0, 0);
	gfx_fill_circle(275, 539, 8);
	gfx_fill_circle(284, 539, 8);
	gfx_color(0, 220, 0);
	gfx_arc(276, 514, 10, 20, 50, 190);
	gfx_color(255, 255, 255);
	gfx_text(300, 600, "a - Left");
	gfx_text(300, 650, "d - Right");
	gfx_text(300, 700, "w - Up");
	gfx_text(300, 750, "s - Down");
	while (loop) {
		if ((event = gfx_event_waiting()) != 0) {
			c = gfx_wait();
			// turn the box white and text black to signal that you've pressed in the right area
			if (event == 3 && gfx_xpos() >= 185 && gfx_xpos() <= 505 && gfx_ypos() >= 350 && gfx_ypos() <= 450) {
				gfx_fill_rectangle(185, 350, 320, 100);
				gfx_color(0, 0, 0);
				gfx_text(235, 410, "Press Here to Start");
				pressed = 1;
			}
			// if press is released while mouse is STILL IN the box
			else if (event == 4 && pressed == 1 && gfx_xpos() >= 185 && gfx_xpos() <= 505 && gfx_ypos() >= 350 && gfx_ypos() <= 450) {
				loop = false;
				gfx_color(255, 255, 255);
			}
			// if press is released while mouse has moved OUTSIDE of the box
			else if (event == 4 && pressed == 1 && (gfx_xpos() <= 185 || gfx_xpos() >= 505 || gfx_ypos() <= 350 || gfx_ypos() >= 450)) {
				pressed = 0;
				gfx_fill_rectangle(180, 340, 350, 130);
				gfx_color(255, 255, 255);
				gfx_text(235, 410, "Press Here to Start");
				gfx_rectangle(185, 350, 320, 100);
			}
		}
	}
	gfx_clear();
}

// function that chooses a random direction for the ghosts;
int chooseGhostDirection(Pacman& ghost) {
	int direction = rand() % 4 + 1;
	if (direction == 1) { ghost.setXVel(3.5); }
	else if (direction == 2) { ghost.setYVel(3.5); }
	else if (direction == 3) { ghost.setXVel(-3.5); }
	else if (direction == 4) { ghost.setYVel(-3.5); }
	return direction;
}

// function to get ghosts out of starting zone
void getOutOfStartingZone(Pacman& clyde, Pacman& blinky, Pacman& pinky, Pacman& inky, array< array< int, 23 >, 25 >& level) {
	if (clyde.getXPos() >= 330 && clyde.getXPos() <= 340 && clyde.getYPos() >= 330 && clyde.getYPos() <= 360 && clyde.getTimesCollided() > 1) {
		clyde.setYVel(-3.5);
		clyde.correctPath();
	}
	if (blinky.getXPos() >= 330 && blinky.getXPos() <= 340 && blinky.getYPos() >= 330 && blinky.getYPos() <= 360 && blinky.getTimesCollided() > 5) {
		blinky.setYVel(-3.5);
		blinky.correctPath();
	}
	if (pinky.getXPos() >= 330 && pinky.getXPos() <= 340 && pinky.getYPos() >= 330 && pinky.getYPos() <= 360 && pinky.getTimesCollided() > 10) {
		pinky.setYVel(-3.5);
		pinky.correctPath();
	}
	if (inky.getXPos() >= 330 && inky.getXPos() <= 340 && inky.getYPos() >= 330 && inky.getYPos() <= 360 && inky.getTimesCollided() > 15) {
		inky.setYVel(-3.5);
		inky.correctPath();
	}
}

// function to get ghosts to change directions (and stop in the correct pixels) if they hit a wall
void ghostHitsBound(Pacman& ghost, array< array< int, 23 >, 25 >& level) {
	srand(time(0));
	int newdirection;
	if (ghost.getGhostHouse() == 1) {
		if (ghost.getXVel() > 0 && spaceRight(ghost, level) == false) {
			ghost.setXVel(-3.5);
			ghost.incrementTimesCollided();
			ghost.correctPath();
		}
		else if (ghost.getXVel() < 0 && spaceLeft(ghost, level) == false) {
			ghost.setXVel(3.5);
			ghost.incrementTimesCollided();
			ghost.correctPath();
		}
		if (ghost.getYVel() < 0 && ghost.getXPos() >= 330 && ghost.getXPos() <= 360 && ghost.getYPos() >= 240 && ghost.getYPos() <= 270) {
			switch (ghost.getCharacter()) {
				case 1:
					ghost.setXVel(-3.5);
					ghost.setXPos(325);
					break;
				case 2:
					ghost.setXVel(3.5);
					ghost.setXPos(335);
					break;
				case 3:
					ghost.setXVel(-3.5);
					ghost.setXPos(325);
					break;
				case 4:
					ghost.setXVel(3.5);
					ghost.setXPos(335);
					break;
			}
			ghost.correctPath();
			ghost.setLastTurnXCenter(ghost.getXCenter());
			ghost.setLastTurnYCenter(ghost.getYCenter());
			ghost.outOfGhostHouse();
		}
	}
	else {
		if (ghost.getXPos() > 656) {
			ghost.setXPos(4);
			ghost.setXVel(3.5);
			return;
		}
		else if (ghost.getXPos() < 4) {
			ghost.setXPos(656);
			ghost.setXVel(-3.5);
			return;
		}
		if (ghost.getXVel() > 0 && ghost.getXPos() >= 330 && ghost.getXPos() <= 360 && ghost.getYPos() >= 240 && ghost.getYPos() <= 270 && ghost.getXCenter() > ghost.getLastTurnXCenter() + 30) {
			ghost.setXVel(ghost.getXVel());
			return;
		}
		if (ghost.getXVel() < 0 && ghost.getXPos() >= 330 && ghost.getXPos() <= 360 && ghost.getYPos() >= 240 && ghost.getYPos() <= 270 && ghost.getXCenter() < ghost.getLastTurnXCenter() - 30) {
			ghost.setXVel(ghost.getXVel());
			return;
		}
		if (ghost.getXVel() > 0 && ghost.getXPos() >= 205 && ghost.getXPos() <= 240 && ghost.getYPos() >= 320 && ghost.getYPos() <= 330) {
			newdirection = rand() % 2 + 1;
			if (newdirection == 1) {
				ghost.setYVel(-3.5);
				ghost.setLastTurnXCenter(ghost.getXCenter());
				ghost.setLastTurnYCenter(ghost.getYCenter());
				return;
			}
			else {
				ghost.setYVel(3.5);
				ghost.setLastTurnXCenter(ghost.getXCenter());
				ghost.setLastTurnYCenter(ghost.getYCenter());
				return;
			}
		}
		else if (ghost.getXVel() < 0 && ghost.getXPos() >= 450 && ghost.getXPos() <= 470 && ghost.getYPos() >= 320 && ghost.getYPos() <= 330) {
			newdirection = rand() % 2 + 1;
			if (newdirection == 1) {
				ghost.setYVel(-3.5);
				ghost.setLastTurnXCenter(ghost.getXCenter());
				ghost.setLastTurnYCenter(ghost.getYCenter());
				return;
			}
			else {
				ghost.setYVel(3.5);
				ghost.setLastTurnXCenter(ghost.getXCenter());
				ghost.setLastTurnYCenter(ghost.getYCenter());
				return;
			}
		}
		if (ghost.getXVel() < 0 && ghost.getXCenter() < ghost.getLastTurnXCenter() - 30) {
			(spaceLeft(ghost, level) == true) ? newdirection = rand() % 3 + 1 : newdirection = rand() % 2 + 1;
			if (newdirection == 1 && spaceAbove(ghost, level) == false) { newdirection = 2; }
			else if (newdirection == 2 && spaceBelow(ghost, level) == false) { newdirection = 1; }
			if (newdirection == 1 && spaceAbove(ghost, level) == true) {
				ghost.setYVel(-3.5);
				ghost.setLastTurnXCenter(ghost.getXCenter());
				ghost.setLastTurnYCenter(ghost.getYCenter());
			}
			else if (newdirection == 2 && spaceBelow(ghost, level) == true) {
				ghost.setYVel(3.5);
				ghost.setLastTurnXCenter(ghost.getXCenter());
				ghost.setLastTurnYCenter(ghost.getYCenter());
			}
		}
		else if (ghost.getXVel() > 0 && ghost.getXCenter() > ghost.getLastTurnXCenter() + 30) {
			(spaceRight(ghost, level) == true) ? newdirection = rand() % 3 + 1 : newdirection = rand() % 2 + 1;
			if (newdirection == 1 && spaceAbove(ghost, level) == false) { newdirection = 2; }
			else if (newdirection == 2 && spaceBelow(ghost, level) == false) { newdirection = 1; }
			if (newdirection == 1 && spaceAbove(ghost, level) == true) {
				ghost.setYVel(-3.5);
				ghost.setLastTurnXCenter(ghost.getXCenter());
				ghost.setLastTurnYCenter(ghost.getYCenter());
			}
			else if (newdirection == 2 && spaceBelow(ghost, level) == true) {
				ghost.setYVel(3.5);
				ghost.setLastTurnXCenter(ghost.getXCenter());
				ghost.setLastTurnYCenter(ghost.getYCenter());
			}
		}
		else if (ghost.getYVel() < 0 && ghost.getYCenter() < ghost.getLastTurnYCenter() - 30) {
			(spaceAbove(ghost, level) == true) ? newdirection = rand() % 3 + 1 : newdirection = rand() % 2 + 1;
			if (newdirection == 1 && spaceRight(ghost, level) == false) { newdirection = 2; }
			else if (newdirection == 2 && spaceLeft(ghost, level) == false) { newdirection = 1; }
			if (newdirection == 1 && spaceRight(ghost, level) == true) {
				ghost.setXVel(3.5);
				ghost.setLastTurnXCenter(ghost.getXCenter());
				ghost.setLastTurnYCenter(ghost.getYCenter());
			}
			else if (newdirection == 2 && spaceLeft(ghost, level) == true) {
				ghost.setXVel(-3.5);
				ghost.setLastTurnXCenter(ghost.getXCenter());
				ghost.setLastTurnYCenter(ghost.getYCenter());
			}
		}
		else if (ghost.getYVel() > 0 && ghost.getYCenter() > ghost.getLastTurnYCenter() + 30) {
			(spaceBelow(ghost, level) == true) ? newdirection = rand() % 3 + 1 : newdirection = rand() % 2 + 1;
			if (newdirection == 1 && spaceRight(ghost, level) == false) { newdirection = 2; }
			else if (newdirection == 2 && spaceLeft(ghost, level) == false) { newdirection = 1; }
			if (newdirection == 1 && spaceRight(ghost, level) == true) {
				ghost.setXVel(3.5);
				ghost.setLastTurnXCenter(ghost.getXCenter());
				ghost.setLastTurnYCenter(ghost.getYCenter());
			}
			else if (newdirection == 2 && spaceLeft(ghost, level) == true) {
				ghost.setXVel(-3.5);
				ghost.setLastTurnXCenter(ghost.getXCenter());
				ghost.setLastTurnYCenter(ghost.getYCenter());
			}
		}
		ghost.correctPath();
	}
}
