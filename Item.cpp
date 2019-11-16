#include "Item.h"
#include "HealthPack.h"
#include "Weapon.h"
#include "Special.h"
#include "Ammo.h"

Item* Item::Create(string type)
{
	if (type == "HealthPack")
		return new HealthPack();
	if (type == "Weapon")
		return new Weapon();
	if (type == "Special")
		return new Special();
	if (type == "Ammo")
		return new Ammo();
}