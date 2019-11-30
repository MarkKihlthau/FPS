#include "Weapon.h"

int Weapon::pickup()
{
	return 2;
}

void Weapon::reloadable()
{
	//Intentionally blank
}

void Weapon::set_weapon(string type)
{
	if (type == "Pistol")
	{
		name = "Pistol";
		damage = 5;
		capacity = 16;
		reload = 32;
	}
	else if (type == "RailGun")
	{
		name = "RailGun";
		damage = 12;
		capacity = 8;
		reload = 16;
	}
	else
	{
		//Error state.
	}
}

string Weapon::get_name()
{
	return name;
}
int Weapon::get_damage()
{
	return damage;
}
int Weapon::get_capacity()
{
	return capacity;
}
int Weapon::get_reload()
{
	return reload;
}