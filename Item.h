#pragma once
#include <string>
using namespace std;
class Item
{
public:
	Item() {}
	virtual ~Item() {}
	virtual void pickup() = 0;
	void virtual place(int x, int y) = 0;
	static Item* Create(string type);

	int item_at[16][16];

private:
	Item* pItem;
};