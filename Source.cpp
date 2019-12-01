#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <Windows.h>
#include <chrono>
#include <time.h>
#include <cstdlib>
#include "Item.h"
#include "Enemy.h"
#include "Control.h"
using namespace std;

int main()
{
	Control GameControl;
	int nScreenWidth = GameControl.nScreenWidth;
	int nScreenHeight = GameControl.nScreenHeight;
	wstring map = GameControl.map;
	
	//Create Screen Buffer
	wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	srand(time(NULL)); //For random number generation
	
	//Load map
	map += L"################";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#......####....#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#"; //7
	map += L"#...######.....#";
	map += L"#..............#";
	map += L"#........#######";
	map += L"#............H.#";
	map += L"#..............#";
	map += L"#############..#";
	map += L"#.........H...W#";
	map += L"################"; //15

	GameControl.items[10][14] = 1; //Healthpack
	GameControl.items[13][11] = 1;
	GameControl.items[14][14] = 2; //Weapon

	GameControl.enemies[GameControl.EnemyX][GameControl.EnemyY] = 1;

	auto TimeP1 = chrono::system_clock::now();
	auto TimeP2 = chrono::system_clock::now();
	auto TimeP3 = chrono::system_clock::now();
	float fElapsedGameTime = 0;

	//Game Loop
	while (1)
	{
		//Create Clock for each Game Loop
		TimeP2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = TimeP2 - TimeP1;
		TimeP1 = TimeP2;
		float fElapsedTime = elapsedTime.count();

		//Controls
		GameControl.Controls(fElapsedTime, map, screen);

		//Dispay walls and shading
		GameControl.Environment(screen, map);

		//Display Weapon Art
		GameControl.WeaponArt(screen);
		
		//Move the Ghost
		int EnemyMoveCount = GameControl.EnemyMoveCount;
		int EnemyMoveSpeed = GameControl.EnemyMoveSpeed;
		bool victory = GameControl.victory;
		if (EnemyMoveCount == EnemyMoveSpeed && victory == false)
		{
			GameControl.MoveEnemy();
			EnemyMoveCount = 0;
		}

		//Display Stats
		//swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A%3.2f FPS=%3.2f", fPlayerX, fPlayerY, fPlayerA, 1.0f / fElapsedTime);
		GameControl.CheckWeaponName();
		int health = GameControl.health;
		wchar_t *weapon = GameControl.weapon;
		int capacity = GameControl.capacity;
		int reload = GameControl.reload;
		int HitX = GameControl.HitX;
		int HitY = GameControl.HitY;
		int E1_hp = GameControl.E1_hp;
		int EnemyX = GameControl.EnemyX;
		int EnemyY = GameControl.EnemyY;
		bool displayVictory = GameControl.displayVictory;
		int GameLoopCount = GameControl.GameLoopCount;
		int ActionLoopCount = GameControl.ActionLoopCount;
		if (victory == false)
		{
			swprintf_s(screen, 100, L"Health:%d Weapon:%s Capacity:%d/Reload:%d HitX:%d HitY:%d GhostHP:%d GX:%d GY:%d",
				health, weapon, capacity, reload, HitX, HitY, E1_hp, EnemyX, EnemyY);
		}
		else
		{
			if (displayVictory == false)
			{
				auto TimeP4 = chrono::system_clock::now();
				chrono::duration<float> elapsedGameTime = TimeP4 - TimeP3;
				fElapsedGameTime = elapsedGameTime.count();
				displayVictory = true;
			}
			swprintf_s(screen, 100, L"You Win! Elapsed Time:%3.2f", fElapsedGameTime);
		}

		//Display Mini-Map
		GameControl.MiniMap(screen);

		//Display Frame 
		screen[nScreenWidth * nScreenHeight - 1] = '/0';
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);

		if (health <= 0)
		{
			break;
		}

		if (GameLoopCount < ActionLoopCount)
			GameLoopCount++;
		if (EnemyMoveCount < EnemyMoveSpeed)
			EnemyMoveCount++;
		
	}
	cout << "You died.";
	char fail;
	while (1)
	{
		cin >> fail;
	}
	return 0;
}
