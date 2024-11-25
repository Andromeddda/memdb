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
	std::string input;
	Database db;

	while (1) {
		cout << "memdb> ";
		std::getline(cin, input);

		if (input == ".exit") break;

		Parser p{input};

		Command c;

		if (p.parse(c)) {
			auto res = c.execute(&db);

			if (res.ok())
				res.print(cout);
		}
		else {
			cout << "parse error\n";
		}
	}
	return 0;
}
