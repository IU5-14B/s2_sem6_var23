#ifndef MUSEUM_H
#define MUSEUM_H

#include "Catalog.h"
#include "Hall.h"
#include "MyContainer.h"
#include "MuseumItem.h"
#include <memory>
#include <string>

class Museum {
private:
    std::string name;
    MyContainer<Hall> halls;
    MyContainer<std::shared_ptr<MuseumItem>> items;
    Catalog catalog;

public:
    Museum(const std::string& name);

    const std::string& getName() const;
    std::size_t getHallCount() const;
    std::size_t getItemCount() const;
    const MyContainer<Hall>& getHalls() const;
    const MyContainer<std::shared_ptr<MuseumItem>>& getItems() const;

    void addHall(int number, const std::string& hallName);
    void addItem(const std::shared_ptr<MuseumItem>& item);
    void showInfo() const;
    void showHalls() const;
    void showCollection() const;
    void showVisitPlan() const;
    const Hall* findHall(const Hall& hall) const;
    const MuseumItem* findItem(const std::string& title) const;
    MyContainer<std::shared_ptr<MuseumItem>> buildShortRouteForHall(const Hall& hall,
                                                                    int maxMinutes) const;
};

#endif
