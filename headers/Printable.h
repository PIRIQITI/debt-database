#ifndef EC6936D4_BE8E_4F2E_9EC6_40D85521C4AC
#define EC6936D4_BE8E_4F2E_9EC6_40D85521C4AC

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <windows.h>
#include <algorithm>
#include <cctype>

class Printable
{
    friend std::ostream &operator<<(std::ostream &os, const Printable &obj);
public:
    Printable() = default;
    virtual ~Printable() = default;
    virtual void print(std::ostream &os) const = 0;
};

#endif /* EC6936D4_BE8E_4F2E_9EC6_40D85521C4AC */
