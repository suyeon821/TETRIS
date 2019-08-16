#pragma once
#include "Block.h"
#include <Windows.h>
#include <process.h>
#include <iostream>
using namespace std;

class Game : public Console {
private:
	Desk desk;
	Block *block;
	GameManager GM;
	short num;
	int rotation;
	//Thread (run)
	static unsigned int __stdcall run(void *arg) {
		Game *game = (Game*)arg;
		while (true) {
			game->move();
			Sleep(200 + 1000 / game->GM.getLv()); // +@ 로 기본 speed 유지시켜, 밸런스 조정가능, mode (easy, normal, hard) 난이도별 기본값 다르게끔. 난이도는 main()에서 입력받는다.
		}
		return 0;
	}
public:
	//Constructor (initialization)
	Game() {
		x = 14; //14
		y = 3;  //3
		direction = 1; // defalt = 1 (always down!)
		block = new Block();
		num = block->getNum();
		rotation = block->getRot();
	}
	//setKey
	void setDirection(int i) {
		direction = i;
	}

	//Move (direction default = 1 유지)
	void move() {
		//up-rotate
		if (direction == 38) {
			block->rotate();
			rotation = block->getRot();
			y++;
		}
		//Defalt Down
		else if (direction == 1) {
			y++;
		}
		//Additional Down
		else if (direction == 40) {
			y += 2;
		}
		//Left
		else if (direction == 37) {
			x -= 2;
			y++;

		}
		//Right
		else if (direction == 39) {
			x += 2;
			y++;
		}

		//Buffer (L/R)
		setBuf(x, y); // short &a = x; short &b = y;

					  //ClearScreen
		system("cls");
		//printScreen
		desk.printDesk(GM);

		//setBlockPos
		block->setXY(x, y);
		//printBlock
		block->printBlock(GM);
		block->reset();

		//default
		gotoXY(0, 0);
		direction = 1;
	}
	//Thread (start)
	void start() {
		_beginthreadex(NULL, 0, run, (void*)this, 0, NULL);
	}

