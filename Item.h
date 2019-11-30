#pragma once
#include <string>
using namespace std;
class Item
{
public:
	Item() {}
	virtual ~Item() {}
	virtual int pickup() = 0;
	static Item* Create(string type);

	//Weapon
	virtual void set_weapon(string type);
	virtual string get_name();
	virtual int get_damage();
	virtual int get_capacity();
	virtual int get_reload();

	//Ammo
	virtual int get_amount();

private:
	Item* pItem;
};