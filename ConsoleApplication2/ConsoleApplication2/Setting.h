#pragma once

#include"opencv2\opencv.hpp"
#include<iostream>
#include<Windows.h>
#include<stdlib.h>

#pragma comment(lib, "winmm.lib")

static const std::string WINDOW_NAME("window");

enum{
	notMouse,
	isMouse,
	isTouched
};

