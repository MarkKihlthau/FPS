#include "Item.h"
#include "HealthPack.h"

Item* Item::Create(string type)
{
	if (type == "HealthPack")
		return new HealthPack();
}