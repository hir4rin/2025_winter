#pragma once
#include <vector>
#include <memory>

class Item;
class Bg;
class ItemFactory
{
public:
	ItemFactory(std::vector<std::shared_ptr<Item>>& pItems);
	virtual ~ItemFactory();

	void Update(Bg* bg);
private:
	std::vector<std::shared_ptr<Item>>& m_pItems;//参照渡し
	int m_frame;
};

