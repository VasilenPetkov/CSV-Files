#include "FacultyNumberCell.h"
#include <stdexcept>

FacultyNumberCell::FacultyNumberCell(const std::string& data) : Cell(CellType::FNCell)
{
	setData(data);
}

void FacultyNumberCell::setData(const std::string& data)
{
	if (!isValidFN(data))
		throw std::invalid_argument("The passed Faculty number is invalid.");

	this->data = data;
}

Cell* FacultyNumberCell::clone() const
{
	return new FacultyNumberCell(*this);
}

std::string FacultyNumberCell::toString() const
{
	return data;
}

bool FacultyNumberCell::operator<(const Cell& other) const
{
	if (other.getType() != CellType::FNCell)
		throw std::invalid_argument("The passed argument has different cell type than Faculty number");

	char thisMajor = (data.size() == 10 ? data[4] : data[0]);
	char otherMajor = (other.toString().size() == 10 ? other.toString()[4] : other.toString()[0]);

	//First, faculty numbers are grouped by Major

	if (thisMajor != otherMajor)
		return thisMajor < otherMajor;

	//In the same major, the old faculty numbers are before the new ones

	bool isThisOldNumber = data.size() == 5;
	bool isOtherOldNumber = other.toString().size() == 5;

	if (isThisOldNumber != isOtherOldNumber)
		return isThisOldNumber; 

	//in case - old < new we return true
	//in the other case - new < old we return false

	return data < other.toString();
}


bool FacultyNumberCell::isValidFN(const std::string& data)
{

	//check for old faculty number (for example 45123)

	if (data.size() == 5 && data[0] >= '2' && data[0] <= '8')
	{
		for (int i = 0; i < 5; i++)
		{
			if (!isdigit(data[i]))
				return false;
		}

		return true;
	}


	//check for new faculty number (for example 0MI0400123)

	if (data.size() == 10 && data[1] == 'M' && data[2] == 'I')
	{
		if (!isdigit(data[0]) || data[4] < '2' || data[4] > '8')
			return false;

		for (int i = 3; i < 10; i++)
		{
			if (!isdigit(data[i]))
				return false;
		}

		return true;
	}

	return false;
}