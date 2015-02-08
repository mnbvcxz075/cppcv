#include"HandMouse.h"

HandMouse::HandMouse(HandRecognition* hr){
	this->hand = hr;
	privious_point.x = 0;
	int i = ShowCursor(true);
}
HandMouse::~HandMouse(){

}



void HandMouse::update(){
	if (privious_point.x == 0){
		privious_point = hand->getCentroid();
	}
	POINT centroid = hand->getCentroid();
	int move_x = centroid.x - privious_point.x;
	int move_y = centroid.y - privious_point.y;
	if (abs(move_x) > max_distance || abs(move_y) > max_distance){
		privious_point = hand->getCentroid();
		return;
	}
	std::cout << "b" << std::endl;

	move_x = move_x > 0 ? std::pow(move_x, 3)/15 : std::pow(move_x, 3)/15;
	move_y = move_y > 0 ? std::pow(move_y, 3) / 15 : std::pow(move_y, 3) / 15;
	moveMouse(move_x,move_y);


	privious_point = centroid;
	//if (hand->getMouseMode() == isTouched){
	//	touchedCount = touchedCount>4 ? 5 : touchedCount + 1;
	//	if (touchedCount > 2&&!touched){
	//		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	//		touched = true;
	//	}
	//}
	//else{
	//	if (touched){
	//		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	//		touched = false;
	//	}
	//	touchedCount = 0;
	//}

}


const void HandMouse::moveMouse(int x,int y){
	int dist = x < y ? x : y; 
	POINT mouse_point;
	GetCursorPos(&mouse_point);
	std::cout << mouse_point.x << "," << mouse_point.y << std::endl;
	std::cout << mouse_point.x + x << "," << mouse_point.y + y << std::endl;
	
	SetCursorPos(mouse_point.x + x, mouse_point.y + y);
}

