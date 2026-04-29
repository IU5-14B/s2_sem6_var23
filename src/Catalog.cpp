#include "Catalog.h"
#include "MuseumItem.h"

const MuseumItem* Catalog::findByTitle(const MyContainer<std::shared_ptr<MuseumItem>>& items,
                                       const std::string& neededTitle) const {
    for (const auto& item : items) {
        if (item->getTitle() == neededTitle) {
            return item.get();
        }
    }

    return nullptr;
}
