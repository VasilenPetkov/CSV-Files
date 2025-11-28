#include "Table.h"
#include <fstream>
#include <iostream>

CSVTable::CSVTable(const CSVTable& other) : hasHeader(other.hasHeader), delimiter(other.delimiter), filename(other.filename),
modified(other.modified), columnNames(other.columnNames)
{
	int rowsCount = other.data.size();
	for (int i = 0; i < rowsCount; i++)
	{
		std::vector<Cell*> temp;
		int columnCount = other.data[i].size();

		for (int j = 0; j < columnCount; j++)
		{
			Cell* cell = other.data[i][j]->clone();
			temp.push_back(cell);
		}

		data.push_back(temp);
	}
}


CSVTable& CSVTable::operator=(const CSVTable& other) {
	if (this != &other) {

		clearData();

		hasHeader = other.hasHeader;
		delimiter = other.delimiter;
		filename = other.filename;
		modified = other.modified;
		columnNames = other.columnNames;

		int rowsCount = other.data.size();

		for (int i = 0; i < rowsCount; i++)
		{
			std::vector<Cell*> temp;
			int columnCount = other.data[i].size();

			for (int j = 0; j < columnCount; j++)
			{
				Cell* cell = other.data[i][j]->clone();
				temp.push_back(cell);
			}

			data.push_back(temp);
		}
	}

	return *this;
}

void CSVTable::open(const std::string& filename, bool hasHeader, char delimiter)
{
	clearData();

	this->filename = filename;
	this->hasHeader = hasHeader;
	this->delimiter = delimiter;
	this->modified = false;

	std::ifstream file(filename);

	if (!file.is_open())
		throw std::runtime_error("Could not open file");

	// First we read the headers if the current table has them
	std::string line;
	if (hasHeader && std::getline(file, line))
	{
		std::string cell;
		std::stringstream ss(line);

		while (std::getline(ss, cell, delimiter))
		{
			columnNames.push_back(cell);
		}
	}


	//Then, read the rows

	while (std::getline(file, line))
	{
		std::vector<Cell*> row;
		std::string cell;
		std::stringstream ss(line);

		while (std::getline(ss, cell, delimiter))
		{
			row.push_back(createCell(cell));
		}

		if (!row.empty())
			data.push_back(row);
	}
}

void CSVTable::save()
{
	if (filename.empty())
		throw std::runtime_error("No filename specified for saving");

	saveAs(filename);
}

void CSVTable::saveAs(const std::string& newFilename)
{
	std::ofstream file(newFilename);

	if (!file.is_open())
		throw std::runtime_error("Could not open file for saving");

	try
	{
		if (hasHeader && !columnNames.empty())
		{
			for (int i = 0; i < columnNames.size(); i++)
			{
				file << columnNames[i];

				if (i < columnNames.size() - 1)
					file << delimiter;

			}

			file << std::endl;
		}

		for (int i = 0; i < data.size(); i++)
		{
			for (int j = 0; j < data[i].size(); j++)
			{
				file << data[i][j]->toString();

				if (j < data[i].size() - 1)
					file << delimiter;
			}

			file << std::endl;
		}

		file.close();
		filename = newFilename;
		modified = false;
	}

	catch (const std::exception& e)
	{
		file.close();
		throw std::runtime_error("Error while saving file");
	}
}

void CSVTable::print() const
{

	//First, we print the row with headers if the current table has them
	if (hasHeader && !columnNames.empty())
	{
		for (int i = 0; i < columnNames.size(); i++)
		{
			std::cout << columnNames[i];

			if (i < columnNames.size() - 1)
				std::cout << delimiter;
		}

		std::cout << std::endl;
	}

	// Then, we print the rows
	for (int i = 0; i < data.size(); i++)
	{
		const std::vector<Cell*>& current_row = data[i];

		if (current_row.empty())
			continue;

		for (int j = 0; j < current_row.size(); j++)
		{
			std::cout << current_row[j]->toString();

			if (j < current_row.size() - 1)
				std::cout << delimiter;

		}
		std::cout << std::endl;
	}
}

void CSVTable::sort(const std::string& column, bool ascending)
{
	int colIndex = getColumnIndex(column);
	sort(colIndex, ascending);
}

