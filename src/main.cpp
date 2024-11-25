#include "database/database.hpp"
#include "parser/parser.hpp"
#include "command/command.hpp"
#include "expression/expression.hpp"

#include <ostream>
#include <iostream>


using namespace std;
using namespace memdb;

int main (void) 
{
	Parser p("SELECT a, b FROM table1 WHERE a > 10 && b < 3");

	Command c;
	p.parse(c);

	cout << "Exit\n";

	(void)c;
	return 0;
}
