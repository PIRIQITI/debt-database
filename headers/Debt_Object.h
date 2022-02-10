#ifndef F272095C_126E_47EF_A88F_7DF9ABFB175A
#define F272095C_126E_47EF_A88F_7DF9ABFB175A

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <windows.h>
#include <algorithm>
#include <cctype>
#include "Printable.h"

class DebtObject final: public Printable
{
private:
    // static variable to count how many debts we have
    static unsigned int debt_count;
    static double total_debt_amount;
    // person's and debt's info
    std::string name {};
    std::string surename {};
    double debt_amount {};
    std::string date {}; // set the date of debt made
public:
    DebtObject(std::string name_val, std::string surename_val, double debt_amount_val,
               std::string date_val="add");
               // default operation to perform is adding
    virtual ~DebtObject() = default;
    virtual void print(std::ostream &os) const override;
    bool operator==(const DebtObject &obj);
    // getters
    static unsigned int get_debt_count();
    static double get_total_debt_amount();
    std::string get_name() const;
    std::string get_surename() const;
    double get_debt_amount() const;
    std::string get_date() const;
    // setters
    void set_name(const char &&ch);
    void set_surename(const char &&ch);
    void set_debt_count(unsigned int debt_count_val);
    void set_total_debt_amount(double total_debt_amount_val);
    void set_current_date();
};

#endif /* F272095C_126E_47EF_A88F_7DF9ABFB175A */
