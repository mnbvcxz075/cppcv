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
	
	if (!hand->existHand()||!hand->mousemouse){
		privious_point.x = 0;
		return;
	}
	if (privious_point.x == 0){
		privious_point = hand->getCentroid();
	}
	POINT centroid = hand->getCentroid();
	if (hand->existFingers()[thumb]){

		int move_x = centroid.x - privious_point.x;
		int move_y = centroid.y - privious_point.y;
		if (abs(move_x) > max_distance || abs(move_y) > max_distance ){
			privious_point = hand->getCentroid();
			return;
		}

		//move_x = abs(std::pow(move_x, 3) / 5) > 25 ? std::pow(move_x, 3) / 5 : move_x > 0 ? 1 : -1;
		//move_y = abs(std::pow(move_y, 3) / 5) > 25 ? std::pow(move_y, 3) / 5 : move_y > 0 ? 1 : -1;
		move_x = abs(std::pow(move_x, 3) / 20) > 20 ? std::pow(move_x, 3) / 20 : move_x;
		move_y = abs(std::pow(move_y, 3) / 20) > 20 ? std::pow(move_y, 3) / 20 : move_y;

		//move_x = move_x > 0 ? std::pow(move_x, 3) / 5 : std::pow(move_x, 3) / 5;
		//move_y = move_y > 0 ? std::pow(move_y, 3) / 5 : std::pow(move_y, 3) / 5;

		moveMouse(move_x, move_y);
	}
	privious_point = centroid;

	
	if (hand->isLTurned()){
		mouse_event(MOUSEEVENTF_WHEEL, 0, 0, 120, 0);
		std::cout << "turn" << std::endl;
	}
if (hand->isRTurned()){
		mouse_event(MOUSEEVENTF_WHEEL, 0, 0, -120, 0);
		std::cout << "turn" << std::endl;
	}
	javascript:void(0);
	
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
	POINT mouse_point;
	GetCursorPos(&mouse_point);	
	//for (int i = 0; i < dist; i++){
	//	SetCursorPos(mouse_point.x + x / dist*i, mouse_point.y + y / dist*i);
	//}
	SetCursorPos(mouse_point.x + x, mouse_point.y + y);
}

