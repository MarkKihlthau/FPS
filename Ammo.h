#pragma once
#include "Weapon.h"
class Ammo : public Weapon
{
public:
	Ammo() {}
	~Ammo() {}

	void pickup();
	void place();
	void reloadable();
};