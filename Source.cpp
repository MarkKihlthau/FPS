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

void Controls(float, wstring, wchar_t*);
void Environment(wchar_t*, wstring);
void MiniMap(wchar_t*);
void CheckWeaponName();
void WeaponArt(wchar_t*);
void MoveEnemy();

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

int main()
{
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

	items[10][14] = 1; //Healthpack
	items[13][11] = 1;

	items[14][14] = 2; //Weapon

	enemies[EnemyX][EnemyY] = 1;

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
		Controls(fElapsedTime, map, screen);

		//Dispay walls and shading
		Environment(screen, map);

		//Display Weapon Art
		WeaponArt(screen);

		//Move the Ghost
		if (EnemyMoveCount == EnemyMoveSpeed && victory == false)
		{
			MoveEnemy();
			EnemyMoveCount = 0;
		}

		//Display Stats
		//swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A%3.2f FPS=%3.2f", fPlayerX, fPlayerY, fPlayerA, 1.0f / fElapsedTime);
		CheckWeaponName();
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
		MiniMap(screen);

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


void Controls(float fElapsedTime, wstring map, wchar_t* screen)
{
	//Handle Rotation
	if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
	{
		fPlayerA -= (3.0f) * fElapsedTime;
	}
	if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
	{
		fPlayerA += (3.0f) * fElapsedTime;
	}

	//Handle Forward and Backward
	if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
	{
		fPlayerX += sinf(fPlayerA) * fSpeed * fElapsedTime;
		fPlayerY += cosf(fPlayerA) * fSpeed * fElapsedTime;

		//Collision Detection
		if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')
		{
			fPlayerX -= sinf(fPlayerA) * fSpeed * fElapsedTime;
			fPlayerY -= cosf(fPlayerA) * fSpeed * fElapsedTime;
		}
	}
	if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
	{
		fPlayerX -= sinf(fPlayerA) * fSpeed * fElapsedTime;
		fPlayerY -= cosf(fPlayerA) * fSpeed * fElapsedTime;

		//Collision Detection
		if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')
		{
			fPlayerX += sinf(fPlayerA) * fSpeed * fElapsedTime;
			fPlayerY += cosf(fPlayerA) * fSpeed * fElapsedTime;
		}
	}

	//Handle Shooting
	if (GetAsyncKeyState((unsigned short)' ') & 0x8000)
	{
		if (capacity > 0 && GameLoopCount == ActionLoopCount)
		{
			capacity = capacity - 1;
			GameLoopCount = 0;
			hit_enemy = 0;
			
			bullet = fPlayerA;
			float fDistanceTo = 0;
			bool bHitWall = false;
			bool bHitEnemy = false;
			bool bBoundary = false;

			float fEyeX = sinf(bullet); //Unit Vector for ray
			float fEyeY = cosf(bullet);

			while ((!bHitWall || !bHitEnemy) && fDistanceTo < fDepth)
			{
				fDistanceTo += 0.1f;

				HitX = (int)(fPlayerX + fEyeX * fDistanceTo); //works if walls are on integer boundries
				HitY = (int)(fPlayerY + fEyeY * fDistanceTo);

				//Test if ray is out of bounds
				if (HitX < 0 || HitX >= nMapWidth || HitY < 0 || HitY >= nMapHeight)
				{
					bHitWall = true;
					fDistanceTo = fDepth;
				}
				else
				{
					//Ray is in bounds, but has hit wall
					if (map[HitY * nMapWidth + HitX] == '#')
					{
						bHitWall = true;
					}
					//Ray is in bounds, but has hit enemy
					else if (map[HitY * nMapWidth + HitX] == 'X')
					{
						bHitEnemy = true;
						hit_enemy = 1;
						E1.hit(damage);
						E1_hp = E1.get_health();
						break;

					}
				}
			}

		}
	}

	//Handle Reload
	if (GetAsyncKeyState((unsigned short)'R') & 0x8000)
	{
		capacity = 16;
	}
}

void Environment(wchar_t* screen, wstring map)
{
	//Ray Casting
	for (int x = 0; x < nScreenWidth; x++)
	{
		// For each column, calculate the projected ray angle into the world
		float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) * fFOV;

		float fDistanceTo = 0;
		bool bHitWall = false;
		bool bBoundary = false;

		float fEyeX = sinf(fRayAngle); //Unit Vector for ray
		float fEyeY = cosf(fRayAngle);

		while (!bHitWall && fDistanceTo < fDepth)
		{
			fDistanceTo += 0.1f;

			int nTestX = (int)(fPlayerX + fEyeX * fDistanceTo); //works if walls are on integer boundries
			int nTestY = (int)(fPlayerY + fEyeY * fDistanceTo);

			//Test if ray is out of bounds
			if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
			{
				bHitWall = true;
				fDistanceTo = fDepth;
			}
			else
			{
				//Ray is in bounds, but has hit wall
				if (map[nTestY * nMapWidth + nTestX] == '#')
				{
					bHitWall = true;

					vector <pair<float, float>> p;

					for (int tx = 0; tx < 2; tx++)
						for (int ty = 0; ty < 2; ty++)
						{
							float vy = (float)nTestY + ty - fPlayerY;
							float vx = (float)nTestX + tx - fPlayerX;
							float d = sqrt(vx*vx + vy*vy);
							float dot = (fEyeX * vx / d) + (fEyeY * vy / d);
							p.push_back(make_pair(d, dot));
						}

					sort(p.begin(), p.end(), [](const pair<float, float> &left, const pair<float, float> &right) {return left.first < right.first; });

					float fBound = 0.01;
					if (acos(p.at(0).second) < fBound) bBoundary = true;
					if (acos(p.at(1).second) < fBound) bBoundary = true;
					if (acos(p.at(2).second) < fBound) bBoundary = true;
				}

				if (map[nTestY * nMapWidth + nTestX] == 'H')
				{
					Item* H1;
					H1 = Item::Create("HealthPack");
					if ((10.0 <= fPlayerX && fPlayerX <= 10.99) && (14.0 <= fPlayerY && fPlayerY <= 14.99)
						&& (items[10][14] == 1))
					{
						item_pickup = H1->pickup();
						if (item_pickup == 1)
						{
							health = 100;
							items[10][14] = 0;
						}
					}

					if ((13.0 <= fPlayerX && fPlayerX <= 13.99) && (11.0 <= fPlayerY && fPlayerY <= 11.99)
						&& (items[13][11] == 1))
					{
						item_pickup = H1->pickup();
						if (item_pickup == 1)
						{
							health = 100;
							items[13][11] = 0;
						}
					}

					
				}

				if (map[nTestY * nMapWidth + nTestX] == 'W')
				{
					Item* W1;
					W1 = Item::Create("Weapon");
					if ((14.0 <= fPlayerX && fPlayerX <= 14.99) && (14.0 <= fPlayerY && fPlayerY <= 14.99)
						&& (items[14][14] == 2))
					{
						item_pickup = W1->pickup();
						W1->set_weapon("Pistol");
						wname = W1->get_name();
						damage = W1->get_damage();
						capacity = W1->get_capacity();
						reload = W1->get_reload();
						if (item_pickup == 2)
						{
							items[14][14] = 0;
						}
					}

				}

				if (map[nTestY * nMapWidth + nTestX] == 'X')
				{
					if ((((float)EnemyX-1.0) <= fPlayerX && fPlayerX <= ((float)EnemyX+1.0) && 
						(((float)EnemyY-1.0) <= fPlayerY && fPlayerY <= ((float)EnemyY+1.0))))
					{
						health = health - E1.strike();
					}
					if (E1.alive() == false)
					{
						enemies[EnemyX][EnemyY] = 0;
						victory = true;
					}

				}

			} //end else

		}

		//Calculate distance to ceiling and floor
		int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceTo);
		int nFloor = nScreenHeight - nCeiling;

		short nShadeWall = ' ';
		short nShadeFloor = ' ';

		if (fDistanceTo <= fDepth / 4.0f)		nShadeWall = 0x2588; //very close
		else if (fDistanceTo < fDepth / 3.0f)	nShadeWall = 0x2593;
		else if (fDistanceTo < fDepth / 2.0f)	nShadeWall = 0x2592;
		else if (fDistanceTo < fDepth)			nShadeWall = 0x2591;
		else									nShadeWall = ' ';	//too far to see

		if (bBoundary)							nShadeWall = ' ';	//blackout

		for (int y = 0; y < nScreenHeight; y++)
		{
			if (y < nCeiling) //ceiling
			{
				screen[y*nScreenWidth + x] = ' ';
			}
			else if (y > nCeiling && y <= nFloor) //wall
			{
				screen[y*nScreenWidth + x] = nShadeWall;
			}
			else //floor
			{
				float b = 1.0f - (((float)y - nScreenHeight / 2.0f) / (float)(nScreenHeight / 2.0f));

				if (b < 0.25)		nShadeFloor = '#';
				else if (b < 0.5)	nShadeFloor = 'x';
				else if (b < 0.75)	nShadeFloor = '-';
				else if (b < 0.9)	nShadeFloor = '.';
				else				nShadeFloor = ' ';
				screen[y*nScreenWidth + x] = nShadeFloor;
			}
		}
	}

}

