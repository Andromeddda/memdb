#include "database/database.hpp"
#include "parser/parser.hpp"
#include "command/command.hpp"
#include "expression/expression.hpp"
#include "prompt_utils.hpp"

#include <ostream>
#include <iostream>


using namespace std;
using namespace memdb;


int main (void) 
{

	Database db;

    db.execute("create table tab1 (name : string, value : int32)");

    db.execute("insert (\"a\", 1) to tab1");

    auto res = db.execute("insert (\"b\", 10) to tab1");
    res.print(cout);
    res = db.execute("select name from tab1 where value < 5");

    res.print(cout);


	// std::string input;
	// Database db;

	// vector<string> history;

	// while (1) {
	// 	cout << "memdb> ";
	// 	std::getline(cin, input);

	// 	if (input == ".exit" || input == ".quit")
	// 		break;

	// 	if (input == ".help"){
	// 		cout << help;
	// 		continue;
	// 	}

	// 	if (input == ".history") {
	// 		for (auto& q : history)
	// 			cout << q << '\n';
	// 		continue;
	// 	}

	// 	Result res = db.execute(input);
	// 	res.print(cout);

	// 	if (res.ok())
	// 		if (history.empty() || history[history.size() - 1] != input)
	// 			history.push_back(input);
	// }
	return 0;
}
