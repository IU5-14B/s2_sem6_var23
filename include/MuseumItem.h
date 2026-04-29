#ifndef MUSEUM_ITEM_H
#define MUSEUM_ITEM_H

#include <string>

class MuseumItem {
protected:
    std::string title;
    int year;

public:
    MuseumItem(const std::string& title, int year);
    virtual ~MuseumItem() = default;

    const std::string& getTitle() const;
    int getYear() const;
    virtual void printInfo() const = 0;
    virtual int getRecommendedVisitMinutes() const = 0;
};

#endif
