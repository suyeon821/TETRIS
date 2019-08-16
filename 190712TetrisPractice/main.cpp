#include "Desk.h"
#include "Game.h"
#include "KeyEvent.h"
#include "Console.h"

using namespace std;
int main() {
	Game* game = new Game();
	KeyEvent key;
	key.start(game);

	game->start();
	while (true) {
		
	}

}