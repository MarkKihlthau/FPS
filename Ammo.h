#pragma once
#include "Weapon.h"
class Ammo : public Weapon
{
public:
	Ammo() {}
	~Ammo() {}

	string name = "Null";
	int amount = 0;

	int pickup();
	void reloadable();
	void set_ammo(string type);
};

int Ammo::pickup()
{
	return 4;
}

void Ammo::reloadable()
{
	if (Ammo::name == "9mm" && Weapon::name == "Pistol")
		capacity = capacity + amount;
	else if (Ammo::name == "Battery" && Weapon::name == "RailGun")
		capacity = capacity + amount;
}

void Ammo::set_ammo(string type)
{
	if (type == "9mm")
	{
		name = "9mm";
		amount = 32;
	}
	else if (type == "Battery")
	{
		name = "Battery";
		amount = 16;
	}
	else
	{
		//Error State.
	}
}