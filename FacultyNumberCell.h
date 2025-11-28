#pragma once
#include "Cell.h"

//Cell in CSV file, which contains a valid faculty number

class FacultyNumberCell : public Cell
{
public:
	FacultyNumberCell(const std::string& data);
	void setData(const std::string& data);

	Cell* clone() const override;
	std::string toString() const override;
	bool operator<(const Cell& other) const override;


	static bool isValidFN(const std::string& data);// checks whether the text is a valid faculty number in FMI
private:
	std::string data;

	
};

