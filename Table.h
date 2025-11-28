#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <stdexcept>
#include "Cell.h"
#include "IntegerCell.h"
#include "FloatNumberCell.h"
#include "TextCell.h"
#include "CurrencyCell.h"
#include "EGNCell.h"
#include "FacultyNumberCell.h"

//class representing csv table with different functionalities
class CSVTable 
{

public:
	CSVTable() : hasHeader(false), delimiter(','), modified(false) {}
	CSVTable(const CSVTable& other);
	CSVTable& operator=(const CSVTable& other);
	~CSVTable() { clearData(); }

	// Operations with file:

	//open csv file and load it
	void open(const std::string& filename, bool hasHeader = true, char delimiter = ',');

	//save the  table in the current file
	void save();

	//save the table in a new file with a new name
	void saveAs(const std::string& newFilename);

	//print the table on the console
	void print() const;

	// Data operations

	//sort column by name
	void sort(const std::string& column, bool ascending = true);

	//sort column by index
	void sort(int columnIndex, bool ascending = true);

	//filter a column by its name, passed operator and value, for example - Age, < , 30
	void filter(const std::string& columnName, const std::string& op, const std::string& value);

	//filter a column by its index, passed operator and value
	void filter(int columnIndex, const std::string& op, const std::string& value);

	//remove the repeating rows, it leaves the first
	void removeDuplicates();

	// Column operations

	//remove column by passed name
	void removeColumn(const std::string& columnName);

	//remove column by index
	void removeColumn(int columnIndex);

	//reorder columns by new sequence
	void reorderColumns(const std::vector<int>& newOrder);

	// Row operations

	//adds row to the table
	void addRow(const std::vector<std::string>& values);

	//remove row from the table by index
	void removeRow(int rowIndex);

	//change value of a cell by row and a name of a column
	void editCell(int row, const std::string& column, const std::string& newValue);

	//change value of a cell by row and column index
	void editCell(int row, int column, const std::string& newValue);

	// Special row additions

	//adds already existing row
	void addRowCopy(int rowToCopy);

	//adds row with the minimum values of each column
	void addRowWithMinValues();

	//adds row with the maximum values of each column
	void addRowWithMaxValues();

	//adds row with the most frequent values in each column
	void addRowWithMostFrequentValues();

	//return the state of the table before the last command
	bool undo();

	//checks whether the table has been modified
	bool isModified() const { return modified; }

	//sets the modified status when change
	void setModified(bool status) { modified = status; }

private:
	std::vector<std::vector<Cell*>> data;
	std::vector<std::string> columnNames;
	bool hasHeader;
	char delimiter;
	std::string filename;
	bool modified;
	std::vector<std::vector<Cell*>> lastState;


	//delete the allocated memory for the table
	void clearData();

	//we save the last state for undo()
	void pushUndoState();

	//create the corect Cell* type from text
	Cell* createCell(const std::string& value) const;

	//determines the correct cell type by text

	CellType determineCellType(const std::string& value) const;

	//return the index of a column by its name
	int getColumnIndex(const std::string& columnName) const;
};