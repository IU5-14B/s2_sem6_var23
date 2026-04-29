#include "Visitor.h"
#include "Guide.h"
#include <iostream>

Visitor::Visitor(const std::string& name) : name(name), guide(nullptr) {}

void Visitor::setGuide(Guide* newGuide) {
    guide = newGuide;
}

void Visitor::askGuideName() const {
    if (guide != nullptr) {
        std::cout << "Посетитель " << name << " общается с гидом " << guide->getName() << '\n';
    } else {
        std::cout << "У посетителя " << name << " пока нет гида\n";
    }
}
