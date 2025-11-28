#pragma once
#include "Cell.h"

//Cell in CSV file, which contains EGN 

class EGNCell : public Cell
{
public:
	EGNCell(const std::string& data);
	void setData(const std::string& data);

	Cell* clone() const override;
	std::string toString() const override;
	bool operator<(const Cell& other) const override;

	
	static bool isValidEGN(const std::string& data); //checks whether the passed text is a valid Bulgarian EGN
	static bool isValidDate(int day, int month, int year); // checks whether the day, month and year form a valid date
private:
	std::string data;

};

