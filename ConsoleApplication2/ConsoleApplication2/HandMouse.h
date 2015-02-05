#pragma once


#include"Setting.h"
#include"HandRecognition.h"

class HandMouse{
public:
	HandMouse(HandRecognition*);
	~HandMouse();

private:
	static const int lower_distance = 3;
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