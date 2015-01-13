#include<Windows.h>
#include<iostream>



int main(){
	INPUT in;
	in.type = INPUT_MOUSE;
	in.mi.dx = 500;
	in.mi.dy = 500;
	in.mi.dwFlags = MOUSEEVENTF_ABSOLUTE;
	LPINPUT pin = (LPINPUT)&in;

	std::cout << ShowCursor(true);
	for (int i = 0; i < 500;i++){

//		SetCursorPos(i,i);
		Sleep(16);
		SendInput(1,pin,sizeof(INPUT));
	}
}