void MiniMap(wchar_t* screen)
{
	map = L""; //reset map
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

	for (int nx = 0; nx < nMapWidth; nx++)
		for (int ny = 0; ny < nMapWidth; ny++)
		{
			screen[(ny + 1)*nScreenWidth + nx] = map[ny * nMapWidth + (nMapWidth - nx - 1)];
		}
	
	if (items[10][14] == 0) //Healthpack
		screen[1805] = '.';
	if (items[13][11] == 0) //Healthpack
		screen[1442] = '.';
	if (items[14][14] == 0) //Weapon
		screen[1801] = '.';

	screen[((int)fPlayerY + 1) * nScreenWidth + (int)(nMapWidth - fPlayerX)] = 'P';
	if (victory == false)
		screen[(EnemyY + 1) * nScreenWidth + (nMapWidth - EnemyX)] = 'X';
	else
	{
		screen[(EnemyY + 1) * nScreenWidth + (nMapWidth - EnemyX)] = '.';
	}

	screen[1683] = '.';
	//map[220] = '.';
	
	
	for (int nx = 0; nx < nMapWidth; nx++)
		for (int ny = 0; ny < nMapWidth; ny++)
		{
			map[ny * nMapWidth + (nMapWidth - nx - 1)] = screen[(ny + 1)*nScreenWidth + nx];
		}
	

	
}

