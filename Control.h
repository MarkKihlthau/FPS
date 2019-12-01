
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
using namespace std;

class Control {
public:
	int nScreenWidth = 120; //Screen Size
	int nScreenHeight = 40;

	float fPlayerX = 1.0f; //player X position
	float fPlayerY = 14.5f; //player Y position
	float fPlayerA = 1.5f; //player look Angle

	int nMapHeight = 16; //Map Size
	int nMapWidth = 16;

	float fFOV = 3.14159 / 4; //field of view
	float fDepth = 16.0f;
	float fSpeed = 5.0f; //player movement speed


	int item_pickup;
	int items[16][16]; //array for item location
	int enemies[16][16]; //array for enemy location
	int GameLoopCount = 0;
	const int ActionLoopCount = 40; //loop count for how often player can shoot
	float bullet;
	int HitX; //X location of gunshot hit
	int HitY; //Y location of gunshot hit

	Enemy E1; //Ghost enemy
	int E1_hp = E1.get_health();
	int EnemyX = 9; //Enemy starting X position
	int EnemyY = 6; //Enemy starting Y position

					//Strings for weapon name
	string wname = "";
	wchar_t w1[10] = L"None";
	wchar_t w2[10] = L"Pistol";
	wchar_t w3[10] = L"RailGun";

	//Player Stats
	int health = 1;
	int damage = 0;
	wchar_t weapon[10];
	int capacity = 0;
	int reload = 0;
	bool victory = false;
	bool displayVictory = false;

	int hit_enemy = 0;
	int EnemyMoveCount = 0; //Counter for enemy movement
	int EnemyMoveSpeed = 200; //How fast the enemy moves

	wstring map; //string that holds the map

	void Controls(float fElapsedTime, wstring map, wchar_t* screen);
	void Environment(wchar_t* screen, wstring map);
	void MiniMap(wchar_t* screen);
	void CheckWeaponName();
	void WeaponArt(wchar_t* screen);
	void MoveEnemy();
};