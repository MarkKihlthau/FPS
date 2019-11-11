#pragma once
#include "Item.h"
class Special : public Item
{
public:
	Special() {}
	~Special() {}

	void pickup();
	void place();
};