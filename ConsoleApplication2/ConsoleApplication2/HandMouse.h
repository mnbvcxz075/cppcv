#pragma once


#include"Setting.h"
#include"HandRecognition.h"

class HandMouse{
public:
	HandMouse(HandRecognition*);
	~HandMouse();

private:
	const int max_distance = 30;
	const int min_distance = 4;
	POINT privious_point;
	int next_index;
	POINT mouse_point;
	int moveCount;
	int touchedCount;
	bool touched;
	bool handd;

	HandRecognition* hand;

public:
	const void moveMouse(int,int);
	void setNextPoint(POINT);
	void update();

};