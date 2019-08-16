#pragma once
#include <Windows.h>
#include <process.h>
#include <iostream>
using namespace std;

class Console {
protected:
	//ConsoleHandle
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//COORD (x,y)
	short x;
	short y;
	//Key
	int direction;
public:
	//Constructor (cursor invisiable)
	Console() {
		CONSOLE_CURSOR_INFO cursorInfo = { 0, };
		cursorInfo.dwSize = 1;
		cursorInfo.bVisible = FALSE;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	}
	//CursorPosition
	void setXY(int x, int y) {
		this->x = x;
		this->y = y;
	}
	void gotoXY(int x, int y) {
		COORD pos = { (short)x, (short)y };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	}
	void setColor(int color) {
		SetConsoleTextAttribute(hConsole, color);
	}
	void getXY(short x, short y) {
		gotoXY(x, y);
		cout << "бр"; // жн бр бс
		Sleep(200);
	}
};