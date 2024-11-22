#include "database/database.hpp"
#include "parser/parser.hpp"
#include "command/command.hpp"
#include "expression/expression.hpp"


using namespace std;

class A
{
private:
	int x;
public:
	A() : x(0) { cout << "A()\n"; }
	~A() { cout << "~A()\n"; }
};

int main (void) 
{
	cout <<  "Create vector a\n";
	std::vector<std::unique_ptr<A>> a;

	cout <<  "Push to vector a\n";
	a.push_back(std::unique_ptr<A>(new A()));
	
	cout <<  "Copy vector a\n";
	std::vector<std::unique_ptr<A>>b = std::move(a);

	return 0;
}
