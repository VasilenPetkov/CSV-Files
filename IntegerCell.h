#pragma once
#include "Cell.h"

//Cell in CSV file, which has integer value

class IntegerCell : public Cell
{
public:
	IntegerCell(int data);
	void setData(int newData);

	Cell* clone() const override;
	std::string toString() const override;
	 bool operator<(const Cell& other) const override;

private:
	int data;
};

