#ifndef UNITS_H_INCLUDED
#define UNITS_H_INCLUDED

#include <string>
#include "field.h"

/* Type of Units */
enum UnitType { FOOTMAN, KNIGHT, ARCHER, MAGE, UNDEFINED };

/* Class for units */
class Unit {
public:
    Unit(UnitType u, bool sd) :
        type(u), side(sd) {
    }
    Unit(){
        type = UNDEFINED;
        side = false;
    }
    friend Field* loadmap(std::istream& is);
    UnitType getType() const {
        return type;
    }
    bool getSide() const {
        return side;
    }
    void setType(UnitType t){
        type = t;
    }
    void setSide(bool s){
        side = s;
    }
private:
    UnitType type;
    bool side;
};

#endif // UNITS_H_INCLUDED


