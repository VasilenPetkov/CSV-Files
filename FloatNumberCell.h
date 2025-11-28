#pragma once
#include "Cell.h"

//Cell in CSV file, which has floating number value

class FloatNumberCell : public Cell
{

public:
	FloatNumberCell(double data);
	void setData(double data);

	Cell* clone() const override;
	std::string toString() const override;
	bool operator<(const Cell& other) const override;

private:
	double data;
};

