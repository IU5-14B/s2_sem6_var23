#ifndef CATALOG_H
#define CATALOG_H

#include "MyContainer.h"
#include <memory>
#include <string>

class MuseumItem;

class Catalog {
public:
    const MuseumItem* findByTitle(const MyContainer<std::shared_ptr<MuseumItem>>& items,
                                  const std::string& neededTitle) const;
};

#endif
