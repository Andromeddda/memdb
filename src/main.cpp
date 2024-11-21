#include <cstdio>
#include <vector>
#include <iostream>
#include <memory>
#include <regex>
#include <cassert>
#include <ctype.h>
#include <utility>

#include "memdb.h"


using namespace std;
using namespace memdb;


int main(void)  
{

    Parser p = Parser("a+ggg-(-c&&abc.de)*(gaga-(~baba))");

    std::unique_ptr<Expression> exp;

    p.parse_expression(exp);

    return 0;
}
