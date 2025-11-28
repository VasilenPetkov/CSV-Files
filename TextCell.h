#pragma once
#include "Cell.h"

//Cell in CSV file, which contains text

class TextCell : public Cell
{
public:
	TextCell(const std::string& data);
	void setData(const std::string& data);

	Cell* clone() const override;
	std::string toString() const override;
	bool operator<(const Cell& other) const override;

	std::string removeQuotationMarks() const; //removes quotation marks("" or '') if the text contains them

private:
	std::string data;
};

