#include<Windows.h>

int main(){
	for (int i = 0; i < 500;i++){
		SetCursorPos(i,i);
		Sleep(16);
	}
}