#ifndef BC863540_24FF_4312_99B4_0CD87978408A
#define BC863540_24FF_4312_99B4_0CD87978408A

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <windows.h>
#include <algorithm>
#include <cctype>
#include "Util.h"
#include "Debt_Object.h"

//
// operation functions this program performs
//

void add_debt()
{
    std::string name {};
    std::string surename {};
    std::string debt {};

    std::stringstream stream;

    std::cout << "Enter person's name -> ";
    std::getline(std::cin, name);
    name = alpha_string(name);
    trim_string(name);
    lowercase_string(name);

    std::cout << "Enter person's surename -> ";
    std::getline(std::cin, surename);
    surename = alpha_string(surename);
    trim_string(surename);
    lowercase_string(surename);

    std::cout << "Enter person's debt -> ";
    std::getline(std::cin, debt);
    trim_string(debt);
    stream.str(debt);

    double debt_amount {};

    stream >> debt_amount;
    if (name == "" && surename == "") {
        std::cout << "You must specify name and surename (or both)" <<
        std::endl;
        return;
    } else {
        if (stream.fail()) {
            std::cout << "You must specify the debt amount (in numbers)" <<
            std::endl;
            return;
        } else {
            // first, open database to retrieve debt_count and total_debt_amount
            std::ifstream database_read;
            database_read.open("../DataBase/Debt_Database.txt");
            if (!database_read) {
                std::cerr << "Error occured when trying to open database"
                << std::endl;
                return;
            }

            std::string line_0 {};
            std::getline(database_read, line_0);
            database_read.close();

            std::string *line_0_filtered {filter_database_line(line_0, 2)};

            unsigned int debt_count {}; // add validation to not be a negative
            double total_debt_amount {};

            std::stringstream stream;
            stream.str(line_0_filtered[0]);
            stream >> debt_count;
            stream.clear();
            stream.str(line_0_filtered[1]);
            stream >> total_debt_amount;

            delete [] line_0_filtered;

            DebtObject *debt_object = new DebtObject{name, surename, debt_amount};

            // this if else logic adds spaces into empty fields to not get garbage data
            // when printing to the console
            if (name == "") {
                debt_object->set_name(' ');
            } else if (surename == "") {
                debt_object->set_surename(' ');
            } else {
                // no code goes here
            }

            debt_object->set_debt_count(debt_count + 1); // because we are adding 1 debt
            debt_object->set_total_debt_amount(total_debt_amount +
            debt_object->get_debt_amount());

            write_to_database(debt_object);

            delete debt_object;
        }

    }
}

void search_debt(const std::string &&_for,
                 std::vector<DebtObject *> *const debts = nullptr)
{
    std::string name {};
    std::string surename {};
    std::string date {};

    std::cout << "Enter person's name -> ";
    std::getline(std::cin, name);
    name = alpha_string(name);
    trim_string(name);
    lowercase_string(name);

    std::cout << "Enter person's surename -> ";
    std::getline(std::cin, surename);
    surename = alpha_string(surename);
    trim_string(surename);
    lowercase_string(surename);

    std::cout << "Enter the date debt was made -> ";
    std::getline(std::cin, date);
    trim_string(date);
    lowercase_string(date);

    if (name == "" && surename == "") {
        std::cout << "You must specify name and surename (or both)" <<
        std::endl;
        return;
    } else {
        DebtObject *debt_object = new DebtObject{name, surename, 0, date};

        if (name == "" && date == "") {
            search_database(debt_object, "surename", _for, debts);
        } else if (surename == "" && date == "") {
            search_database(debt_object, "name", _for, debts);
        } else if (name == "") {
            search_database(debt_object, "surename&date", _for, debts);
        } else if (surename == "") {
            search_database(debt_object, "name&date", _for, debts);
        } else if (date == "") {
            search_database(debt_object, "name&surename", _for, debts);
        } else {
            search_database(debt_object, "all", _for, debts);
        }
        delete debt_object;
    }
}

