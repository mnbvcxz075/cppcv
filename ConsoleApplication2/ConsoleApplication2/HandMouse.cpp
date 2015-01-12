#include"HandMouse.h"

HandMouse::HandMouse(HandRecognition* hr){
	mouse_point = POINT();
	GetCursorPos(&mouse_point);

	privious_points = new POINT[5];
	next_index = 0;

	this->hand = hr;
}
HandMouse::~HandMouse(){

}

void HandMouse::setNextPoint(int x,int y){
	privious_points[next_index] = mouse_point;
	next_index = next_index < 5 ? next_index + 1 : 0;

	mouse_point.x = x;
	mouse_point.y = y;
}

const void HandMouse::moveMouse(){
	SetCursorPos(mouse_point.x, mouse_point.y);
}