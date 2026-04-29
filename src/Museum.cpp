#include "Museum.h"
#include "Exhibit.h"
#include <algorithm>
#include <iostream>

Museum::Museum(const std::string& name) : name(name) {}

const std::string& Museum::getName() const {
    return name;
}

std::size_t Museum::getHallCount() const {
    return halls.size();
}

std::size_t Museum::getItemCount() const {
    return items.size();
}

const MyContainer<Hall>& Museum::getHalls() const {
    return halls;
}

const MyContainer<std::shared_ptr<MuseumItem>>& Museum::getItems() const {
    return items;
}

void Museum::addHall(int number, const std::string& hallName) {
    halls.push_back(Hall(number, hallName));
}

void Museum::addItem(const std::shared_ptr<MuseumItem>& item) {
    items.push_back(item);
}

void Museum::showInfo() const {
    std::cout << "Музей: " << name << '\n'
              << "Количество залов: " << halls.size() << '\n'
              << "Количество экспонатов: " << items.size() << '\n';
}

void Museum::showHalls() const {
    std::cout << "Залы музея \"" << name << "\":\n";

    if (halls.empty()) {
        std::cout << "Залы пока не добавлены.\n";
        return;
    }

    for (const Hall& hall : halls) {
        hall.printInfo();
    }
}

void Museum::showCollection() const {
    std::cout << "Экспонаты музея \"" << name << "\":\n";

    if (items.empty()) {
        std::cout << "Коллекция пока пуста.\n";
        return;
    }

    for (const auto& item : items) {
        item->printInfo();
    }
}

void Museum::showVisitPlan() const {
    std::cout << "План осмотра музея \"" << name << "\":\n";

    if (items.empty()) {
        std::cout << "План осмотра пока невозможно составить.\n";
        return;
    }

    for (const auto& item : items) {
        std::cout << "- " << item->getTitle()
                  << ": " << item->getRecommendedVisitMinutes() << " мин.\n";
    }
}

const Hall* Museum::findHall(const Hall& hall) const {
    auto hallIt = std::find(halls.begin(), halls.end(), hall);
    if (hallIt == halls.end()) {
        return nullptr;
    }

    return &(*hallIt);
}

const MuseumItem* Museum::findItem(const std::string& title) const {
    return catalog.findByTitle(items, title);
}

MyContainer<std::shared_ptr<MuseumItem>> Museum::buildShortRouteForHall(const Hall& hall,
                                                                        int maxMinutes) const {
    MyContainer<std::shared_ptr<MuseumItem>> route;
    const Hall* foundHall = findHall(hall);

    if (foundHall == nullptr) {
        return route;
    }

    for (const auto& item : items) {
        const Exhibit* exhibit = dynamic_cast<const Exhibit*>(item.get());
        if (exhibit == nullptr) {
            continue;
        }

        if (exhibit->getHallNumber() == foundHall->getNumber() &&
            item->getRecommendedVisitMinutes() <= maxMinutes) {
            route.push_back(item);
        }
    }

    return route;
}
