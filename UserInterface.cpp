
#include <iostream>
#include "UserInterface.h"

void UserInterface::run()
{
	std::cout << "CSV Table Commands" << std::endl;
	printHelp();

	std::string command;
	
	while (true)
	{
		std::cout << "Enter command: ";
		std::cin >> command;
		std::cin.ignore();

		if (command == "exit")
		{
			if (handleExit())
				break;
		}

		else if (!executeCommand(command))
			std::cout << "Invalid command or error. Enter help for options." << std::endl;
	}
}

void UserInterface::printHelp() const
{
	std::cout << "Available commands: " << std::endl;
	std::cout << "open           - Open CSV file" << std::endl;
	std::cout << "save           - Save to current file" << std::endl;
	std::cout << "saveas         - Save to new file" << std::endl;
	std::cout << "addrow         - Add new row" << std::endl;
	std::cout << "sort           - Sort table" << std::endl;
	std::cout << "filter         - Filter rows" << std::endl;
	std::cout << "removedups     - Remove duplicates" << std::endl;
	std::cout << "removecol      - Remove column" << std::endl;
	std::cout << "reordercolumns - Reorder columns by index" << std::endl;
	std::cout << "editcell       - Edit cell value" << std::endl;
	std::cout << "undo           - Undo last action" << std::endl;
	std::cout << "print          - Display table" << std::endl;
	std::cout << "exit           - Exit program" << std::endl;
	std::cout << "help           - Show this help menu" << std::endl;
}

bool UserInterface::executeCommand(const std::string& command)
{
	if (command == "open")
		return handleOpen();

	if (command == "save")
		return handleSave();

	if (command == "saveas")
		return handleSaveAs();

	if (command == "addrow")
		return handleAddRow();

	if (command == "sort")
		return handleSort();

	if (command == "filter")
		return handleFilter();

	if (command == "removedups")
		return handleRemoveDuplicates();

	if (command == "removecol")
		return handleRemoveColumn();

	if (command == "reordercolumns")
		return handleReorderColumns();

	if (command == "editcell")
		return handleEditCell();

	if (command == "undo")
		return handleUndo();

	if (command == "print")
		return handlePrint();

	if (command == "help")
	{
		printHelp();
		return true;
	}

	return false;
}


bool UserInterface::handleOpen()
{
	std::cout << "Enter filename: ";
	std::string fileName;
	std::cin >> fileName;

	std::cin.ignore();

	std::cout << "Does the file have a header? (Type yes or no): ";
	std::string hasHeaderInput;
	std::cin >> hasHeaderInput;

	std::cin.ignore();

	bool hasHeader = (hasHeaderInput == "Yes" || hasHeaderInput == "yes");

	std::cout << "Enter delimiter or press Enter for default ','): ";
	std::string delimiterInput;

	std::getline(std::cin, delimiterInput);

	char delimiter = delimiterInput.empty() ? ',' : delimiterInput[0];

	try
	{
		table.open(fileName, hasHeader, delimiter);
		currentFile = fileName;
		std::cout << "File loaded successfully." << std::endl;
		return true;
	}

	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return false;
	}
}

bool UserInterface::handleSave()
{
	if (currentFile.empty())
	{
		std::cout << "No active file.Try saveas instead." << std::endl;
		return false;
	}

	try
	{
		table.save();
		std::cout << "File saved successfully." << std::endl;
		return true;
	}

	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << "\n";
		return false;
	}
}

bool UserInterface::handleSaveAs()
{
	std::cout << "Enter new filename: ";
	std::string filename;
	std::getline(std::cin, filename);

	try
	{
		table.saveAs(filename);
		currentFile = filename;
		std::cout << "File saved successfully. " << std::endl;
		return true;
	}

	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << "\n";
		return false;
	}
}

