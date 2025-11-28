#include "Cell.h"

bool Cell::operator>(const Cell& other) const
{
    return other < *this;
}

bool Cell::operator<=(const Cell& other) const 
{
    // !(*this > other)
    return (*this < other) || (*this == other);
}

bool Cell::operator>=(const Cell& other) const 
{
    return !(*this < other);
}

bool Cell::operator==(const Cell& other) const
{
    return !(*this < other) && !(other < *this);
}

bool Cell::operator!=(const Cell& other) const 
{
    return !(*this == other);
}

CellType Cell::getType() const
{ 
    return cellType; 
}