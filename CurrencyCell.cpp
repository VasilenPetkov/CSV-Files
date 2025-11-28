#include "CurrencyCell.h"
#include <stdexcept>

CurrencyCell::CurrencyCell(const std::string& data) : Cell(CellType::CurrencyCell)
{
	setData(data);
}

void CurrencyCell::setData(const std::string& data)
{
	if (!isValidCurrency(data))
		throw std::invalid_argument("The passed currency is invalid.");

	this->data = data;
}

Cell* CurrencyCell::clone() const
{
	return new CurrencyCell(*this);
}

std::string CurrencyCell::toString() const
{
	return data;
}

bool CurrencyCell::operator<(const Cell& other) const
{
	if (other.getType() != CellType::CurrencyCell)
		throw std::invalid_argument("The passed argument has different cell type than currency");

	std::string thisCurrency = getCurrencyChar(this->data);
	std::string otherCurrency = getCurrencyChar(other.toString());

	if (thisCurrency == otherCurrency)
	{
		double thisValue = getValue(this->data);
		double otherValue = getValue(other.toString());

		return thisValue < otherValue;
	}

	return thisCurrency < otherCurrency;
}


bool CurrencyCell::isValidCurrency(const std::string& str) 
{
	if (str.length() < 3)
		return false;

	std::string valuePart = str.substr(0, str.size() - 3);
	std::string currencyPart = getCurrencyChar(str);


	try
	{
		std::stod(valuePart);
	}
	catch (...)
	{
		return false;
	}

	return isUppercaseLetters(currencyPart);

}

bool CurrencyCell::isUppercaseLetters(const std::string& str) 
{
	if (str.length() != 3)
		return false;

	for (int i = 0; i < 3; i++)
	{
		if (!(str[i] >= 'A' && str[i] <= 'Z'))
			return false;
	}

	return true;

}

std::string CurrencyCell::getCurrencyChar(const std::string& str) 
{
	return str.substr(str.length() - 3); // To extract the 3 chars (for example BGN)
}

double CurrencyCell::getValue(const std::string& str) 
{
	return std::stod(str.substr(0, str.length() - 3)); // To exctract the value part of the currency 
}



