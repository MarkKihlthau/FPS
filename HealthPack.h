#pragma once
#include "Item.h"
class HealthPack : public Item
{
public:
	HealthPack() {}
	~HealthPack() {}

	int pickup();
};

int HealthPack::pickup()
{
	return 1;
}