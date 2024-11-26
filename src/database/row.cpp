#include "database/row.hpp"
#include "database/table.hpp"

namespace memdb
{
    Row::Row(Table* table, const std::vector<Cell>& data) :
        table_(table), data_(data)
    { }

    Row::Row(Table* table, std::vector<Cell>&& data) :
        table_(table), data_(data)
    { }

    Row::Row(Table* table, const std::unordered_map<std::string, Cell>& data) :
        table_(table)
    {
        data_.resize(table->width());
        for (auto &[name, cell]: data) {
            data_[table->column_position(name)] = cell;
        }
    }

    Cell& Row::operator[] (size_t index)
    {
        return data_[index];
    }
    const Cell& Row::operator[] (size_t index) const
    {
        return data_[index];
    }

    Table* Row::get_table() const
    {
        return table_;
    }
} // namespace memdb