	//해당 걸리는 줄에 블럭이 존재하는지 검사 후, 블럭이 없으면 틀이 지워지지 않도록 해당 좌표의 블럭을 틀과 같은 특수기호로 바꿔줌 (치환)
	//블럭이 있으면 방향키로 인해 수정된 좌표를 바로잡음 & 치환상태에서 예외 방향키를 눌렀을 때 reset()으로 지워진 치환상태를 다시 유지
	//예외 방향키? : 예를 들어, 왼쪽 끝에서 왼쪽 키를 눌렀을 때
	void setBuf(short &x, short &y) {
		short check; //몇 열을 검사할 것인지 결정
		short end = x + num * 2 - 2; // 오른쪽 끝 좌표, x(20)->end(26) ... 자기 자신의 라인을 제외하고 +num 해야하기 때문
									 //overlap검사시, 배열과 겹치는 블럭의 좌표를 저장 (Virtual)
		short virtualX = x; //블럭의 겹친 부분의 x좌표   
		short virtualY = y; //블럭의 겹친 부분의 y좌표
		bool test = false;  //예외처리용 변수 : test가 true면 overlap(right)가 되었다는 의미이므로, 해당 경우 조건적으로 right buffer 를 제한적으로 적용
		bool temp = true; //temp 변수를 사용할지 안할지 결정하는 플래그
		short tempX; //virtualXY가 이중오버랩에서 xy로 갱신될 때, 체크인 인수문제를 해결하기 위해 임시적으로 virtualXY 값을 저장하는 변수
		short tempY;

		//Bottom
		if (isOverlap(virtualX, virtualY)) { //블럭과 해당 이차원배열이 충돌할 경우
											 //행한 방향키에 따라 이전 상태로 복구 (걸린 블럭의 해당 좌표를 virtual(x,y)에 저장)
			switch (direction) {
			case 38: //up
					 //일차적인 문제해결 (원상복구, 재설정)
				y--;
				virtualY--;
				block->setRot(rotation);
				rotation = block->getRot();
				virtualX = x;
				virtualY = y;
				end = x + num * 2 - 2;

				//한 칸 아래로 가게끔 (원래 move를 거치면 아래로 한 칸은 가니까)
				y++;
				virtualY++;

				//재 overlap 검사
				if (isOverlap(virtualX, virtualY)) {
					y--;
					virtualY--;
					temp = false;
					break;
				}

				//overlap 미해당시 check_in()을 건너뛸 수 있도록
				goto Label;

			case 37: //left
				x += 2;
				y--;
				virtualX += 2;
				virtualY--;
				break;
			case 39: //right
				test = true;
				x -= 2;
				virtualX -= 2;
				y--;
				virtualY--;
				//오른쪽 4by4 오버플로우 예외처리
				if (end >= 8 + MAX_WIDTH && (y + num - 1) < 2 + MAX_HEIGHT) {
					y++;
					virtualY++;
				}
				break;
			case 40: //additional down
				y--;
				virtualY--;

				tempX = virtualX;
				tempY = virtualY;

				virtualX = x;
				virtualY = y;

				//이중오버랩
				if (isOverlap(virtualX, virtualY)) {
					temp = false;
					y--;
					virtualY--;
				}

				if (temp) {
					virtualX = tempX;
					virtualY = tempY;
				}

				break;
			default:
				y--;
				virtualY--;
			}
			//바로 블럭'아래행 검사
			if (GM.getMap(virtualY - 3 + 1, (virtualX - 8) / 2) == 1) {
				check_in();
				return;
			}
		}

	Label:

		//LEFT
		if (x <= 6) {
			check = (6 - x) / 2;
			//검사
			for (int i = 0; i < num; i++) {
				if (block->getVec(i, check) == 1) { //true
													//치환유지
					if (x < 6) {
						for (int i = 0; i < num; i++) {
							block->setVec(i, check - 1, 2);
						}
					}
					x += 2;
					return;
				}
			}
			//치환
			for (int i = 0; i < num; i++) {
				block->setVec(i, check, 2);
			}
			return;
		}
		//Right
		if (end >= 8 + MAX_WIDTH) {
			check = (end - (8 + MAX_WIDTH)) / 2;
			//검사
			for (int i = 0; i < num; i++) {
				if (block->getVec(i, num - check - 1) == 1) { //true
															  //치환유지
					if (end > 8 + MAX_WIDTH) {
						//오른쪽 끝에서 ㅣ -> ㅡ 로 변하는 경우, end (30) 으로 x-=2 만 되면 오른쪽 끝 블럭이 잘림, so -4 & return 으로 문제 해결 
						if (block->getNum() == 4 && block->getRot() % 2 == 1) {
							if (!test) { x -= 4; }
							return;
						}
						//그렇지 않은 경우 치환유지
						else {
							for (int i = 0; i < num; i++) {
								block->setVec(i, num - check, 2);
							}
						}
					}
					if (!test) { x -= 2; }
					return;
				}
			}
			//치환
			for (int i = 0; i < num; i++) {
				block->setVec(i, num - check - 1, 2);
			}
			return;
		}
	}
	//블럭과 블럭에 해당하는 배열이 겹치는지
	//인자 : Virtual(x,y)
	bool isOverlap(short &x, short &y) {
		for (int i = 0; i < num; i++) {
			for (int j = 0; j < num; j++) {
				if (GM.getMap(y - 3 + i, (x - 8) / 2 + j) == 1) {
					//map == 1 && block == 1
					if (block->getVec(i, j) == 1) {
						//겹친 블럭의 좌표를 알아냄
						x += j * 2;
						y += i;
						return true;
					}
					//map == 1 && block == 0
					else {
						if (y - 3 + i == 20) { block->setVec(i, j, 4); } //배열의 마지막 행인 경우 예외처리
						else if (x + num * 2 - 2 > 8 + MAX_WIDTH) { block->setVec(i, j, 0); } //배열의 마지막 열인 경우 예외처리 (오버플로우)
						else { block->setVec(i, j, 3); }
					}
				}
			}
		}
		return false;
	}
	void check_in() {
		for (int i = 0; i < num; i++) {
			for (int j = 0; j < num; j++) {
				if (block->getVec(i, j) == 1) {
					GM.setMap(y - 3 + i, (x - 8) / 2 + j, 1);
					GM.setMapCol(y - 3 + i, (x - 8) / 2 + j, block->getCol());
				}
			}
		}

		check_line();

		delete block;
		block = new Block();
		x = 14;
		y = 3;
		num = block->getNum();
		rotation = block->getRot();

		//종료조건 : 새로운 블럭 생성 직후 오버랩 검사
		if (isOverlap(x, y)) desk.overDesk();
	}

	void check_line() { //line 체크, 파괴, 갱신
		short high = y - 3;            //block 맨 위' 배열 인덱스 (위 행)
		short low = y + (num - 1) - 3; //block 맨 아래' 배열 인덱스 (아래 행)
		bool isFull = true;

		//예외처리 (블럭의 맨 아래행이 비어있을 때 flag(true) 되면서, low를 줄여줌 
		bool flag = false;
		for (int i = 0; i < num; i++) {
			if (block->getVec(num - 1, i) == 1) { flag = false; break; }
			else flag = true;
		}
		if (flag) { low--; }

		//block에 해당하는 아래 행부터 위 행까지 check_line 할 것
		for (short line = low; line >= high; line--) {
			int i = 0;
			while (isFull && i < MAX_WIDTH / 2) {
				if (GM.getMap(line, i) == 0) {
					isFull = false;
					break;
				}
				i++;
			}
			//isFull == true (라인 파괴)
			if (isFull) {
				for (int i = 0; i < MAX_WIDTH / 2; i++) {
					GM.setMap(line, i, 0); //해당 라인 공백으로
				}
				GM.plusScore();
				GM.moveDown(line);
				line++;
			}
			//isFull == false (다음 라인 검사 by for문)
			else {
				isFull = true;
				continue;
			}
		}
	}
};