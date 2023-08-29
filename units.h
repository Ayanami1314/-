#ifndef UNITS_H_INCLUDED
#define UNITS_H_INCLUDED

#include <string>
/* Type of Units */
enum UnitType
{
    FOOTMAN,
    KNIGHT,
    ARCHER,
    MAGE,
    UNDEFINED
};

/* Class for units */
class Unit
{
public:
    Unit(UnitType u, bool sd) : type(u), side(sd)
    {
        switch (u)
        {
        case FOOTMAN:
            this->MovePoints = 3;
            break;
        case KNIGHT:
            this->MovePoints = 4;
            break;
        case ARCHER:
            this->MovePoints = 2;
            break;
        case MAGE:
            this->MovePoints = 2;
            break;
        default:
            this->MovePoints = 0;
            break;
        }
    }
    Unit(UnitType u, bool sd, int mp) : type(u), side(sd), MovePoints(mp)
    {
        ;
    }
    Unit(const Unit &u)
    {
        type = u.type;
        side = u.side;
        MovePoints = u.MovePoints;
    }
    Unit()
    {
        type = UNDEFINED;
        side = false;
        MovePoints = 0;
    }
    // friend Field *loadmap(std::istream &is);
    UnitType getType() const
    {
        return type;
    }
    bool getSide() const
    {
        return side;
    }
    void setType(UnitType t)
    {
        type = t;
    }
    void setSide(bool s)
    {
        side = s;
    }
    void setMovePoints(int mp)
    {
        MovePoints = mp;
    }
    void setMovePointsByType(UnitType ut)
    {
        switch (ut)
        {
        case FOOTMAN:
            this->MovePoints = 3;
            break;
        case KNIGHT:
            this->MovePoints = 4;
            break;
        case ARCHER:
            this->MovePoints = 2;
            break;
        case MAGE:
            this->MovePoints = 2;
            break;
        default:
            this->MovePoints = 0;
            break;
        }
    }
    int getMovePoints() const
    {
        return MovePoints;
    }

private:
    UnitType type;
    bool side;
    int MovePoints;
};

#endif // UNITS_H_INCLUDED
