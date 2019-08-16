#pragma once
#include <iostream>
#include <Windows.h>
#include "GameManager.h"
#include "Constants.h"
#include "Console.h"
using namespace std;

class Desk : public Console {
public:
	Desk() {
		HWND console = GetConsoleWindow(); //get 윈도우' 핸들값
		MoveWindow(console, 100, 200, 320, 450, true); //윈도우' 위치, 크기, visiable 변경
		newDesk();
	}
	void newDesk() {
		cout << "\n\n\n\n   - Welcome to TETRIS! -      \n";
		Sleep(800);
	}
	void printDesk(GameManager gm) {
		cout << endl;
		setColor(15);
		cout << "        Lv." << gm.getLv() << "  |TETRIS|  " << "Sc." << gm.getScore() << endl;
		cout << "      ┏━━━━━━━━━━━━━━━━━━━━━┓" << endl;
		for (int i = 0; i < MAX_HEIGHT; i++) {
			cout << "      ┃ ";
			for (int j = 0; j < MAX_WIDTH / 2; j++) {
				if (gm.getMap(i, j) == 0) { cout << "  "; }
				else {
					setColor(gm.getMapCol(i, j));
					//cout << gm.getMapCol(i, j);
					cout << "■";
				}
			}
			setColor(15);
			cout << "┃" << endl;
		}
		cout << "      ┗━━━━━━━━━━━━━━━━━━━━━┛" << endl;
	}
	void overDesk() {
		system("cls");
		setColor(15);
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n        -GAME OVER-      \n" << endl;
		cout << "\n\n\n\n\n\n\n\n\n\n\n" << endl;
		exit(0);
	}
};