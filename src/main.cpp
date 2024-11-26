#include "database/database.hpp"
#include "parser/parser.hpp"
#include "command/command.hpp"
#include "expression/expression.hpp"

#include <ostream>
#include <iostream>


using namespace std;
using namespace memdb;

static const char help[] = 
	"\n=== memdb ===\n\n.quit or .exit - terminate the program\n\n\
.history - show session history\n\n\
CREATE TABLE <name> <column descriptions>\n\t column description: ([{key | unique | autoincrement} <column_name> : <type>])\n\n\
SELECT <column list> FROM <table> [WHERE <condition>]\n\n\
INSERT <row> TO <table>\n\n\
UPDATE <table> SET <assignments>\n\t assignment: <column_name> = <expression>\n\n\
DELETE <table> WHERE <contition>\n\n";

int main (void) 
{
	std::string input;
	Database db;

	vector<string> history;

	while (1) {
		cout << "memdb> ";
		std::getline(cin, input);

		if (input == ".exit" || input == ".quit")
			break;

		if (input == ".help"){
			cout << help;
			continue;
		}

		if (input == ".history") {
			for (auto& q : history)
				cout << q << '\n';
			continue;
		}

		Result res = db.execute(input);
		res.print(cout);

		if (res.ok())
			if (history.empty() || history[history.size() - 1] != input)
				history.push_back(input);
	}
	return 0;
}
