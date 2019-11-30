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

void Item::set_weapon(string type)
{
	return;
}
string Item::get_name()
{
	return "";
}
int Item::get_damage()
{
	return 0;
}
int Item::get_capacity()
{
	return 0;
}
int Item::get_reload()
{
	return 0;
}

int Item::get_amount()
{
	return 0;
}