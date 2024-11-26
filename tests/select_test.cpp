#include <gtest/gtest.h>
#include <iostream>

#include "database/database.hpp"

using namespace memdb;

TEST(QueryTest, Select) 
{
    Database db;

    db.execute("create table tab1 (name : string, value : int32)");

    db.execute("insert (\"a\", 1) to table");

    db.execute("insert (\"b\", 10) to table");

    auto res = db.execute("select name from table where value > 5");

    ASSERT_TRUE(res.ok());
    ASSERT_TRUE(res.get_table()->size() == 1);

    res.print(std::cout);
}