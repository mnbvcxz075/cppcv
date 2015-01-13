#include"HandMouse.h"

HandMouse::HandMouse(HandRecognition* hr){
	this->hand = hr;
	mouse_point = POINT();
	GetCursorPos(&mouse_point);
	
	int i = ShowCursor(true);

	privious_point =hand->getCentroid;
	next_index = 0;

	moveCount = 0;
	isMove = false;

}
HandMouse::~HandMouse(){

}

void HandMouse::update(){
	POINT p=hand->getCentroid();
	isMove = hand->getMouseMode != notMouse;
	setNextPoint(p);
	privious_point = p;
	moveMouse();
}

void HandMouse::setNextPoint(POINT p){
	if (isMove){
		GetCursorPos(&mouse_point);
		mouse_point.x += (p.x - privious_point.x)^3/10;
		mouse_point.y += (p.y - privious_point.y)^3/10;
	}
}

const void HandMouse::moveMouse(){
	SetCursorPos(mouse_point.x, mouse_point.y);
}

