#include "Guide.h"
#include "MuseumItem.h"
#include <iostream>

Guide::Guide(const std::string& name) : name(name) {}

const std::string& Guide::getName() const {
    return name;
}

std::size_t Guide::getRouteCount() const {
    return route.size();
}

void Guide::addToRoute(MuseumItem* item) {
    route.push_back(item);
}

void Guide::showRoute() const {
    std::cout << "Маршрут гида " << name << ":\n";

    if (route.empty()) {
        std::cout << "  Маршрут пока не составлен.\n";
        return;
    }

    int stopNumber = 1;
    for (auto it = route.begin(); it != route.end(); ++it) {
        const MuseumItem* item = *it;
        std::cout << "Остановка " << stopNumber << ":\n";
        item->printInfo();
        std::cout << "  Рекомендуемое время осмотра: "
                  << item->getRecommendedVisitMinutes() << " мин.\n";
        ++stopNumber;
    }
}
