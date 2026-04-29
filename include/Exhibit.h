#ifndef EXHIBIT_H
#define EXHIBIT_H

#include "MuseumItem.h"
#include <string>

class Exhibit : public MuseumItem {
protected:
    std::string author;
    int hallNumber;

public:
    Exhibit(const std::string& title, int year, const std::string& author, int hallNumber);

    const std::string& getAuthor() const;
    int getHallNumber() const;
};

class Painting : public Exhibit {
private:
    std::string technique;

public:
    Painting(const std::string& title, int year, const std::string& author, int hallNumber,
             const std::string& technique);

    void printInfo() const override;
    int getRecommendedVisitMinutes() const override;
    const std::string& getTechnique() const;
};

class Sculpture : public Exhibit {
private:
    std::string material;

public:
    Sculpture(const std::string& title, int year, const std::string& author, int hallNumber,
              const std::string& material);

    void printInfo() const override;
    int getRecommendedVisitMinutes() const override;
    const std::string& getMaterial() const;
};

class Artifact : public Exhibit {
private:
    std::string epoch;

public:
    Artifact(const std::string& title, int year, const std::string& author, int hallNumber,
             const std::string& epoch);

    void printInfo() const override;
    int getRecommendedVisitMinutes() const override;
    const std::string& getEpoch() const;
};

#endif
