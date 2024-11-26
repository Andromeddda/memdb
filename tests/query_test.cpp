#include <gtest/gtest.h>
#include <iostream>

#include "database/database.hpp"

using namespace memdb;

TEST(QueryTest, CreateTable)
{
    Database db;
    Result res("empty table");

    ASSERT_NO_THROW({
        res = db.execute("create table tab1 (name : string, value : int32)");
        });

    ASSERT_TRUE(res.ok());

    Table* table = res.get_table();

    ASSERT_EQ(table->size(), 0);
    ASSERT_EQ(table->width(), 2);
    ASSERT_EQ(table->name(), "tab1");
}


TEST(QueryTest, InsertOrdered)
{
    Database db;
    Result res("empty table");
    db.execute("create table tab1 (name : string, value : int32)");


    ASSERT_NO_THROW({
        res = db.execute("insert (\"whatever\", 12345) to tab1");
        });


    ASSERT_TRUE(res.ok());

    Table* table = res.get_table();

    ASSERT_EQ(table->size(), 1);
    ASSERT_EQ(table->width(), 2);
    ASSERT_EQ(table->name(), "tab1");
}

TEST(QueryTest, InsertUnordered)
{
    Database db;
    Result res("empty table");
    db.execute("create table tab1 (name : string, value : int32)");
    db.execute("insert (\"whatever\", 12345) to tab1");


    ASSERT_NO_THROW({
        res = db.execute("insert (value = 11111, name = \"whatever 2\") to tab1");
        });


    ASSERT_TRUE(res.ok());

    Table* table = res.get_table();

    ASSERT_EQ(table->size(), 2);
    ASSERT_EQ(table->width(), 2);
    ASSERT_EQ(table->name(), "tab1");
}


TEST(QueryTest, Select) 
{
    Database db;
    Result res("empty table");

    db.execute("create table tab1 (name : string, value : int32)");
    db.execute("insert (\"a\", 1) to tab1");
    db.execute("insert (\"b\", 10) to tab1");


    ASSERT_NO_THROW({
        res = db.execute("select name from tab1 where value > 5");
        });


    ASSERT_TRUE(res.ok());

    Table* table = res.get_table();

    ASSERT_EQ(table->size(), 1);

    delete table;
}


TEST(QueryTest, Delete) 
{
    Database db;
    Result res("empty table");

    db.execute("create table tab1 (name : string, value : int32)");
    db.execute("insert (\"a\", 1) to tab1");
    db.execute("insert (\"b\", 10) to tab1");


    ASSERT_NO_THROW({
        res = db.execute("delete tab1 where value < 5");
        });


    ASSERT_TRUE(res.ok());

    Table* table = res.get_table();

    ASSERT_EQ(table->size(), 1);

    delete table;
}