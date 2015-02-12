#include"HandMouse.h"

HandMouse::HandMouse(HandRecognition* hr){
	this->hand = hr;
	privious_point.x = 0;
	int i = ShowCursor(true);
	isLeftClicked = false;
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
	if (hand->existFingers()[thumb]){
		move_x = move_x > 0 ? std::pow(move_x, 3) / 15 : std::pow(move_x, 3) / 15;
		move_y = move_y > 0 ? std::pow(move_y, 3) / 15 : std::pow(move_y, 3) / 15;
	}
	else{
		std::cout << "thumb" << std::endl;

	}
	moveMouse(move_x,move_y);

	privious_point = centroid;

	if (hand->isTurned()){
		mouse_event(MOUSEEVENTF_WHEEL, 0, 0, -1, 0);
		std::cout << "turn" << std::endl;
	}

	
	
	if (!hand->existFingers()[index]&&!isLeftClicked){
		mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		isLeftClicked = true;
		std::cout << "down" << std::endl;
	}
	if (hand->existFingers()[index] && isLeftClicked){
		mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		isLeftClicked = false;
		std::cout << "up" << std::endl;
	}

}



const void HandMouse::moveMouse(int x,int y){
	int dist = abs(abs(x) > abs(y) ? x : y);
	std::cout << x << "," << y << "," << dist<< std::endl;
	POINT mouse_point;
	GetCursorPos(&mouse_point);	
	for (int i = 0; i < dist; i++){
		SetCursorPos(mouse_point.x + x/dist*i, mouse_point.y + y/dist*i);
		std::cout << x / dist*i << std::endl;
	}
	//SetCursorPos(mouse_point.x + x, mouse_point.y + y);
}

