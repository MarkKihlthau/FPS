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
		return base_damage;
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
	int base_damage = 1;
};