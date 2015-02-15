#pragma once

#include"Setting.h"
#include"HandRecognition.h"

class HandMouse{
public:
	HandMouse(HandRecognition*);
	~HandMouse();

private:
	const int max_distance = 20;
	const int min_distance = 1;
	POINT privious_point;
	int next_index;
	POINT mouse_point;
	bool isLeftClicked;

	HandRecognition* hand;

public:
	const void moveMouse(int,int);
	void setNextPoint(POINT);
	void update();

};