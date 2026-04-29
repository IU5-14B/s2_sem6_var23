#ifndef VISITOR_H
#define VISITOR_H

#include <string>

class Guide;

class Visitor {
private:
    std::string name;
    Guide* guide;

public:
    Visitor(const std::string& name);

    void setGuide(Guide* newGuide);
    void askGuideName() const;
};

#endif
