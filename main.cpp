#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <conio.h>
#include <time.h>

using namespace std;

//global porperties
const int KEY_LEFT = 'a', KEY_RIGHT = 'd', KEY_UP = 'w', KEY_DOWN = 's';
const int HEIGHT = 25, WIDTH = 40;
bool hasntWon = true;
int hp = 100;
unsigned char healthbar[10] = { '@', '@', '@', '@', '@', '@', '@', '@', '@', '@' };

enum DirectionX {Right, Left};
enum DirectionY {Up, Down};

//The player struct
struct Player
{
	char sprite;	//Player symbol
	int x;			//x and y positions of the player
	int y;
};

//The enemy struct
struct Enemy
{
	char sprite;	//Enemy symbol
	int x;			//x and y positions of the enemy
	int y;
	DirectionX pastDirX;
	DirectionY pastDirY;
};

unsigned char moveEnemy(Enemy& enemy, unsigned char maze[HEIGHT][WIDTH], int i, int j, string& smove, bool& nc)
{
	if (nc)
	{
		//possible up, down, left, right
		bool psbUp = false;
		bool psbDown = false;
		bool psbLeft = false;
		bool psbRight = false;

		//is it possible to go to the given directions?
		if (maze[enemy.y][enemy.x + 1] != '#')	//RIGHT
			psbRight = true;
		if (maze[enemy.y][enemy.x - 1] != '#')	//LEFT
			psbLeft = true;
		if (maze[enemy.y + 1][enemy.x] != '#')	//DOWN, remember that (x, y) = (0, 0) is the top left corner
			psbDown = true;
		if (maze[enemy.y - 1][enemy.x] != '#')	//UP
			psbUp = true;

		//movement tracking
		enum Mv {up, right, down, left, NA};
		Mv move = NA;

		//previous movement
		if (smove == "up")
			move = up;
		else if (smove == "right")
			move = right;
		else if (smove == "down")
			move = down;
		else if (smove == "left")
			move = left;
		else
			move = NA;

		//ENEMY MOVEMENT IN X
		//if previous movement in x was right, but it's not possible to move right
		if (enemy.pastDirX == Right && !psbRight)
		{
			if (enemy.pastDirY == Up)	//the past movement in Y is Up
			{
				if (psbUp)	//possible to move up
					move = up;
				else if (psbDown && move != up) //possible to move down as long as we didnt come from up
					move = down;
				else if (psbLeft)	//possible to move left
					move = left;
				else			//no other choice, move down
					move = down;
			}
			else	//the past movement in Y is Down
			{
				if (psbDown)	//possible to move down
					move = down;
				else if (psbUp && move != down) //possible to move up as long as we didnt come from down
					move = up;
				else if (psbLeft)	//possible to move left
					move = left;
				else			//no other choice, move up
					move = up;
			}
		}
		//if the enemy is going right and can still go right
		else if (enemy.pastDirX == Right && psbRight)	
			move = right;	
		//if previous movement in x was left, but it's not possible to move left
		else if (enemy.pastDirX == Left && !psbLeft)
		{
			if (enemy.pastDirY == Up)	//the past movement in Y is Up
			{
				if (psbUp)	//possible to move up
					move = up;
				else if (psbDown && move != up) //possible to move down as long as we didnt come from up
					move = down;
				else if (psbRight)	//possible to move left
					move = right;
				else			//no other choice, move down
					move = down;
			}
			else	//the past movement in Y is Down
			{
				if (psbDown)	//possible to move down
					move = down;
				else if (psbUp && move != down) //possible to move up as long as we didnt come from down
					move = up;
				else if (psbRight)	//possible to move left
					move = right;
				else			//no other choice, move up
					move = up;
			}
		}
		//if the enemy is going left and can still go left
		else if (enemy.pastDirX == Left && psbLeft)
			move = left;

		//ENEMY MOVEMENT IN Y
		//if previous movement in y was up, but it's not possible to move up
		if (enemy.pastDirY == Up && !psbUp)
		{
			if (enemy.pastDirX == Right)	//the past movement in X is Right
			{
				if (psbRight)	//possible to move right
					move = right;
				else if (psbLeft && move != right) //possible to move left as long as we didnt come from right
					move = left;
				else if (psbDown)	//possible to move down
					move = down;
				else			//no other choice, move left
					move = left;
			}
			else	//the past movement in X is Left
			{
				if (psbLeft)	//possible to move left
					move = left;
				else if (psbRight && move != left) //possible to move right as long as we didnt come from left
					move = right;
				else if (psbDown)	//possible to move down
					move = down;
				else			//no other choice, move right
					move = right;
			}
		}
		//if the enemy is going up and can still go up
		else if (enemy.pastDirY == Up && psbUp)
			move = up;
		//if previous movement in y was down, but it's not possible to move down
		if (enemy.pastDirY == Down && !psbDown)
		{
			if (enemy.pastDirX == Right)	//the past movement in X is Right
			{
				if (psbRight)	//possible to move right
					move = right;
				else if (psbLeft && move != right) //possible to move left as long as we didnt come from right
					move = left;
				else if (psbUp)		//possible to move up
					move = up;
				else			//no other choice, move left
					move = left;
			}
			else	//the past movement in X is Left
			{
				if (psbLeft)	//possible to move left
					move = left;
				else if (psbRight && move != left) //possible to move right as long as we didnt come from left
					move = right;
				else if (psbUp)		//possible to move up
					move = up;
				else			//no other choice, move right
					move = right;
			}
		}
		//if the enemy is going down and can still go down
		else if (enemy.pastDirY == Down && psbDown)
			move = down;

		//what is the said movement
		switch (move)
		{
			case up:	//set variables accordingly
				maze[enemy.y][enemy.x] = ' ';
				enemy.pastDirY = Up;
				smove = "up";
				enemy.y--;
				break;
			case right:
				maze[enemy.y][enemy.x] = ' ';
				enemy.pastDirX = Right;
				smove = "right";
				enemy.x++;
				break;
			case down:
				maze[enemy.y][enemy.x] = ' ';
				enemy.pastDirY = Down;
				smove = "down";
				enemy.y++;
				break;
			case left:
				maze[enemy.y][enemy.x] = ' ';
				enemy.pastDirX = Left;
				smove = "left";
				enemy.x--;
				break;
			case NA:
				smove = "NA";
				break;
		}
	}

	nc = false;
	return maze[i][j];
}

