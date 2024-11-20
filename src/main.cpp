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
    std::variant<std::array<std::byte, 5>, std::array<char, 5>> a(std::array<char, 5>{'a', 'b', 'c', 'd', 'e'});

    std::array<std::byte, 5> b = std::get<std::array<std::byte, 5>>(a);

    for (int i = 0; i < 5; i++)
        cout << (char)b[i] << ' ';

    return 0;
}
