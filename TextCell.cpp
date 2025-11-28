#include "TextCell.h"
#include <stdexcept>

std::string TextCell::removeQuotationMarks() const
{
	if (data.size() < 2)
		return data;

	char firstChar = data.front();
	char lastChar = data.back();

	if ((firstChar == '"' && lastChar == '"') || (firstChar == '\'' && lastChar == '\''))
	{
		return data.substr(1, data.size() - 2);
	}

	return data;
}

TextCell::TextCell(const std::string& data) : Cell(CellType::TextCell), data(data)
{

}

void TextCell::setData(const std::string& data)
{
	this->data = data;
}

Cell* TextCell::clone() const
{
	return new TextCell(*this);
}

std::string TextCell::toString() const
{
	return data;
}

bool TextCell::operator<(const Cell& other) const
{
	if (other.getType() != CellType::TextCell)
		throw std::invalid_argument("The passed argument has different cell type than text");

	const TextCell& otherCell = (TextCell&)(other);

	return this->removeQuotationMarks() < otherCell.removeQuotationMarks();
}
