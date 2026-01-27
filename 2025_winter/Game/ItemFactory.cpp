#include "ItemFactory.h"
#include "Item.h"



ItemFactory::ItemFactory(std::vector<std::shared_ptr<Item>>& pItems):
	m_pItems(pItems),
	m_frame(0)
{
	
}

ItemFactory::~ItemFactory()
{
	
}

void ItemFactory::Update(Bg* bg)
{
	

	if (m_frame % 300 == 0)
	{
		auto item = std::make_shared<Item>(std::shared_ptr<EnemyWizard>());//新しくアイテムを生成
		auto item2 = std::make_shared<Item>(std::shared_ptr<EnemyRider>());//新しくアイテムを生成
		auto item3 = std::make_shared<Item>(std::shared_ptr<EnemyArcher>());//新しくアイテムを生成

		item->SetBgPointer(bg);
		item2->SetBgPointer(bg);
		item3->SetBgPointer(bg);

		item->ChangePos() = Vec2{ 792,736 };
		item2->ChangePos() = Vec2{ 1254,736 };
		item3->ChangePos() = Vec2{ 1699,736 };

		m_pItems.push_back(item);
		m_pItems.push_back(item2);
		m_pItems.push_back(item3);
	}
	
	m_frame++;
}
