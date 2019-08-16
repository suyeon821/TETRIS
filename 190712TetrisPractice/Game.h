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
			Sleep(200 + 1000 / game->GM.getLv()); // +@ �� �⺻ speed ��������, �뷱�� ��������, mode (easy, normal, hard) ���̵��� �⺻�� �ٸ��Բ�. ���̵��� main()���� �Է¹޴´�.
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

	//Move (direction default = 1 ����)
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

	//�ش� �ɸ��� �ٿ� ���� �����ϴ��� �˻� ��, ���� ������ Ʋ�� �������� �ʵ��� �ش� ��ǥ�� ���� Ʋ�� ���� Ư����ȣ�� �ٲ��� (ġȯ)
	//���� ������ ����Ű�� ���� ������ ��ǥ�� �ٷ����� & ġȯ���¿��� ���� ����Ű�� ������ �� reset()���� ������ ġȯ���¸� �ٽ� ����
	//���� ����Ű? : ���� ���, ���� ������ ���� Ű�� ������ ��
	void setBuf(short &x, short &y) {
		short check; //�� ���� �˻��� ������ ����
		short end = x + num * 2 - 2; // ������ �� ��ǥ, x(20)->end(26) ... �ڱ� �ڽ��� ������ �����ϰ� +num �ؾ��ϱ� ����
									 //overlap�˻��, �迭�� ��ġ�� ���� ��ǥ�� ���� (Virtual)
		short virtualX = x; //���� ��ģ �κ��� x��ǥ   
		short virtualY = y; //���� ��ģ �κ��� y��ǥ
		bool test = false;  //����ó���� ���� : test�� true�� overlap(right)�� �Ǿ��ٴ� �ǹ��̹Ƿ�, �ش� ��� ���������� right buffer �� ���������� ����
		bool temp = true; //temp ������ ������� ������ �����ϴ� �÷���
		short tempX; //virtualXY�� ���߿��������� xy�� ���ŵ� ��, üũ�� �μ������� �ذ��ϱ� ���� �ӽ������� virtualXY ���� �����ϴ� ����
		short tempY;

		//Bottom
		if (isOverlap(virtualX, virtualY)) { //���� �ش� �������迭�� �浹�� ���
											 //���� ����Ű�� ���� ���� ���·� ���� (�ɸ� ���� �ش� ��ǥ�� virtual(x,y)�� ����)
			switch (direction) {
			case 38: //up
					 //�������� �����ذ� (���󺹱�, �缳��)
				y--;
				virtualY--;
				block->setRot(rotation);
				rotation = block->getRot();
				virtualX = x;
				virtualY = y;
				end = x + num * 2 - 2;

				//�� ĭ �Ʒ��� ���Բ� (���� move�� ��ġ�� �Ʒ��� �� ĭ�� ���ϱ�)
				y++;
				virtualY++;

				//�� overlap �˻�
				if (isOverlap(virtualX, virtualY)) {
					y--;
					virtualY--;
					temp = false;
					break;
				}

				//overlap ���ش�� check_in()�� �ǳʶ� �� �ֵ���
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
				//������ 4by4 �����÷ο� ����ó��
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

				//���߿�����
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
			//�ٷ� ��'�Ʒ��� �˻�
			if (GM.getMap(virtualY - 3 + 1, (virtualX - 8) / 2) == 1) {
				check_in();
				return;
			}
		}

	Label:

		//LEFT
		if (x <= 6) {
			check = (6 - x) / 2;
			//�˻�
			for (int i = 0; i < num; i++) {
				if (block->getVec(i, check) == 1) { //true
													//ġȯ����
					if (x < 6) {
						for (int i = 0; i < num; i++) {
							block->setVec(i, check - 1, 2);
						}
					}
					x += 2;
					return;
				}
			}
			//ġȯ
			for (int i = 0; i < num; i++) {
				block->setVec(i, check, 2);
			}
			return;
		}
		//Right
		if (end >= 8 + MAX_WIDTH) {
			check = (end - (8 + MAX_WIDTH)) / 2;
			//�˻�
			for (int i = 0; i < num; i++) {
				if (block->getVec(i, num - check - 1) == 1) { //true
															  //ġȯ����
					if (end > 8 + MAX_WIDTH) {
						//������ ������ �� -> �� �� ���ϴ� ���, end (30) ���� x-=2 �� �Ǹ� ������ �� ���� �߸�, so -4 & return ���� ���� �ذ� 
						if (block->getNum() == 4 && block->getRot() % 2 == 1) {
							if (!test) { x -= 4; }
							return;
						}
						//�׷��� ���� ��� ġȯ����
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
			//ġȯ
			for (int i = 0; i < num; i++) {
				block->setVec(i, num - check - 1, 2);
			}
			return;
		}
	}
	//���� ���� �ش��ϴ� �迭�� ��ġ����
	//���� : Virtual(x,y)
	bool isOverlap(short &x, short &y) {
		for (int i = 0; i < num; i++) {
			for (int j = 0; j < num; j++) {
				if (GM.getMap(y - 3 + i, (x - 8) / 2 + j) == 1) {
					//map == 1 && block == 1
					if (block->getVec(i, j) == 1) {
						//��ģ ���� ��ǥ�� �˾Ƴ�
						x += j * 2;
						y += i;
						return true;
					}
					//map == 1 && block == 0
					else {
						if (y - 3 + i == 20) { block->setVec(i, j, 4); } //�迭�� ������ ���� ��� ����ó��
						else if (x + num * 2 - 2 > 8 + MAX_WIDTH) { block->setVec(i, j, 0); } //�迭�� ������ ���� ��� ����ó�� (�����÷ο�)
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

		//�������� : ���ο� �� ���� ���� ������ �˻�
		if (isOverlap(x, y)) desk.overDesk();
	}

	void check_line() { //line üũ, �ı�, ����
		short high = y - 3;            //block �� ��' �迭 �ε��� (�� ��)
		short low = y + (num - 1) - 3; //block �� �Ʒ�' �迭 �ε��� (�Ʒ� ��)
		bool isFull = true;

		//����ó�� (���� �� �Ʒ����� ������� �� flag(true) �Ǹ鼭, low�� �ٿ��� 
		bool flag = false;
		for (int i = 0; i < num; i++) {
			if (block->getVec(num - 1, i) == 1) { flag = false; break; }
			else flag = true;
		}
		if (flag) { low--; }

		//block�� �ش��ϴ� �Ʒ� ����� �� ����� check_line �� ��
		for (short line = low; line >= high; line--) {
			int i = 0;
			while (isFull && i < MAX_WIDTH / 2) {
				if (GM.getMap(line, i) == 0) {
					isFull = false;
					break;
				}
				i++;
			}
			//isFull == true (���� �ı�)
			if (isFull) {
				for (int i = 0; i < MAX_WIDTH / 2; i++) {
					GM.setMap(line, i, 0); //�ش� ���� ��������
				}
				GM.plusScore();
				GM.moveDown(line);
				line++;
			}
			//isFull == false (���� ���� �˻� by for��)
			else {
				isFull = true;
				continue;
			}
		}
	}
};