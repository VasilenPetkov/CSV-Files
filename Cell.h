#pragma once
#include <string>


//valid cell types are Text, Number, EGN and Faculty Number
enum class CellType
{
    IntegerCell,
    FloatNumberCell,
    TextCell,
    CurrencyCell,
    EGNCell,
    FNCell
};


//Abstract Base class for cell in table
//it represent one element from a specific type
class Cell
{

public:
    Cell(CellType cellType) : cellType(cellType) {}

    virtual ~Cell() = default;

    virtual Cell* clone() const = 0; //creates copy of the cell, each derived class implements it
    virtual std::string toString() const = 0; //return the value of the cell in text forma
    virtual bool operator<(const Cell& other) const = 0; //the only virtual operator is <, because the rest are implemented using <

    bool operator>(const Cell& other) const;
    bool operator<=(const Cell& other) const;
    bool operator>=(const Cell& other) const;
    bool operator==(const Cell& other) const;
    bool operator!=(const Cell& other) const;

    CellType getType() const;

private:
    CellType cellType;
};

