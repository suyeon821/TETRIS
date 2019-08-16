#pragma once
#include "Console.h"
#include "GameManager.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>
using namespace std;

class Block : public Console {
private:
	vector < vector < vector<short> > > block;
	int rotation = 0;
	int setting=0;
	int color;
	short num;
public:
	//Constructor
	Block() {
		//random setting
		srand((unsigned int)time(NULL));
		
		setting = 3;
		color = setting + 8;

		//foam with setting
		//foamat (0) - ��
		if (setting == 0) {
			block =
			{
				{
					{ 0,1,0,0 },
					{ 0,1,0,0 },
					{ 0,1,0,0 },
					{ 0,1,0,0 }
				},
				{
					{ 0,0,0,0 },
					{ 0,0,0,0 },
					{ 1,1,1,1 },
					{ 0,0,0,0 }
				},
				{
					{ 0,1,0,0 },
					{ 0,1,0,0 },
					{ 0,1,0,0 },
					{ 0,1,0,0 }
				},
				{
					{ 0,0,0,0 },
					{ 0,0,0,0 },
					{ 1,1,1,1 },
					{ 0,0,0,0 }
				}
			};
			num = 4;
		}
		//foamat (1) - ��
		if (setting == 1) {
			block =
			{
				{
					{ 0,0,0 },
					{ 0,1,0 },
					{ 1,1,1 }
				},
				{
					{ 1,0,0 },
					{ 1,1,0 },
					{ 1,0,0 }
				},
				{
					{ 1,1,1 },
					{ 0,1,0 },
					{ 0,0,0 }
				},
				{
					{ 0,0,1 },
					{ 0,1,1 },
					{ 0,0,1 }
				}
			};
			num = 3;
		}
		//foamat (2) - ��
		if (setting == 2) {
			block =
			{
				{
					{ 0,0,0 },
					{ 1,0,0 },
					{ 1,1,1 }
				},
				{
					{ 1,1,0 },
					{ 1,0,0 },
					{ 1,0,0 }
				},
				{
					{ 1,1,1 },
					{ 0,0,1 },
					{ 0,0,0 }
				},
				{
					{ 0,0,1 },
					{ 0,0,1 },
					{ 0,1,1 }
				}
			};
			num = 3;
		}
		//foamat (3) - �� (mirror)
		if (setting == 3) {
			block =
			{
				{
					{ 0,0,0 },
					{ 0,0,1 },
					{ 1,1,1 }
				},
				{
					{ 1,0,0 },
					{ 1,0,0 },
					{ 1,1,0 }
				},
				{
					{ 1,1,1 },
					{ 1,0,0 },
					{ 0,0,0 }
				},
				{
					{ 0,1,1 },
					{ 0,0,1 },
					{ 0,0,1 }
				}
			};
			num = 3;
		}
		//foamat (4) - ��
		if (setting == 4) {
			block =
			{
				{
					{ 1,1 },
					{ 1,1 }
				},
				{
					{ 1,1 },
					{ 1,1 }
				},
				{
					{ 1,1 },
					{ 1,1 }
				},
				{
					{ 1,1 },
					{ 1,1 }
				}
			};
			num = 2;
		}
		//foamat (5) - z
		if (setting == 5) {
			block =
			{
				{
					{ 0,0,0 },
					{ 1,1,0 },
					{ 0,1,1 }
				},
				{
					{ 0,0,1 },
					{ 0,1,1 },
					{ 0,1,0 }
				},
				{
					{ 0,0,0 },
					{ 1,1,0 },
					{ 0,1,1 }
				},
				{
					{ 0,0,1 },
					{ 0,1,1 },
					{ 0,1,0 }
				}
			};
			num = 3;
		}
		//foamat (6) - z (mirror)
		if (setting == 6) {
			block =
			{
				{
					{ 0,0,0 },
					{ 0,1,1 },
					{ 1,1,0 }
				},
				{
					{ 1,0,0 },
					{ 1,1,0 },
					{ 0,1,0 }
				},
				{
					{ 0,0,0 },
					{ 0,1,1 },
					{ 1,1,0 }
				},
				{
					{ 1,0,0 },
					{ 1,1,0 },
					{ 0,1,0 }
				}
			};
			num = 3;
		}
	}
	//Rotation
	void rotate() {
		rotation++;
		if (rotation >= 4) rotation = 0;
	}
	//getter
	inline int getNum() {
		return num;
	}
	inline short getVec(int i, int j) {
		return block[rotation][i][j];
	}
	inline int getRot() {
		return rotation;
	}
	inline int getCol() {
		return color;
	}
	//setter
	inline void setVec(int i, int j, int x) {
		block[rotation][i][j] = x;
	}
	inline void setRot(int rot) {
		if (rot == 0) rotation = 3;
		else rotation--;
	}
	//printBlock
	void printBlock(GameManager gm) {
		short end = x + num * 2 - 2; // ������ �� ��ǥ, x(20)->end(26) ... �ڱ� �ڽ��� ������ �����ϰ� +num �ؾ��ϱ� ����
									 //overlap�˻��, �迭�� ��ġ�� ���� ��ǥ�� ���� (Virtual)

		for (int i = 0; i < num; i++) {
			gotoXY(x, y + i);
			for (int j = 0; j < num; j++) {
				if (block[rotation][i][j] == 0) cout << "  ";
				else if (block[rotation][i][j] == 1) {
					setColor(color);
					cout << "��";
				}
				else if (block[rotation][i][j] == 2) {
					setColor(15);
					cout << "�� ";
				}
				else if (block[rotation][i][j] == 3) {
					if(end >= 8+ MAX_WIDTH){
						cout << "  ";
					}
					else 
					{
						int Row = y + i - 3;
						int Col = (x - 8) / 2 + j;
						setColor(gm.getMapCol(Row, Col));
						cout << "��";
					}
				}
				else if (block[rotation][i][j] == 4) {
					setColor(15);
					cout << "��";
				}
				setColor(color);
			}
		}
	}
	//reset
	void reset() {
		for (int i = 0; i < num; i++) {
			for (int j = 0; j < num; j++) {
				if (block[rotation][i][j] >= 2) block[rotation][i][j] = 0;
			}
		}
	}
};