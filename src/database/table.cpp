#include "database/table.hpp"
#include "expression/expression.hpp"

namespace memdb
{
    Table::Table(const std::string& table_name, 
            const std::vector<Column>& columns) :
    name_(table_name), columns_(columns), rows_()
    {
        for (auto i = 0LU; i < columns_.size(); ++i)
            column_map_[columns_[i].name_] = i;
    }

    Table::Table(const char* table_name, 
            const std::vector<Column>& columns) :
    name_(table_name), columns_(columns), rows_()
    {
        for (auto i = 0LU; i < columns_.size(); ++i)
            column_map_[columns_[i].name_] = i;
    }

    std::string Table::name() 
    {
        return name_; 
    }

    // Print the table to the output stream
    void Table::display(std::ostream& os) {
        (void)os;
    }

    size_t Table::width() const 
    {
        return columns_.size();
    }

    size_t Table::column_index(const std::string& column_name)
    {
        return column_map_.at(column_name);
    }

    std::shared_ptr<Table> Table::select(
        const std::vector<std::string>& columns, Expression* where)
    {
        // Create column map
        std::vector<Column> res_columns;
        for (auto col_name : columns) // run through every selected column name
            res_columns.push_back(
                columns_[column_map_[col_name]]
            );

        auto res = std::shared_ptr<Table>(new Table("", res_columns));

        // initialize new row with the width of new table
        auto res_row = std::vector<Cell>(res->width());

        for (Row& row : rows_)
        {
            Cell eval = where->evaluate(row);

            if (eval.get_type() != CellType::BOOL)
                throw IncorrectWhereStatementException();

            if (!eval.get_bool())
                continue;

            for (auto i = 0LU; i < res->width(); ++i)
                res_row[i] = row[              
                    column_map_[               
                        res_columns[i].name_ // (1) name of column
                        ]                    // (2) index of column with this name in the old table 
                    ];                       // (3) column on this index 

            // move new row to new table
            res->insert_row_ordered(std::move(res_row));
        }

        return res;
    }

    void Table::insert_row_ordered(const std::vector<Cell>& data)
    {
        rows_.push_back(Row(this, data));
    }

    void Table::insert_row_ordered(std::vector<Cell>&& data)
    {
        rows_.push_back(Row(this, data));
    }

    void Table::insert_row_unordered(const std::unordered_map<std::string, Cell>& data)
    {
        rows_.push_back(Row(this, data));
    }


} // namespace memdb