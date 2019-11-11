#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <Windows.h>
#include <chrono>
#include "Item.h"
using namespace std;

int nScreenWidth = 120;
int nScreenHeight = 40;

float fPlayerX = 1.0f; //player X position
float fPlayerY = 14.5f; //player Y position
float fPlayerA = 1.5f; //player look Angle

int nMapHeight = 16;
int nMapWidth = 16;

float fFOV = 3.14159 / 4; //field of view
float fDepth = 16.0f;
float fSpeed = 5.0f;

void Controls(float, wstring, wchar_t*);
void Environment(wchar_t*, wstring);
void MiniMap(wchar_t*, wstring);
void AimingRect(wchar_t*);

bool shot = false;

int main()
{
	//Create Screen Buffer
	wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	wstring map; //16x16 map
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
	map += L"#..............#";
	map += L"#..............#";
	map += L"#############..#";
	map += L"#.........H....#";
	map += L"################"; //15

	auto TimeP1 = chrono::system_clock::now();
	auto TimeP2 = chrono::system_clock::now();

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

		//Aiming Recticle
		AimingRect(screen);

		//Display Mini-Map
		MiniMap(screen, map);

		//Display Stats
		//swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A%3.2f FPS=%3.2f", fPlayerX, fPlayerY, fPlayerA, 1.0f / fElapsedTime);
		
		//Display Frame 
		screen[nScreenWidth * nScreenHeight - 1] = '/0';
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
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
		shot = true;
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
					H1->place(10, 14);
					if ((10.0 <= fPlayerX && fPlayerX <= 10.99) && (14.0 <= fPlayerY && fPlayerY <= 14.99))
					{
						swprintf_s(screen, 40, L"HealthPack picked up!");
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

void MiniMap(wchar_t* screen, wstring map)
{
	for (int nx = 0; nx < nMapWidth; nx++)
		for (int ny = 0; ny < nMapWidth; ny++)
		{
			screen[(ny + 1)*nScreenWidth + nx] = map[ny * nMapWidth + (nMapWidth - nx - 1)];
		}
	if (shot == false)
		screen[((int)fPlayerY + 1) * nScreenWidth + (int)(nMapWidth - fPlayerX)] = 'P';
	else
		screen[((int)fPlayerY + 1) * nScreenWidth + (int)(nMapWidth - fPlayerX)] = 'X';
}

void AimingRect(wchar_t* screen)
{
	for (int n1 = 2340; n1 > 1860; n1 = n1 - 120) //top
		screen[n1] = '!';
	for (int n2 = 2580; n2 < 3060; n2 = n2 + 120) //bottom
		screen[n2] = '!';
	for (int n3 = 2462; n3 < 2469; n3++) //left
		screen[n3] = '!';
	for (int n4 = 2458; n4 > 2451; n4--) //right
		screen[n4] = '!';

	//Bullet
	if (shot == true)
	{
		screen[2460] = '.';
		shot = false;
	}
}