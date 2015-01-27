#include"HandMouse.h"

HandMouse::HandMouse(HandRecognition* hr){
	this->hand = hr;
	mouse_point = POINT();
	GetCursorPos(&mouse_point);
	
	int i = ShowCursor(true);
	while (notMouse==hand->getMouseMode()){
		hand->update();
	}
	privious_point =hand->getCentroid();
	next_index = 0;

	moveCount = 0;
	touchedCount = 0;
	touched = false;
}
HandMouse::~HandMouse(){

}



void HandMouse::update(){
	const POINT p = hand->getCentroid();

	if( hand->getMouseMode() == notMouse||!hand->mousemouse)return;
	if (p.x - privious_point.x > -20 && p.x - privious_point.x<20 && p.y - privious_point.y>-20 && p.y - privious_point.y < 20){
		setNextPoint(p);
		moveMouse();
	}
	if (hand->getMouseMode() == isTouched){
		touchedCount = touchedCount>2 ? 3 : touchedCount + 1;
		if (touchedCount > 2&&!touched){
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			touched = true;
		}
	}
	else{
		if (touched){
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			touched = false;
		}
		touchedCount = 0;
	}

	privious_point = p;
}

void HandMouse::setNextPoint(POINT p){
	//if (isMove){
		GetCursorPos(&mouse_point);
		mouse_point.x += p.x > privious_point.x ? std::pow((p.x - privious_point.x), 2) / 10 : -std::pow((p.x - privious_point.x), 2) / 10;
		mouse_point.y += p.y > privious_point.y ? std::pow((p.y - privious_point.y), 2) / 10 : -std::pow((p.y - privious_point.y), 2) / 10;
		//}
}

const void HandMouse::moveMouse(){
	//if (isMove){
		SetCursorPos(mouse_point.x, mouse_point.y);
	//}
}