int main()
{
	srand (time(NULL));

	unsigned char maze[HEIGHT][WIDTH] = {
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', '#', '#', '#', '#', '#', ' ', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', ' ', '#', ' ', '#', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', ' ', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', ' ', '#', '#', '#', ' ', '#', ' ', '#', ' ', ' ', ' ', ' ', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', ' ', ' ', ' ', '#', ' ', ' ', ' ', '#', ' ', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', ' ', '#', '#', '#', ' ', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', ' ', '#', ' ', '#', ' ', ' ', ' ', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', ' ', ' ', ' ', ' ', ' ', '#', ' ', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', '#', '#', '#', '#', ' ', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', ' '},
		{'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
	};

	//create player and set prpoerties
	Player player;
	player.sprite = '@';
	player.x = 0;
	player.y = 1;

	//====================================ENEMY SPAWNING=======================
	//create enemy1 and set properties
	Enemy e1;
	e1.sprite = '<';
	e1.x = rand() % WIDTH;
	e1.y = rand() % HEIGHT;
	e1.pastDirX = Left;
	e1.pastDirY = Up;

	//make sure enemy does not spawn on a wall
	while (maze[e1.y][e1.x] != ' ')
	{
		e1.x = rand() % WIDTH;
		e1.y = rand() % HEIGHT;
	}

	//if enemy spawn position is blank, change it to its sprite
	if (maze[e1.y][e1.x] == ' ')
		maze[e1.y][e1.x] = e1.sprite;
	//====================================ENEMY SPAWNING=========================

	//enemies' previous moves declared
	string prevmove1 = "NA";
	//--Add more for more enemies--

	//the game is not ended
	while (hasntWon)
	{
		//Enemies
		maze[e1.y][e1.x] = e1.sprite;

		//The player
		if (maze[player.y][player.x] == ' ')
			maze[player.y][player.x] = player.sprite;
		else if (maze[player.y][player.x] == '<')		//Change if other enemy sprites come in
			maze[player.y][player.x] = '%';

		//Printing the current status of the maze
		for (int y = 0; y < HEIGHT; y++)
		{
			cout << endl;
			for (int x = 0; x < WIDTH; x++)
			{
				cout << maze[y][x];
			}
		}
		cout << endl;

		char key = _getch();	//get input

		//player action
		switch (key)
		{
			case KEY_UP:
				if (maze[player.y - 1][player.x] != '#')
				{
					maze[player.y][player.x] = ' ';
					if (maze[player.y - 1][player.x] == '<')
						hp -= 20;
					player.y--;
				}
				break;
			case KEY_RIGHT:
				if (maze[player.y][player.x + 1] != '#')
				{
					maze[player.y][player.x] = ' ';
					if (maze[player.y][player.x + 1] == '<')
						hp -= 20;
					player.x++;
				}
				break;
			case KEY_DOWN:
				if (maze[player.y + 1][player.x] != '#')
				{
					maze[player.y][player.x] = ' ';
					if (maze[player.y + 1][player.x] == '<')
						hp -= 20;
					player.y++;
				}
				break;
			case KEY_LEFT:
				if (maze[player.y][player.x - 1] != '#')
				{
					maze[player.y][player.x] = ' ';
					if (maze[player.y][player.x - 1] == '<')
						hp -= 20;
					player.x--;
				}
				break;
			case 'q':
				exit(0);
				break;
		}
		//player action over

		//enemy action
		bool once1 = true;

		for (int i = 0; i < HEIGHT; i++)
		{
			for (int j = 0; j < WIDTH; j++)
			{
				maze[i][j] = moveEnemy(e1, maze, i, j, prevmove1, once1);	//do this for other enemies as well
			}
		}

		//decrease hp if player collides with an enemy
		if (player.x == e1.x && player.y == e1.y)
			hp -= 30;

		if (hp <= 0)
		{
			cout << "\n\nYou lost :(\n" << endl;
			system("PAUSE");
			hasntWon = false;
		}

		if (player.x == WIDTH - 1)
		{
			cout << "\n\nYou win :)\n" << endl;
			system("PAUSE");
			hasntWon = false;
		}

		system("CLS");
	}
}