void CSVTable::sort(int colIndex, bool ascending)
{
	if (data.empty())
		return;

	if (colIndex < 0 || colIndex >= data[0].size())
		throw std::invalid_argument("Invalid column index");

	pushUndoState();

	// For the sorting, I used bubble sort algorithm

	for (int i = 0; i < data.size() - 1; i++)
	{
		for (int j = 0; j < data.size() - i - 1; j++)
		{
			bool needSwap;

			if (ascending)
			{
				needSwap = (*data[j][colIndex] > *data[j + 1][colIndex]);
			}
			else
			{
				needSwap = (*data[j][colIndex] < *data[j + 1][colIndex]);
			}

			if (needSwap)
				std::swap(data[j], data[j + 1]);
		}
	}

	modified = true;
}

void CSVTable::filter(const std::string& columnName, const std::string& op, const std::string& value)
{
	int colIndex = getColumnIndex(columnName);
	filter(colIndex, op, value);
}

void CSVTable::filter(int colIndex, const std::string& op, const std::string& value)
{
	if (data.empty())
		return;

	if (colIndex < 0 || colIndex >= data[0].size())
		throw std::invalid_argument("Invalid column index");

	pushUndoState();

	Cell* filterCell = createCell(value);
	std::vector<std::vector<Cell*>> filteredData;

	for (int i = 0; i < data.size(); i++)
	{
		bool keepRow = false;

		if (op == "==")
		{
			keepRow = (*data[i][colIndex] == *filterCell);
		}
		else if (op == "!=")
		{
			keepRow = (*data[i][colIndex] != *filterCell);
		}
		else if (op == "<")
		{
			keepRow = (*data[i][colIndex] < *filterCell);
		}
		else if (op == "<=")
		{
			keepRow = (*data[i][colIndex] <= *filterCell);
		}
		else if (op == ">")
		{
			keepRow = (*filterCell < *data[i][colIndex]);
		}
		else if (op == ">=")
		{
			keepRow = (*filterCell <= *data[i][colIndex]);
		}
		else
		{
			delete filterCell;
			throw std::invalid_argument("Invalid operator");
		}


		if (keepRow)
		{
			std::vector<Cell*> newRow;

			for (int j = 0; j < data[i].size(); j++)
			{
				newRow.push_back(data[i][j]->clone());
			}

			filteredData.push_back(newRow);
		}
	}

	for (int i = 0; i < data.size(); i++)
	{
		for (int j = 0; j < data[i].size(); j++)
		{
			delete data[i][j];
		}
	}

	delete filterCell;

	data = filteredData;
	modified = true;
}


int CSVTable::getColumnIndex(const std::string& column) const
{
	for (int i = 0; i < columnNames.size(); i++)
	{
		if (columnNames[i] == column)
			return i;
	}

	throw std::invalid_argument("Column not found");
}

void CSVTable::pushUndoState()
{
	lastState.clear();

	for (int i = 0; i < data.size(); i++)
	{
		std::vector<Cell*> newRow;

		for (int j = 0; j < data[i].size(); j++)
		{
			newRow.push_back(data[i][j]->clone());
		}

		lastState.push_back(newRow);
	}
}

Cell* CSVTable::createCell(const std::string& value) const
{
	if (!value.empty())
	{
		if (value.front() == '"' && value.back() == '"' || (value.front() == '\'' && value.back() == '\''))
		return new TextCell(value);
	}

	CellType type = determineCellType(value);
	switch (type)
	{
	case CellType::IntegerCell:
		return new IntegerCell(std::stoi(value));
	case CellType::FloatNumberCell:
		return new FloatNumberCell(std::stod(value));
	case CellType::TextCell:
		return new TextCell(value);
	case CellType::CurrencyCell:
		return new CurrencyCell(value);
	case CellType::EGNCell:
		return new EGNCell(value);
	case CellType::FNCell:
		return new FacultyNumberCell(value);
	default:
		return new TextCell(value);
	}
}

void CSVTable::removeDuplicates()
{
	if (data.empty())
		return;

	pushUndoState();

	int i = 0;
	while (i < data.size() - 1)
	{
		int j = i + 1;
		while (j < data.size())
		{
			bool areEqual = true;


			//To compare each cell in the two rows
			for (int col = 0; col < data[i].size(); col++)
			{
				if (*data[i][col] != *data[j][col])
				{
					areEqual = false;
					break;
				}
			}

			if (areEqual)
			{

				for (int k = 0; k < data[j].size(); ++k)
				{
					delete data[j][k];
				}

				data.erase(data.begin() + j);
			}
			else
			{
				j++;
			}
		}

		i++;
	}

	modified = true;
}

