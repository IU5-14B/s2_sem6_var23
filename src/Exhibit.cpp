#include "Exhibit.h"
#include <iostream>

Exhibit::Exhibit(const std::string& title, int year, const std::string& author, int hallNumber)
    : MuseumItem(title, year), author(author), hallNumber(hallNumber) {}

const std::string& Exhibit::getAuthor() const {
    return author;
}

int Exhibit::getHallNumber() const {
    return hallNumber;
}

Painting::Painting(const std::string& title, int year, const std::string& author, int hallNumber,
                   const std::string& technique)
    : Exhibit(title, year, author, hallNumber), technique(technique) {}

void Painting::printInfo() const {
    std::cout << "Картина: " << title
              << ", художник: " << author
              << ", год: " << year
              << ", техника: " << technique
              << ", зал: " << hallNumber << '\n';
}

int Painting::getRecommendedVisitMinutes() const {
    return 15;
}

const std::string& Painting::getTechnique() const {
    return technique;
}

Sculpture::Sculpture(const std::string& title, int year, const std::string& author, int hallNumber,
                     const std::string& material)
    : Exhibit(title, year, author, hallNumber), material(material) {}

void Sculpture::printInfo() const {
    std::cout << "Скульптура: " << title
              << ", автор: " << author
              << ", год: " << year
              << ", материал: " << material
              << ", зал: " << hallNumber << '\n';
}

int Sculpture::getRecommendedVisitMinutes() const {
    return 12;
}

const std::string& Sculpture::getMaterial() const {
    return material;
}

Artifact::Artifact(const std::string& title, int year, const std::string& author, int hallNumber,
                   const std::string& epoch)
    : Exhibit(title, year, author, hallNumber), epoch(epoch) {}

void Artifact::printInfo() const {
    std::cout << "Артефакт: " << title
              << ", происхождение: " << author
              << ", датировка: " << year
              << ", эпоха: " << epoch
              << ", зал: " << hallNumber << '\n';
}

int Artifact::getRecommendedVisitMinutes() const {
    return 10;
}

const std::string& Artifact::getEpoch() const {
    return epoch;
}
