#pragma once
#include "Table.h"
#include <iostream>
#include <string>

class UserInterface {
public:
    void run();

private:
    CSVTable table;
    std::string currentFile;

    //shows list with the available commands and their description
    void printHelp() const;

    //compares the user input with the existing command and returns if the command is succesfull
    bool executeCommand(const std::string& command);

    //handling loading csv file
    bool handleOpen();

    //handling saving current file
    bool handleSave();

    //handling saving to a new file, with a name passed by the user
    bool handleSaveAs();

    //handling adding a new row to the file with chosen type (duplicate, minValues, maxValues, frequentValues)
    bool handleAddRow();

    //handling sorting the table by column index, ascending or descending
    bool handleSort();

    //handling filtering different rows 
    bool handleFilter();

    //handling removing the duplicates
    bool handleRemoveDuplicates();

    //handling the removing of column
    bool handleRemoveColumn();

    //handlng the reorder of culumns
    bool handleReorderColumns();

    //handling changing the value of a cell
    bool handleEditCell();

    //handling the cancelation of last change, if a last change exist
    bool handleUndo();

    //handle the printing to the console
    bool handlePrint();

    //handles the exit of the program
    bool handleExit();

};
