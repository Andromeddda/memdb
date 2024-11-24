#include "database/table.hpp"
#include "expression/expression.hpp"

namespace memdb
{
    // Construct with string name and vector of columns
    Table::Table(const std::string& table_name, const std::vector<Column>& columns) 
    : name_(table_name), columns_(columns)
    {
        for (auto i = 0LU; i < columns_.size(); ++i)
            column_positions_[columns_[i].name_] = i;
    }

    // Construct with char* name and vector of columns
    Table::Table(const char* table_name, const std::vector<Column>& columns) 
    : name_(table_name), columns_(columns)
    {
        for (auto i = 0LU; i < columns_.size(); ++i)
            column_positions_[columns_[i].name_] = i;
    }

    std::string Table::name() 
    {
        return name_; 
    }

    size_t Table::width() const 
    {
        return columns_.size();
    }

    size_t Table::size() const 
    {
        return rows_.size();
    }

    size_t Table::column_position(const std::string& column_name) const
    {
        return column_positions_.at(column_name);
    }

    void Table::insert(const std::vector<Cell>& data)
    {
        rows_[this->size() + 1] = std::unique_ptr<Row>(new Row(this, data));
    }

    void Table::insert(std::vector<Cell>&& data)
    {
        rows_[this->size() + 1] = std::unique_ptr<Row>(new Row(this, data));
    }

    void Table::insert(const std::unordered_map<std::string, Cell>& data)
    {
        rows_[this->size() + 1] = std::unique_ptr<Row>(new Row(this, data));
    }

    // Query select method
    // Allocates new table
    Table* Table::select(
        const std::vector<std::string>& columns, const Expression& where)
    {
        // Create column list of new table
        std::vector<Column> res_columns;

        for (auto col_name : columns) // run through every selected column name
            res_columns.push_back(columns_[column_positions_[col_name]]);
        // Allocate new table
        
        Table* res = new Table("", res_columns);

        // initialize new row with the width of new table
        auto res_row = std::vector<Cell>(res->width());

        // go through every row in the table
        for (auto &[row_i, row] : rows_)
        {
            if (!where.evaluate(row.get()).get_bool())
                continue;

            // assign new row cells to old row cells
            for (auto i = 0LU; i < res->width(); ++i)
                res_row[i] = (*row)[              
                    column_positions_[               
                        res_columns[i].name_ // (1) name of column
                        ]                    // (2) index of column with this name in the old table 
                    ];                       // (3) column on this index 

            // move new row to new table
            res->insert(std::move(res_row));
        }

        return res;
    }

    void Table::drop(const Expression& where)
    {
        for (auto &[row_i, row] : rows_)
        {
            if (!where.evaluate(row.get()).get_bool())
                continue;

            rows_.erase(row_i);
        }
    }

    void Table::update(
        const std::unordered_map<std::string, Expression>& assignment, const Expression& where)
    {
        for (auto &[i, row] : rows_)
        {
            if (!where.evaluate(row.get()).get_bool())
                continue;

            for (auto &[col_name, rhs] : assignment)
            {
                (*row)[column_positions_[col_name]] = rhs.evaluate(row.get());
            }
        }
    }

} // namespace memdb