#include <cstdio>
#include <vector>
#include <iostream>
#include <memory>
#include <regex>
#include <cassert>
#include <ctype.h>

#include "table.h"
#include "command.h"

using namespace std;
using namespace memdb;

std::string unescape(const std::string& str)
{
    std::string res = "";
    for (auto i = 0LU; i < str.size(); i++)
    {
        if (str[i] != '\\') {
            res += str[i];
            continue;
        }

        assert(i + 1 != str.size());
        switch(str[++i]) 
        {
        case 'a': res += '\a'; break;
        case 'b': res += '\b'; break;
        case 'f': res += '\f'; break;
        case 'n': res += '\n'; break;
        case 'r': res += '\r'; break;
        case 't': res += '\t'; break;
        case 'v': res += '\v'; break;
        case '\'': res += '\''; break;
        case '\"': res += '\"'; break;
        case '\\': res += '\\'; break;
        case '\?': res += '\?'; break;
        case 'x':
            char hex = 0;
            assert(i + 1 != str.size());
            if (isxdigit(str[++i])) hex += str[i];
            assert(i + 1 != str.size());
            if (isxdigit(str[++i])) { hex *= 16; hex += str[i]; }
            res += hex;
            break;  
        }
    }
    return res;
}

int main(void)  
{
    string a = "tab: \"\\t\", newline: \"\\n\", hex: \"\\x31\"";

    cout << "a:\n" << a << "\n\n";

    cout << "unescape(a):\n" << unescape(a) << '\n';
    return 0;
}


