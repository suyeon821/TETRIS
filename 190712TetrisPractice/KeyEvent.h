#pragma once

#include "game.h"

class KeyEvent {
private:
	HANDLE hin;
	COORD KeyWhere;
	DWORD EventCount;
	INPUT_RECORD InRec;
	DWORD NumRead;
	int downKey;
	Game *game;
	//Thread (run)
	static unsigned int __stdcall run(void *arg) {
		KeyEvent *key = (KeyEvent*)arg;
		while (true) {
			int temp = key->getKey();
			if (temp > 36 && temp < 41) { key->game->setDirection(temp); }
		}
		return 0;
	}
public:
	//Constructor
	KeyEvent() {
		hin = GetStdHandle(STD_INPUT_HANDLE);
		EventCount = 1;
	}
	//ReadKey
	int getKey() {
		ReadConsoleInput(hin, &InRec, 1, &NumRead);
		if (InRec.EventType == KEY_EVENT) {
			if (InRec.Event.KeyEvent.bKeyDown) {
				downKey = InRec.Event.KeyEvent.wVirtualKeyCode;
				return downKey;
			}
		}
		else {
			return -1;
		}
	}
	//Thread (start)
	void start(Game *game) {
		this->game = game;
		_beginthreadex(NULL, 0, run, (void*)this, 0, NULL);
	}
};