bool UserInterface::handleAddRow()
{
	std::cout << "Choose row type" << std::endl;
	std::cout << "1 - Copy existing row" << std::endl;
	std::cout << "2 - Row with min values" << std::endl;
	std::cout << "3 - Row with max vlues" << std::endl;
	std::cout << "4 - Row with frequent values" << std::endl;
	std::cout << "Your choice: ";

	int choice;
	std::cin >> choice;
	std::cin.ignore();

	try
	{
		if (choice == 1)
		{
			std::cout << "Enter row number to copy: ";
			int row;
			std::cin >> row;
			std::cin.ignore();

			table.addRowCopy(row);
		}
	
		else if (choice == 2)
		{
			table.addRowWithMinValues();
		}

		else if (choice == 3)
		{
			table.addRowWithMaxValues();
		}

		else if (choice == 4)
		{
			table.addRowWithMostFrequentValues();
		}
		else
		{
			throw std::invalid_argument("Invalid choice");
		}

		std::cout << "Row added successfully. " << std::endl;
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return false;
	}
}

bool UserInterface::handleSort()
{
	std::cout << "Enter column name or index: ";
	std::string col;
	std::getline(std::cin, col);

	std::cout << "Sort order (1 for asc, 0 for desc): ";
	int order;
	std::cin >> order;
	std::cin.ignore();

	try
	{
		if (isdigit(col[0]))
			table.sort(std::stoi(col), order);

		else
		{
			table.sort(col, order);
		}

		std::cout << "Table sorted." << std::endl;
		return true;
	}

	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return false;
	}
}

bool UserInterface::handleFilter()
{
	std::cout << "Enter column name orindex: ";
	std::string col;
	std::getline(std::cin, col);

	std::cout << "Enter operator ==,!=,<,<=,>,>=: ";
	std::string op;

	std::cin >> op;
	std::cin.ignore();

	std::cout << "Enter value: ";
	std::string value;

	std::cin >> value;
	std::cin.ignore();

	try
	{
		if (isdigit(col[0]))
			table.filter(std::stoi(col), op, value);

		else
		{
			table.filter(col, op, value);
		}

		std::cout << "Table is filtered. " << std::endl;
		return true;
	}

	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << "\n";
		return false;
	}
}

bool UserInterface::handleRemoveDuplicates()
{
	try
	{
		table.removeDuplicates();
		std::cout << "Duplicates removed." << std::endl;
		return true;
	}

	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return false;
	}
}

bool UserInterface::handleRemoveColumn()
{
	std::cout << "Enter column name or index: ";
	std::string col;
	std::getline(std::cin, col);

	try
	{
		if (isdigit(col[0]))
		{
			table.removeColumn(std::stoi(col));
		}
		else
		{
			table.removeColumn(col);
		}
		

		std::cout << "Column removed. " << std::endl;
		return true;
	}

	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return false;
	}
}


bool UserInterface::handleReorderColumns()
{
	std::cout << "Enter new order of column indexes (space-separated): ";
	
	std::string line;
	std::getline(std::cin, line);
	std::stringstream ss(line);

	std::vector<int> newOrder;
	int index;

	while (ss >> index)
	{
		newOrder.push_back(index);
	}

	try
	{
		table.reorderColumns(newOrder);
		std::cout << "Columns reordered successfully." << std::endl;
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return false;
	}
}

bool UserInterface::handleEditCell()
{
	std::cout << "Enter row index: ";
	int row;
	std::cin >> row;
	std::cin.ignore();

	std::cout << "Enter column name: ";
	std::string column;
	std::getline(std::cin, column);

	std::cout << "Enter new value: ";
	std::string newValue;
	std::getline(std::cin, newValue);

	try
	{
		table.editCell(row, column, newValue);
		std::cout << "Cell is updated successfully." << std::endl;
		return true;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return false;
	}
}

bool UserInterface::handleUndo() {
	try
	{
		if (table.undo())
		{
			std::cout << "Undo successful." << std::endl;
			return true;
		}

		std::cout << "Nothing to undo. " << std::endl;
		return false;
	}

	catch (const std::exception& e) 
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return false;
	}
}

bool UserInterface::handlePrint()
{
	try
	{
		table.print();
		return true;
	}

	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return false;
	}
}

bool UserInterface::handleExit()
{
	if (table.isModified())
	{
		std::cout << "You have unsaved changes. Do you want to save before exit? Yes or no: ";
		std::string choice;
		std::cin >> choice;
		std::cin.ignore();

		if (choice == "Yes" || choice == "yes")
			return handleSave();
	}

	return true;
}