void CheckWeaponName()
{
	if (wname == "")
		wcscpy_s(weapon, w1);
	else if (wname == "Pistol")
		wcscpy_s(weapon, w2);
	else if (wname == "RailGun")
		wcscpy_s(weapon, w3);
}

void WeaponArt(wchar_t* screen)
{
	
	if (wname == "Pistol" && GameLoopCount >= ActionLoopCount-10) //ready pistol
	{
		short shadeHand = 0x2588;
		short shadeGun = 0x2592;
		screen[4730] = '+'; //bottom left hand
		screen[4750] = '+'; //bottom right hand
		screen[3770] = '+'; //top left hand
		screen[3790] = '+'; //top right hand
		screen[3773] = '+'; //bottom left gun
		screen[3787] = '+'; //bottom right gun
		screen[3053] = '+'; //top left gun
		screen[3067] = '+'; //top right gun
		
		for (int i = 4731; i < 4750; i++)
			screen[i] = '-';
		for (int i = 3771; i < 3790; i++)
			screen[i] = '-';
		for (int i = 3774; i < 3787; i++)
			screen[i] = '-';
		for (int i = 3054; i < 3067; i++)
			screen[i] = '-';
		

		for (int i = 3770 + 120; i < 4730; i = i + 120)
			screen[i] = '|';
		for (int i = 3790 + 120; i < 4750; i = i + 120)
			screen[i] = '|';
		for (int i = 3053 + 120; i < 3773; i = i + 120)
			screen[i] = '|';
		for (int i = 3067 + 120; i < 3787; i = i + 120)
			screen[i] = '|';
		

		for (int i = (4731 - 120); i < (4750 - 120); i++)
			screen[i] = shadeHand;
		for (int i = (4731 - 240); i < (4750 - 240); i++)
			screen[i] = shadeHand;
		for (int i = (4731 - 360); i < (4750 - 360); i++)
			screen[i] = shadeHand;
		for (int i = (4731 - 480); i < (4750 - 480); i++)
			screen[i] = shadeHand;
		for (int i = (4731 - 600); i < (4750 - 600); i++)
			screen[i] = shadeHand;
		for (int i = (4731 - 720); i < (4750 - 720); i++)
			screen[i] = shadeHand;
		for (int i = (4731 - 840); i < (4750 - 840); i++)
			screen[i] = shadeHand;

		for (int i = (3774 - 120); i < (3787 - 120); i++)
			screen[i] = shadeGun;
		for (int i = (3774 - 240); i < (3787 - 240); i++)
			screen[i] = shadeGun;
		for (int i = (3774 - 360); i < (3787 - 360); i++)
			screen[i] = shadeGun;
		for (int i = (3774 - 480); i < (3787 - 480); i++)
			screen[i] = shadeGun;
		for (int i = (3774 - 600); i < (3787 - 600); i++)
			screen[i] = shadeGun;
	}
	
	if (wname == "Pistol" && GameLoopCount <= ActionLoopCount-10) //fired pistol
	{
		short shadeHand = 0x2588;
		short shadeGun = 0x2592;

		//Shade hand
		for (int i = (4731 - 120); i < (4750 - 120); i++)
			screen[i] = shadeHand;
		for (int i = (4731 - 240); i < (4750 - 240); i++)
			screen[i] = shadeHand;
		for (int i = (4731 - 360); i < (4750 - 360); i++)
			screen[i] = shadeHand;
		for (int i = (4731 - 480); i < (4750 - 480); i++)
			screen[i] = shadeHand;
		for (int i = (4731 - 600); i < (4750 - 600); i++)
			screen[i] = shadeHand;
		for (int i = (4731 - 720); i < (4750 - 720); i++)
			screen[i] = shadeHand;
		for (int i = (4731 - 840); i < (4750 - 840); i++)
			screen[i] = shadeHand;
		for (int i = (4731 - 960); i < (4750 - 960); i++)
			screen[i] = shadeHand;
		for (int i = (4731 - 1080); i < (4750 - 1080); i++)
			screen[i] = shadeHand;



		screen[4730] = '+'; //bottom left hand
		screen[4750] = '+'; //bottom right hand
		screen[3530] = '+'; //top left hand
		screen[3550] = '+'; //top right hand
		screen[4373] = '+'; //bottom left gun
		screen[4387] = '+'; //bottom right gun
		screen[1973] = '+'; //top left gun
		screen[1987] = '+'; //top right gun

		for (int i = (4731); i < (4750); i++)
			screen[i] = '-';
		for (int i = (3531); i < (3550); i++)
			screen[i] = '-';
		for (int i = (4374); i < (4387); i++)
			screen[i] = '-';
		for (int i = (1974); i < (1987); i++)
			screen[i] = '-';

		for (int i = (4730 - 120); i > 3530; i = i - 120)
			screen[i] = '|';
		for (int i = (4750 - 120); i > 3550; i = i - 120)
			screen[i] = '|';
		for (int i = (4373 - 120); i > 1973; i = i - 120)
			screen[i] = '|';
		for (int i = (4387 - 120); i > 1987; i = i - 120)
			screen[i] = '|';

		//Shade Gun
		for (int i = (4374 - 120); i < (4387 - 120); i++)
			screen[i] = shadeGun;
		for (int i = (4374 - 240); i < (4387 - 240); i++)
			screen[i] = shadeGun;
		for (int i = (4374 - 360); i < (4387 - 360); i++)
			screen[i] = shadeGun;
		for (int i = (4374 - 480); i < (4387 - 480); i++)
			screen[i] = shadeGun;
		for (int i = (4374 - 600); i < (4387 - 600); i++)
			screen[i] = shadeGun;
		for (int i = (4374 - 720); i < (4387 - 720); i++)
			screen[i] = shadeGun;
		for (int i = (4374 - 840); i < (4387 - 840); i++)
			screen[i] = shadeGun;
		for (int i = (4374 - 960); i < (4387 - 960); i++)
			screen[i] = shadeGun;
		for (int i = (4374 - 1080); i < (4387 - 1080); i++)
			screen[i] = shadeGun;
		for (int i = (4374 - 1200); i < (4387 - 1200); i++)
			screen[i] = shadeGun;
		for (int i = (4374 - 1320); i < (4387 - 1320); i++)
			screen[i] = shadeGun;
		for (int i = (4374 - 1440); i < (4387 - 1440); i++)
			screen[i] = shadeGun;
		for (int i = (4374 - 1560); i < (4387 - 1560); i++)
			screen[i] = shadeGun;
		for (int i = (4374 - 1680); i < (4387 - 1680); i++)
			screen[i] = shadeGun;
		for (int i = (4374 - 1800); i < (4387 - 1800); i++)
			screen[i] = shadeGun;
		for (int i = (4374 - 1920); i < (4387 - 1920); i++)
			screen[i] = shadeGun;
		for (int i = (4374 - 2040); i < (4387 - 2040); i++)
			screen[i] = shadeGun;
		for (int i = (4374 - 2160); i < (4387 - 2160); i++)
			screen[i] = shadeGun;
		for (int i = (4374 - 2280); i < (4387 - 2280); i++)
			screen[i] = shadeGun;

		screen[4740 - 2520] = '-';
		screen[4739 - 2520] = '+';
		screen[4741 - 2520] = '+';

	}

}

void MoveEnemy()
{
	int moveX = (rand() % 2);
	int moveY = (rand() % 2);
	int flip = (rand() % 2);

	if (flip == 0)
	{
		EnemyX = EnemyX + moveX;
		EnemyY = EnemyY + moveY;
	}
	else
	{
		EnemyX = EnemyX - moveX;
		EnemyY = EnemyY - moveY;
	}

	if (EnemyX < 0)
		EnemyX = 0;
	else if (EnemyX > 16)
		EnemyX = 16;

	if (EnemyY < 0)
		EnemyY = 0;
	else if (EnemyY > 16)
		EnemyY = 16;

	return;
}