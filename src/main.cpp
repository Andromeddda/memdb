#include "database/database.hpp"
#include "parser/parser.hpp"
#include "command/command.hpp"
#include "expression/expression.hpp"

#include <ostream>
#include <iostream>


using namespace std;

template <typename A, typename B>
ostream& operator<< (ostream& os, unordered_map<A, B> mp)
{
	for (auto &[a, b] : mp) {
		os << "{ " << a << ": " << b << " } ";
	}
	os << '\n';
	return os;
}

int main (void) 
{
	std::unordered_map<int, int>
		mp;

	for (int i = 0; i < 10; i++)
		mp[i] = i*i;

	cout << mp;

	

	int max = 0;
	for (int i = 0; i < (int)mp.size(); i++)
	{
		if (mp.find(i) == mp.end())
			continue;

		if (!(mp[i] % 2 == 1))
			mp[max++] = mp[i];
	}

	int size = (int)mp.size();
	for (int i = max; i < size; i++)
		mp.erase(i);

	cout << mp;


	return 0;
}
