#pragma once
#include "Item.h"
class Special : public Item
{
public:
	Special() {}
	~Special() {}

	int pickup();
};

int Special::pickup()
{
	return 3;
}