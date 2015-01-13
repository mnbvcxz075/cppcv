#include"HandMouse.h"

HandMouse::HandMouse(HandRecognition* hr){
	mouse_point = POINT();
	GetCursorPos(&mouse_point);
	
	int i = ShowCursor(true);

	privious_point = POINT();
	privious_point.x = 0;
	privious_point.y = 0;
	next_index = 0;

	moveCount = 0;
	isMove = false;

	this->hand = hr;
}
HandMouse::~HandMouse(){

}

void HandMouse::update(){
	POINT p = hand->getCentroid();
	std::cout << moveCount << ',' << (p.x - privious_point.x) << ',' << abs(p.y - privious_point.y) << std::endl;
	std::cout << p.x << ',' << p.y << ',' << privious_point.x << ',' << privious_point.y<< std::endl;
	if (!isMove){
		if (p.x != 0 && p.y != 0){
			if (std::abs(p.x - privious_point.x) > lower_distance
				&&std::abs(p.y - privious_point.y) > lower_distance){
				moveCount++;
				if (moveCount > 2){
					moveCount = 0;
					isMove = true;
					hand->setIsMouse(true);
				}
			}
		}
	}
	else{
		if (p.x != 0 && p.y != 0){
			if (std::abs(p.x - privious_point.x) < lower_distance
				&&std::abs(p.y - privious_point.y) < lower_distance){
				moveCount++;
				if (moveCount > 2){
					moveCount = 0;
					isMove = false;
					hand->setIsMouse(false);
				}
			}
		}

	}

	setNextPoint(p);
	privious_point = p;
	moveMouse();
}

void HandMouse::setNextPoint(POINT p){
	if(isMove){
		mouse_point.x += p.x - privious_point.x;
		mouse_point.y += p.y - privious_point.y;
	}
}

const void HandMouse::moveMouse(){
	SetCursorPos(mouse_point.x, mouse_point.y);
}