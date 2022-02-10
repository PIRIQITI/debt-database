#ifndef BB922FA8_B493_42E1_9124_959519EFA373
#define BB922FA8_B493_42E1_9124_959519EFA373

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <windows.h>
#include <algorithm>
#include <cctype>
#include <climits>
#include "Debt_Object.h"
#include "Util.h"

//
// helper functions
//

void lowercase_string(std::string &str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char ch) {
        return std::tolower(ch);
    });
}

std::string alpha_string(const std::string &str)
{
    std::string result {};
    for (size_t i {}; i < str.length(); i++) {
        if (str[i] == ' ') {
            result += str[i];
            continue;
        } else if (std::isalpha(str[i]))
            result += str[i];
    }
    return result;
}

void trim_string(std::string &str)
{
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), str.end());
}

/* filters formatted line of database, takes out data and returns a pointer
containing (name, surename, debt and date) or (debt_count and total_debt_amount) */
std::string* filter_database_line(const std::string &line, const unsigned short &&data_count)
{
    // filter database from all of it's special characters
    // take the data out divide into pieces (name, surename, debt, date)
    // and return a std::string pointer containing those
    std::string *result = new std::string[data_count];
    int *indexes = new int[data_count * 2];
    for (size_t i {}, j {}; i < line.length(); i++) {
        if (line[i] == '\"')
            indexes[j++] = i;
    }
    
    for (size_t i {}, l {}; i < data_count * 2; i+=2, l++) {
        if (i == data_count * 2)
            break;
        for (size_t j {}; j < line.length(); j++) {
            result[l] += line[++(indexes[i])];
            if (indexes[i] == indexes[i + 1] - 1)
                break;
        }
    }
    delete [] indexes;
    return result;
}

/* writes data to the database in the format which the filter_database_line() filters */
void write_to_database(const DebtObject *const debt_object)
{// open main file in read mode copy all the content in temp and add new item at the end
// then truncate the main file open temp in read mode and move the date in the main file
    std::ifstream database_read;
    database_read.open("../DataBase/Debt_Database.txt");
    if (!database_read) {
        std::cerr << "Error occured when trying to open database" << std::endl <<
        "Failed to add the debt to the database" << std::endl;
        return;
    }

    // write new debt_count and total_debt_amount
    std::string line_0 {};
    line_0 = "{debt_count:\"" + std::to_string(debt_object->get_debt_count()) + 
    "\",total_debt_amount:\"" + std::to_string(debt_object->get_total_debt_amount()) +
    "\"}";

    //line_0 = "{debt_count:\"\",total_debt_amount:\"\"}";
    
    std::ofstream temp_file;
    temp_file.open("../DataBase/Temp.txt", std::ios::out);
    if (!temp_file) {
        std::cerr << "Error occured when trying to open database" << std::endl <<
        "Failed to add the debt to the database" << std::endl;
        return;
    }

    std::string current_line {};
    unsigned int line_count {};
    while (std::getline(database_read, current_line)) {
        if (line_count++ == 0)
            temp_file << line_0 << std::endl;
        else
            temp_file << current_line << std::endl;

        // check if line_count reached the unsigned int limit
        if (line_count == UINT_MAX) {
            std::cerr << "Error occured when trying to open database" << std::endl <<
            "Failed to add the debt to the database (database is full)" << std::endl;
            return;
        }
    }

    database_read.close();
    temp_file.close();

    std::ifstream temp_read;
    temp_read.open("../DataBase/Temp.txt");
    if (!temp_read) {
        std::cerr << "Error occured when trying to open database" << std::endl <<
        "Failed to add the debt to the database" << std::endl;
        return;
    }
    std::ofstream main_write;
    main_write.open("../DataBase/Debt_Database.txt", std::ios::out);
    if (!main_write) {
        std::cerr << "Error occured when trying to open database" << std::endl <<
        "Failed to add the debt to the database" << std::endl;
        return;
    }
    // write 0-th line of database then new debt and then the rest of the file
    std::string line_0_main_write {};
    std::getline(temp_read, line_0_main_write);

    main_write << line_0_main_write << std::endl;

    std::string new_debt {};
    new_debt = "{name:\"" + debt_object->get_name() +
    "\",surename:\"" + debt_object->get_surename() +
    "\",debt_amount:\"" + std::to_string(debt_object->get_debt_amount()) +
    "\",date:\"" + debt_object->get_date() + "\"}";

    main_write << new_debt << std::endl;
    

    std::string line {};
    
    while (std::getline(temp_read, line))
        main_write << line << std::endl;

    temp_read.close();
    main_write.close();

    std::cout << "Debt successfully added to the database" << std::endl;
}

void search_database(const DebtObject *const debt_object, std::string &&by,
                     const std::string &_for, std::vector<DebtObject *> *const debts)
{
    std::ifstream database_read;
    database_read.open("../DataBase/Debt_Database.txt");
    if (!database_read) {
        std::cerr << "Error occured when trying to open database"
        << std::endl;
        return;
    }
    
    std::string line {};
    unsigned int number {};

    std::string line_0 {};
    std::getline(database_read, line_0);

    while (std::getline(database_read, line)) {
        std::string *debt = filter_database_line(line, 4);
        std::stringstream stream;
        stream.str(debt[2]);
        double debt_amount {};
        stream >> debt_amount;
        DebtObject *from_db = new DebtObject{debt[0], debt[1], debt_amount, debt[3]};
        if (by == "surename" && debt_object->get_surename() == from_db->get_surename()) {
            std::cout << ++number << ". " << *from_db << std::endl;
            if (_for == "delete")
                debts->push_back(from_db);
        } else if (by == "name" && debt_object->get_name() == from_db->get_name()) {
            std::cout << ++number << ". " << *from_db << std::endl;
            if (_for == "delete")
                debts->push_back(from_db);
        } else if (by == "surename&date" && debt_object->get_surename() == from_db->get_surename()
                   && debt_object->get_date() == from_db->get_date()) {
            std::cout << ++number << ". " << *from_db << std::endl;
            if (_for == "delete")
                debts->push_back(from_db);
        } else if (by == "name&date" && debt_object->get_name() == from_db->get_name()
                   && debt_object->get_date() == from_db->get_date()) {
            std::cout << ++number << ". " << *from_db << std::endl;
            if (_for == "delete")
                debts->push_back(from_db);
        } else if (by == "name&surename" && debt_object->get_name() == from_db->get_name()
                   && debt_object->get_surename() == from_db->get_surename()) {
            std::cout << ++number << ". " << *from_db << std::endl;
            if (_for == "delete")
                debts->push_back(from_db);
       } else if (by == "all" && debt_object->get_surename() == from_db->get_surename()
                   && debt_object->get_name() == from_db->get_name()
                   && debt_object->get_date() == from_db->get_date()) {
            std::cout << ++number << ". " << *from_db << std::endl;
            if (_for == "delete")
                debts->push_back(from_db);
        } else {
            // no code goes here
        }
        
        if (debts != nullptr)
            debts->shrink_to_fit();
        
        delete [] debt;
        if (_for != "delete")
            delete from_db;
    }

    if (number == 0) {
        // if number is one no debts was found
        std::cout << "No debt was found with the information provided" << std::endl;
    }
}

#endif /* BB922FA8_B493_42E1_9124_959519EFA373 */