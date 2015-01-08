#include"Setting.h"

class HandMouse{
public:
	HandMouse();
	~HandMouse();

private:
	POINT mouse_position;
	POINT* previous_points;

public:
	void setPosition();
	void moveMouse();

};