void CSVTable::removeColumn(const std::string& columnName)
{
	int colIndex = -1;
	
	for (int i = 0; i < columnNames.size(); i++)
	{
		if (columnNames[i] == columnName)
		{
			colIndex = i;
			break;
		}
	}

	if (colIndex == -1)
		throw std::invalid_argument("Column not found");

	removeColumn(colIndex);
}

void CSVTable::removeColumn(int columnIndex)
{
	if (data.empty())
		return;

	if (columnIndex < 0 || columnIndex >= data[0].size())
		throw std::invalid_argument("Invalid column index");


	pushUndoState(); 

	//If the column has header, remove it
	if (!columnNames.empty())
		columnNames.erase(columnNames.begin() + columnIndex);


	// Remove each cell which was part of the colum in each row
	for (int row = 0; row < data.size(); row++)
	{
		delete data[row][columnIndex]; 
		data[row].erase(data[row].begin() + columnIndex);
	}

	modified = true;
}

void CSVTable::reorderColumns(const std::vector<int>& newOrder)
{
	if (data.empty())
		return;

	const int columnCount = data[0].size();

	if (newOrder.size() != columnCount)
		throw std::invalid_argument("Invalid new order size");

	std::vector<bool> used(columnCount, false);

	for (int i = 0; i < newOrder.size(); ++i)
	{
		int index = newOrder[i];

		if (index < 0 || index >= columnCount)
			throw std::invalid_argument("Invalid column index in new order");

		if (used[index])
			throw std::invalid_argument("Duplicated column index in new order");

		used[index] = true;
	}

	pushUndoState();

	// Reorder the columns
	std::vector<std::vector<Cell*>> newData;

	for (int row = 0; row < data.size(); row++)
	{
		std::vector<Cell*> newRow;

		for (int i = 0; i <newOrder.size(); i++)
		{
			int colIndex = newOrder[i];
			newRow.push_back(data[row][colIndex]->clone());
		}


		newData.push_back(newRow);
	}

	for (int i = 0; i < data.size(); i++)
	{
		for (int j = 0; j < data[i].size(); ++j)
		{
			delete data[i][j];
		}
	}

	data = newData;

	// Reorder the headers , if the current table has them
	if (!columnNames.empty())
	{
		std::vector<std::string> newColumnNames;

		for (int i = 0; i < newOrder.size(); i++)
		{
			int colIndex = newOrder[i];
			newColumnNames.push_back(columnNames[colIndex]);
		}

		columnNames = newColumnNames;
	}

	modified = true;
}

void CSVTable::clearData()
{
	for (int i = 0; i < data.size(); ++i)
	{
		for (int j = 0; j < data[i].size(); ++j)
		{
			delete data[i][j];
		}
	}
	data.clear();
}

bool CSVTable::undo()
{
	if (lastState.empty()) 
		return false;

	clearData(); 

	for (int i = 0; i < lastState.size(); ++i)
	{
		std::vector<Cell*> row;

		for (int j = 0; j < lastState[i].size(); ++j)
		{
			row.push_back(lastState[i][j]->clone());
		}

		data.push_back(row);
	}
	modified = true;
	return true;
}

void CSVTable::addRow(const std::vector<std::string>& values)
{
	if (!data.empty() && values.size() != data[0].size())
		throw std::invalid_argument("Wrong number of values for row");

	pushUndoState();

	std::vector<Cell*> newRow;
	for (int i = 0; i < values.size(); i++)
	{
		newRow.push_back(createCell(values[i]));
	}

	data.push_back(newRow);
	modified = true;
}

void CSVTable::removeRow(int rowIndex)
{
	if (data.empty())
		throw std::runtime_error("Table is empty");

	if (rowIndex < 0 || rowIndex >= data.size())
		throw std::invalid_argument("Invalid row index");

	pushUndoState();

	for (int i = 0; i < data[rowIndex].size(); i++)
	{
		delete data[rowIndex][i];
	}

	data.erase(data.begin() + rowIndex);
	modified = true;
}

