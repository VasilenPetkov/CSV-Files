#include "FloatNumberCell.h"
#include <stdexcept>

FloatNumberCell::FloatNumberCell(double data) : Cell(CellType::FloatNumberCell), data(data)
{
}

void FloatNumberCell::setData(double data)
{
	this->data = data;
}

Cell* FloatNumberCell::clone() const 
{
	return new FloatNumberCell(*this);
}

std::string FloatNumberCell::toString() const
{
	return std::to_string(data);
}

bool FloatNumberCell::operator<(const Cell& other) const
{
	if (other.getType() != CellType::FloatNumberCell)
		throw std::invalid_argument("The passed argument has different cell type than double");

	return this->data < std::stod(other.toString());
}