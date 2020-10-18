#include "pch.h"

enum class ItemID {
    first = 0,
    second,
    ken,
    yas
};

struct Item {
    std::string text;
    bool isActive;
};

int main() {
    std::map<ItemID, Item> items;

    items.insert(std::make_pair(ItemID::first, Item{"Im number 1!", true}));
    items.insert(std::make_pair(ItemID::second, Item{"Twooooo", false}));
    items[ItemID::ken] = {"Ken Here!", true};

    for (auto& item : items) {
        std::cout << item.second.text << " | " << item.second.isActive << std::endl;
    }

    for (auto& [itemID, item] : items) {
        std::cout << item.text << " | " << (item.isActive ? "Active" : "Disabled") << std::endl;
    }

    items[ItemID::yas] = {"Yeppers", false};
    std::cout << items[ItemID::yas].text << std::endl;

    items[ItemID::yas].text = "Yepperoonie";
    std::cout << items.find(ItemID::yas)->second.text << std::endl;

    auto& [itemID, item] = *items.find(ItemID::yas);
    std::cout << item.text << std::endl;
}