void CSVTable::editCell(int row, const std::string& column, const std::string& newValue)
{
	int colIndex = -1;

	for (int i = 0; i < columnNames.size(); i++)
	{
		if (columnNames[i] == column)
		{
			colIndex = i;
			break;
		}
	}

	if (colIndex == -1)
		throw std::invalid_argument("Column not found");

	editCell(row, colIndex, newValue);
}

void CSVTable::editCell(int row, int column, const std::string& newValue)
{
	if (data.empty())
		throw std::runtime_error("Table is empty");

	if (row < 0 || row >= data.size())
		throw std::invalid_argument("Invalid row index");

	if (column < 0 || column >= data[0].size())
		throw std::invalid_argument("Invalid column index");

	pushUndoState();

	//free memory for the old one
	delete data[row][column];

	// create the new one
	data[row][column] = createCell(newValue);
	modified = true;
}

void CSVTable::addRowCopy(int rowToCopy)
{
	if (data.empty())
		throw std::out_of_range("Table is empty");

	if (rowToCopy < 0 || rowToCopy >= data.size())
		throw std::out_of_range("Invalid row index");


	pushUndoState();

	std::vector<Cell*> newRow;
	for (int i = 0; i < data[rowToCopy].size(); i++)
	{
		newRow.push_back(data[rowToCopy][i]->clone());
	}

	data.push_back(newRow);
	modified = true;
}

void CSVTable::addRowWithMinValues()
{
	if (data.empty())
		throw std::out_of_range("Table is empty");

	pushUndoState();

	std::vector<Cell*> minRow;
	int start = hasHeader ? 1 : 0;

	for (int col = 0; col < data[0].size(); col++)
	{
		Cell* min = data[start][col]->clone();
		for (int row = start + 1; row < data.size(); row++)
		{
			if (*data[row][col] < *min)
			{
				delete min;
				min = data[row][col]->clone();
			}
		}

		minRow.push_back(min);
	}

	data.push_back(minRow);
	modified = true;
}
void CSVTable::addRowWithMaxValues()
{
	if (data.empty())
		throw std::out_of_range("Table is empty");

	pushUndoState();

	std::vector<Cell*> maxRow;
	int cols = data[0].size();
	int rows = data.size();

	int startRow = 0;
	if (hasHeader)
		startRow = 1;

	for (int col = 0; col < cols; col++)
	{
		Cell* maxCell = data[startRow][col];

		for (int row = startRow; row < rows; row++)
		{
			if (*data[row][col] > *maxCell)
				maxCell = data[row][col];
		}

		maxRow.push_back(maxCell->clone());
	}

	data.push_back(maxRow);
	modified = true;
}


void CSVTable::addRowWithMostFrequentValues() 
{
	if (data.empty()) 
		throw std::out_of_range("Table is empty");

	pushUndoState();

	std::vector<Cell*> frequentRow;
	int cols = data[0].size();
	int rows = data.size();

	int startRow = 0;
	if (hasHeader)
		startRow = 1;

	for (int col = 0; col < cols; col++)
	{
		Cell* mostFrequent = nullptr;
		int maxCount = 0;

		for (int i = startRow; i < rows; i++) 
		{
			int currentCount = 0;
			for (int j = startRow; j < rows; j++)
			{
				if (*data[i][col] == *data[j][col]) 
					currentCount++;
				
			}

			if (currentCount > maxCount) 
			{
				maxCount = currentCount;
				mostFrequent = data[i][col];
			}
		}

		frequentRow.push_back(mostFrequent->clone());
	}

	data.push_back(frequentRow);
	modified = true;
}



CellType CSVTable::determineCellType(const std::string& value) const
{
	if (value.empty()) 
		return CellType::TextCell;
	if (EGNCell::isValidEGN(value)) 
		return CellType::EGNCell;
	if (FacultyNumberCell::isValidFN(value)) 
		return CellType::FNCell;
	if (CurrencyCell::isValidCurrency(value))
		return CellType::CurrencyCell;

	try
	{
		size_t pos;
		int i = std::stoi(value, &pos);
		if (pos == value.length())
			return CellType::IntegerCell;
	}
	catch (...) {}

	try
	{
		size_t pos;
		double d = std::stod(value, &pos);
		if (pos == value.length())
			return CellType::FloatNumberCell;
	}
	catch (...) {}

	return CellType::TextCell;
}