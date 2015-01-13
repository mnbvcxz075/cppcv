#include<Windows.h>
#include<iostream>

int main(){
	std::cout << ShowCursor(true);
	for (int i = 0; i < 500;i++){

		SetCursorPos(i,i);
		Sleep(16);
	}
}