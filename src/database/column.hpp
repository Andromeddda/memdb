#ifndef HEADER_GUARD_DATABASE_COLUMN_H
#define HEADER_GUARD_DATABASE_COLUMN_H

#include <string>
#include <map>
#include <unordered_map>
#include "cell/cell.hpp"

namespace memdb 
{
    enum ColumnAttribute
    {
        Key = 1,
        Unique = 2,
        Autoincrement = 4
    };

    typedef typename std::map<Cell, size_t, CellCompare> Index;

    struct Column
    {
        CellType        type_;
        std::string     name_;
        unsigned char   attributes_;
        Index index_;


        Column();
        Column(const char *name);
        Column(const std::string& name);
        Column(CellType type, const std::string& name, unsigned char attributes);
        Column(CellType type, const char* name, unsigned char attributes);

        Column(const Column& other) = default;
        Column(Column&& other) = default;
        Column& operator= (const Column& other) = default;
        Column& operator= (Column&& other) = default;

        bool operator== (const Column& other) const;
    };

    struct ColumnHash
    {
        size_t operator() (const Column& lhs) const {
            return std::hash<std::string>{}(lhs.name_);
        } 
    };

} // namespace memdb

#endif