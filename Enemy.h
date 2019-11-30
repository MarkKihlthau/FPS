#pragma once
#include <stdlib.h>
#include <time.h> 
using namespace std;

class Enemy
{
public:
	void hit(int bullet)
	{
		health = health - bullet;
	}

	int strike()
	{
		srand(time(NULL));
		int damage = base_damage + (rand() % 10);
		return damage;
	}

	bool alive()
	{
		if (health >= 0)
			return true;
		else
			return false;
	}

	int get_health()
	{
		return health;
	}

private:
	int health = 100;
	int base_damage = 5;
};