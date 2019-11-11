#pragma once
#include "Item.h"
class Weapon : public Item
{
public:
	Weapon() {}
	~Weapon() {}

	virtual void pickup();
	virtual void place();
	virtual void reloadable();
};