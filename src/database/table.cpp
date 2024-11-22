#include "database/table.hpp"
#include "expression/expression.hpp"

namespace memdb
{
    // Construct with string name and vector of columns
    Table::Table(const std::string& table_name, 
            const std::vector<Column>& columns) :
    name_(table_name), columns_(columns), rows_()
    {
        for (auto i = 0LU; i < columns_.size(); ++i)
            column_map_[columns_[i].name_] = i;
    }

    // Construct with char* name and vector of columns
    Table::Table(const char* table_name, 
            const std::vector<Column>& columns) :
    name_(table_name), columns_(columns), rows_()
    {
        for (auto i = 0LU; i < columns_.size(); ++i)
            column_map_[columns_[i].name_] = i;
    }

    // getter for name
    std::string Table::name() 
    {
        return name_; 
    }

    // Print the table to the output stream
    void Table::display(std::ostream& os) {
        (void)os;
    }

    // get the number of columns
    size_t Table::width() const 
    {
        return columns_.size();
    }

    size_t Table::size() const 
    {
        return rows_.size();
    }

    // get the number of column by its name
    size_t Table::column_index(const std::string& column_name)
    {
        return column_map_.at(column_name);
    }

    Column Table::get_column(const std::string& column_name)
    {
        return columns_[column_index(column_name)];
    }

    void Table::insert_row_ordered(const std::vector<Cell>& data)
    {
        rows_[this->size() + 1] = std::unique_ptr<Row>(new Row(this, data));
    }

    void Table::insert_row_ordered(std::vector<Cell>&& data)
    {
        rows_[this->size() + 1] = std::unique_ptr<Row>(new Row(this, data));
    }

    void Table::insert_row_unordered(const std::unordered_map<std::string, Cell>& data)
    {
        rows_[this->size() + 1] = std::unique_ptr<Row>(new Row(this, data));
    }

    // Query select method
    // Allocates new table
    std::shared_ptr<Table> Table::select(
        const std::vector<std::string>& columns, Expression* where)
    {
        // Create column list of new table
        std::vector<Column> res_columns;

        for (auto col_name : columns) // run through every selected column name
            res_columns.push_back(get_column(col_name));

        // Allocate new table
        auto res = std::shared_ptr<Table>(new Table("", res_columns));

        // initialize new row with the width of new table
        auto res_row = std::vector<Cell>(res->width());

        // go through every row in the table
        for (auto &[j, row] : rows_)
        {
            bool selected = where ? where->evaluate(row.get()).get_bool() : true;
            if (!selected) continue;

            // assign new row cells to old row cells
            for (auto i = 0LU; i < res->width(); ++i)
                res_row[i] = (*row)[              
                    column_map_[               
                        res_columns[i].name_ // (1) name of column
                        ]                    // (2) index of column with this name in the old table 
                    ];                       // (3) column on this index 

            // move new row to new table
            res->insert_row_ordered(std::move(res_row));
        }

        return res;
    }

    void Table::delete_where(Expression* where)
    {
        for (auto &[i, row] : rows_)
        {
            bool selected = where ? where->evaluate(row.get()).get_bool() : true;
            if (!selected) continue;

            rows_.erase(i);
        }
    }

    void Table::update(
        const std::unordered_map<std::string, std::unique_ptr<Expression>>& assignment, Expression* where)
    {
        for (auto &[i, row] : rows_)
        {
            bool selected = where ? where->evaluate(row.get()).get_bool() : true;
            if (!selected) continue;

            for (auto &[destination, assignment] : assignment)
            {
                (*row)[column_index(destination)] = assignment->evaluate(row.get());
            }
        }
    }

} // namespace memdb