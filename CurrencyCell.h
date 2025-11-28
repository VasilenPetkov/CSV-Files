#pragma once
#include "Cell.h"

//Cell in CSV file, which contains value in specific currency

class CurrencyCell : public Cell
{
public:
	CurrencyCell(const std::string& data);
	void setData(const std::string& data);

	Cell* clone() const override;
	std::string toString() const override;
	bool operator<(const Cell& other) const override;


	static bool isUppercaseLetters(const std::string& str); // checks whether the passed text consists of uppercase letters
	static bool isValidCurrency(const std::string& str); // checks whether the passed text is valid currency

	static double getValue(const std::string& str); // extract the value part of the currency text
	static std::string getCurrencyChar(const std::string& str); // extract the letters representing the type of currency in the text
private:
	std::string data;
};

