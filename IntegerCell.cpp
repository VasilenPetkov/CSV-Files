#include "IntegerCell.h"
#include <stdexcept>

IntegerCell::IntegerCell(int data) : Cell(CellType::IntegerCell), data(data)
{

}

void IntegerCell::setData(int newData)
{
	this->data = newData;
}

Cell* IntegerCell::clone() const
{
	return new IntegerCell(*this);
}

std::string IntegerCell::toString() const
{
	return std::to_string(data);
}

bool IntegerCell::operator<(const Cell& other) const
{
	if (other.getType() != CellType::IntegerCell)
		throw std::invalid_argument("The passed argument has different cell type than integer.");

	return this->data < std::stoi(other.toString());
}

