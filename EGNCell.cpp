#include "EGNCell.h"
#include <stdexcept>

EGNCell::EGNCell(const std::string& data) : Cell(CellType::EGNCell)
{
	setData(data);
}

void EGNCell::setData(const std::string& data)
{
	if (!isValidEGN(data))
		throw std::invalid_argument("The passed EGN is invalid.");

	this->data = data;
}

Cell* EGNCell::clone() const
{
	return new EGNCell(*this);
}

std::string EGNCell::toString() const
{
	return data;
}

bool EGNCell::operator<(const Cell& other) const
{
	if (other.getType() != CellType::EGNCell)
		throw std::invalid_argument("The passed argument has different cell type than EGN");

    return this->data < other.toString();
}

//Bulgarian EGN contains 10 digits and the first 6 are in format YYMMDD where: 
//people born 1900 - 1999  - months from 1 - 12
//people born 1800 - 1899  - months from 21 to 32 (01-12 + 20)
//people born after 2000   - months from 41 to 52 (01-12 + 40)

bool EGNCell::isValidEGN(const std::string& data)
{
    if (data.length() != 10)
        return false;

    for (int i = 0; i < 10; i++)
    {
        if (!isdigit(data[i]))
            return false;
    }

    int yearPart = std::stoi(data.substr(0, 2));  //the first two digits are the last 2 digits of the year
    int month = std::stoi(data.substr(2, 2)); // the 2 digits which represent month
    int day = std::stoi(data.substr(4, 2)); // the 2 digits which represent day of birth
    int fullYear = 0;

    if (month >= 1 && month <= 12)
    {
        fullYear = 1900 + yearPart;
    }
    else if (month >= 21 && month <= 32)
    {
        fullYear = 1800 + yearPart;
        month -= 20;
    }
    else if (month >= 41 && month <= 52)
    {
        fullYear = 2000 + yearPart;
        month -= 40;
    }
    else
    {
        return false;
    }

    return EGNCell::isValidDate(day, month, fullYear);
}

bool EGNCell::isValidDate(int day, int month, int year) 
{
	if (month < 1 || month > 12 || year < 1800 ||  year > 2025 || day < 1)
		return false;

	const int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	//number of days in each month

	int daysInCurrentMonth = daysInMonth[month - 1];


	//Check for leap year
	if (month == 2)
	{
		//** The Following IF statement is from programiz.com **
		// https://www.programiz.com/cpp-programming/examples/leap-year
		//it checks if a specific year is leap year

		if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
			daysInCurrentMonth = 29;
	}

	return day <= daysInCurrentMonth;
}