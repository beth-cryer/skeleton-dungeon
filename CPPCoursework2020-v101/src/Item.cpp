#include "header.h"
#include "Item.h"

Item::Item(int iconId, std::string name, std::string desc)
	: itemId(0), iconId(iconId), name(name), desc(desc) {

}

Item::~Item() {}

void Item::virtItemUse() {
}

void Item::virtItemExamine() {
}

void Item::virtItemDrop() {
}