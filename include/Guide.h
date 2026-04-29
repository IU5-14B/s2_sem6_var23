#ifndef GUIDE_H
#define GUIDE_H

#include "MyContainer.h"
#include <string>

class MuseumItem;

class Guide {
private:
    std::string name;
    MyContainer<MuseumItem*> route;

public:
    Guide(const std::string& name);

    const std::string& getName() const;
    std::size_t getRouteCount() const;
    void addToRoute(MuseumItem* item);
    void showRoute() const;
};

#endif
