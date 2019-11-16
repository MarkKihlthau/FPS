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

private:
	Item* pItem;
};