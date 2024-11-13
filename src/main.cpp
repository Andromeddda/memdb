#include <cstdio>
#include <vector>
#include <iostream>
#include "cell.h"

using namespace std;
using namespace memdb;


int main(void)  {
    vector<Cell> cells;

    cells.push_back(Cell(true));
    cells.push_back(Cell("hui"));
    cells.push_back(Cell(1488));
    cells.push_back(Cell());

    for (Cell c : cells) {
        cout << c.DataToString() << ", ";
    }
    cout << '\n';
    return 0;
}

