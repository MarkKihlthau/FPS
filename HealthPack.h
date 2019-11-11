#pragma once
#include "Item.h"
class HealthPack : public Item
{
public:
	HealthPack() {}
	~HealthPack() {}

	void pickup();
	void place(int x, int y);
};

void HealthPack::pickup()
{

}

void HealthPack::place(int x, int y)
{
	item_at[x][y] = 1;
}