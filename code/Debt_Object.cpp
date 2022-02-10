#include "../headers/Debt_Object.h"

// function prototypes
std::string filtered_date(const std::string &&date_val);

// dont froget to set these using class setters
unsigned int DebtObject::debt_count = 0;
double DebtObject::total_debt_amount = 0;

DebtObject::DebtObject(std::string name_val, std::string surename_val, double debt_amount_val,
                       std::string date_val)
: name{name_val}, surename {surename_val}, debt_amount {debt_amount_val}
{
    if (date_val == "add") {
        debt_count = 0;
        total_debt_amount = 0;
        this->set_current_date(); // sets the current date
    } else {
        this->date = date_val;
    }
    // date value determines what operation we are performing
    // if it is "add" then current date is set to the debt object
    // else we are either deleting or searching so we need dates from database
}

bool DebtObject::operator==(const DebtObject &obj)
{
    if (this == &obj)
        return true;
    else {
        if (this->name == obj.get_name() && this->surename == obj.get_surename() &&
            this->date == obj.get_date() && this->debt_amount == obj.get_debt_amount())
            return true;
        else
            return false;
    }
}

// derived from printable

void DebtObject::print(std::ostream &os) const
{
    os << "name: " << std::setw(20) << std::left << this->name << 
    ", surename: " << std::setw(20) << std::left << this->surename <<
    ", debt: " << std::setw(10) << std::left << this->debt_amount <<
    ", date: " << std::setw(11) << std::left << this->date;
}

// getters

unsigned int DebtObject::get_debt_count()
{
    return debt_count;
}
double DebtObject::get_total_debt_amount()
{
    return total_debt_amount;
}

std::string DebtObject::get_name() const
{
    return this->name;
}
std::string DebtObject::get_surename() const
{
    return this->surename;
}
double DebtObject::get_debt_amount() const
{
    return this->debt_amount;
}
std::string DebtObject::get_date() const
{
    return this->date;
}

// setters

void DebtObject::set_name(const char &&ch)
{
    this->name = ch;
}

void DebtObject::set_surename(const char &&ch)
{
    this->surename = ch;
}

void DebtObject::set_debt_count(unsigned int debt_count_val)
{
    debt_count += debt_count_val;
}
void DebtObject::set_total_debt_amount(double total_debt_amount_val)
{
    total_debt_amount += total_debt_amount_val;
}
void DebtObject::set_current_date()
{
    time_t now = time(0);
    this->date = filtered_date(ctime(&now));
}

// filter the date (take out weekday and clock)
std::string filtered_date(const std::string &&date_val)
{
    std::string filtered {};
    for (size_t i {}; i < date_val.length(); i++) {
        if (i <= 3 || (i >= 10 && i <= 18) || i == date_val.length() - 1)
            continue;
         filtered += date_val[i];
    }
    filtered[0] = std::tolower(filtered[0]);
    return filtered;
}
