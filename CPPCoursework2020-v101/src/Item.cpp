#include "header.h"
#include "Item.h"
#include "GameEngine.h"

Item::Item(GameEngine* pEngine, int iconId, std::string name, std::string desc)
	: pEngine(pEngine), itemId(0), iconId(iconId), name(name), desc(desc) {

}

Item::~Item() {}

void Item::virtItemUse() {
}

void Item::virtItemExamine() {
}

void Item::virtItemDrop() {
}