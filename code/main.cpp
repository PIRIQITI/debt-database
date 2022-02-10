#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <windows.h>
#include <algorithm>
#include <cctype>
#include "../headers/Debt_Object.h"
#include "../headers/Operations.h"
#include "../headers/Util.h"

void display_options();
// implement press sny key to continue
int main()
{
    std::cout << "Welcome to debt storing database." << std::endl;

    std::string input {};

    do {
        display_options();

        std::getline(std::cin, input);
        trim_string(input);
        lowercase_string(input);
        //std::cout << input << std::endl;
        std::stringstream stream;
        int choice {};

        // if input is not an integer start over if it is q or quit then quit
        if (input == "q" || input == "quit") {
            break;
        } else if (input == "clear") {
            system("cls");
            continue;
        } else if (input.length()) {
            stream.str(input);
            stream >> choice;
            if (stream.fail()) {
                std::cerr << "Invald input, please try again (Enter Numbers!)"
                << std::endl;
                continue;
            }
        } else {
            std::cerr << "Invalid input, please try again" << std::endl;
            continue;
        }

        switch (choice) {
            case 1:
                // add
                add_debt();
                break;
            case 2:
                // search
                search_debt("search");
                break;
            case 3:
                // remove
                remove_debt();
                break;
            case 4:
                // list all
                list_all();
                break;
            default:
                std::cerr << "Invalid input, please try again" <<
                "(Enter Numbers 1, 2, 3 or 4)" << std::endl;
        }

    } while (true);

    return 0;
}

void display_options()
{
    std::cout
    << "==================================" << std::endl
    << "1. Add a debt to the database"      << std::endl
    << "2. Search a debt in the database"   << std::endl
    << "3. Remove a debt from the database" << std::endl
    << "4. List all debts"                  << std::endl
    << "type [clear] to clear the console"  << std::endl
    << "==================================" << std::endl
    << "\n-> ";
}
