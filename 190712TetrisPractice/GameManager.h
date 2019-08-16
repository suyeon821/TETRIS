#pragma once
#include "Constants.h"

class GameManager {
private:
	int level = 3;
	unsigned int score = 0;
	short map[MAX_HEIGHT + 1][MAX_WIDTH / 2][2] = { 0, }; //2D array - map from (8, 3) to (6 + MAX_WIDTH, 2 + MAX_HEIGHT) - [i=y-3],[j=(x-8)/2]
public:
	//Constructor
	GameManager() {
		for (int i = 0; i < MAX_WIDTH / 2; i++) {
			//check-in을 위해 마지막 숨김 행 = 1 로 설정
			map[MAX_HEIGHT][i][0] = 1;
		}
	}
	//Level
	int getLv() {
		return level;
	}
	void setLv() {
		level++;
	}
	//Score
	int getScore() {
		return score;
	}
	void plusScore() {
		score += 10;
		if (score == 100) {
			score = 0;
			setLv();
		}
	}
	//Map
	short getMap(int i, int j) {
		return map[i][j][0];
	}
	void setMap(int i, int j, int x) {
		map[i][j][0] = x;
	}
	short getMapCol(int i, int j) {
		return map[i][j][1];
	}
	void setMapCol(int i, int j, int col) {
		map[i][j][1] = col;
	}
	//check_line (moveDown)
	void moveDown(int line) {
		int upline = line - 1;
		while (upline >= 0) {
			for (int i = 0; i < MAX_WIDTH / 2; i++) {
				map[line][i][0] = map[upline][i][0];
				map[line][i][1] = map[upline][i][1];
			}
			line--;
			upline--;
		}
	}
};