void remove_debt()
{
    // search of delete item goes here
    std::vector<DebtObject *> *const debts = new std::vector<DebtObject *>;
    search_debt("delete", debts);

    if (debts->size() == 0) {
        return;
    } else {
        std::cout << "Enter [b] or [back] to go back" << std::endl;
        std::cout << "Enter the number of debt you want to delete -> ";

        std::string number {};

        std::getline(std::cin, number);
        trim_string(number);
        lowercase_string(number);
        //std::cout << input << std::endl;
        std::stringstream stream;
        size_t choice {};

        // if input is not an integer start over if it is q or quit then quit
        if (number == "b" || number == "back") {
            return;
        } else if (number.length()) {
            stream.str(number);
            stream >> choice;
            if (stream.fail()) {
                std::cerr << "Invald input, (Enter Numbers!)"
                << std::endl;
                return;
            }
        } else {
            std::cerr << "Invalid input" << std::endl;
            return;
        }

        if (choice <= 0 || choice > debts->size()) {
            std::cerr << "Invalid choice (input number doesn't indicate any debt)" <<
            std::endl;
            return;
        } else {
            // delete code goes here
            // delete choice - 1
            DebtObject *delete_object = (*debts)[choice - 1];

            std::ifstream database_read;
            database_read.open("../DataBase/Debt_Database.txt");
            if (!database_read) {
                std::cerr << "Error occured when trying to open database"
                << std::endl;
                return;
            }

            std::ofstream temp_write;
            temp_write.open("../DataBase/Temp.txt", std::ios::out);
            if (!temp_write) {
                std::cerr << "Error occured when trying to open database"
                << std::endl;
                return;
            }
            
            std::string line {};

            std::getline(database_read, line);
            std::string *line_0_filtered = filter_database_line(line, 2);
            
            unsigned int debt_count {}; // add validation to not be a negative
            double total_debt_amount {};

            std::stringstream stream;
            stream.str(line_0_filtered[0]);
            stream >> debt_count;
            stream.clear();
            stream.str(line_0_filtered[1]);
            stream >> total_debt_amount;
            
            delete [] line_0_filtered;

            debt_count--;
            total_debt_amount -= delete_object->get_debt_amount();

            std::string line_0 {};
            line_0 = "{debt_count:\"" + std::to_string(debt_count) + 
            "\",total_debt_amount:\"" + std::to_string(total_debt_amount) + "\"}";
            size_t deleted_count {};

            temp_write << line_0 << std::endl;
            while (std::getline(database_read, line)) {
                std::string *debt = filter_database_line(line, 4);
                std::stringstream stream;
                stream.str(debt[2]);
                double debt_amount {};
                stream >> debt_amount;
                DebtObject *from_db = new DebtObject{debt[0], debt[1], debt_amount, debt[3]};
                
                if (*from_db == *delete_object && deleted_count++ == 0) {
                    delete from_db;
                    continue;
                }
                
                temp_write << line << std::endl;
                delete from_db;
            }
            
            database_read.close();
            temp_write.close();
            
            // dont forget to loop through the vector free up the storage and free up vec
            for (size_t i {}; i < debts->size(); i++)
                delete (*debts)[i];
            delete debts;
            std::ofstream database_write;
            database_write.open("../DataBase/Debt_Database.txt", std::ios::out);
            if (!database_write) {
                std::cerr << "Error occured when trying to open database"
                << std::endl;
                return;
            }

            std::ifstream temp_read;
            temp_read.open("../DataBase/Temp.txt");
            if (!temp_read) {
                std::cerr << "Error occured when trying to open database"
                << std::endl;
                return;
            }

            std::string line_to_write {};

            while (std::getline(temp_read, line_to_write))
                database_write << line_to_write << std::endl;

            std::cout << "Debt was deleted successfully" << std::endl;
        }
    }
}

void list_all()
{
    std::ifstream database_read;
    database_read.open("../DataBase/Debt_Database.txt");
    if (!database_read) {
        std::cerr << "Error occured when trying to open database"
        << std::endl;
        return;
    }

    std::string line {};

    std::getline(database_read, line);
    std::string *totals = filter_database_line(line, 2);
    unsigned int number {};
    while (std::getline(database_read, line)) {
        std::string *debt = filter_database_line(line, 4);
        std::stringstream stream;
        stream.str(debt[2]);
        double debt_amount {};
        stream >> debt_amount;
        DebtObject *debt_object = new DebtObject{debt[0], debt[1], debt_amount, debt[3]};
        std::cout << ++number << ". " << *debt_object << std::endl;
        delete [] debt;
        delete debt_object;
    }
    std::cout << "Total debts present: " << totals[0] <<
    ", Sum of all debts in the database: " << totals[1] << std::endl;

    delete [] totals;
}

#endif /* BC863540_24FF_4312_99B4_0CD87978408A */
