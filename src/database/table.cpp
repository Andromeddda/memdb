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

    void print_head_aligned(std::ostream& os, const std::vector<Column>& columns, size_t alignment)
    {
        std::string bar = std::string((alignment + 3)*columns.size() + 1, '-') + '\n';

        os << bar;

        for (auto &col : columns) {
            os << "| ";
            os << col.name_;
            os << std::string(alignment - col.name_.size(), ' ');
            os << ' ';
        }
        os << "|\n";
    }

    void print_row_aligned(std::ostream& os, const Row& row, size_t alignment)
    {   
        // positions indicating how much of each sell are already printed
        std::vector<size_t> printed(row.size(), 0);

        std::string bar = std::string((alignment + 3)*row.size() + 1, '-') + '\n';
        os << bar;

        // print untill all cells will fit
        bool fit = false;
        while (!fit) {
            fit = true;

            // run through every cell in row
            for (auto i = 0LU; i < row.size(); i++) {
                std::string cur = row[i].ToString();
                os << "| ";

                // if all cell is printed, print spaces to align
                if (printed[i] >= cur.size()) {
                    os << std::string(alignment + 1, ' ');
                    continue;
                }

                if (cur.size() - printed[i] <= alignment) 
                {
                    // print last chunk of string
                    std::string last_chunk = std::string(cur.begin() + printed[i], cur.end());
                    os << last_chunk;
                    os << std::string(alignment - last_chunk.size(), ' ');
                    os << " ";
                    printed[i] = cur.size();
                }
                else {
                    // print another chunk of string
                    os << std::string(cur.begin() + printed[i], cur.begin() + alignment);
                    os << " ";
                    printed[i] += alignment;
                    fit = false;
                }
            }

            os << "|\n";
        }
    }

    void Table::print(std::ostream& os)
    {
        size_t alignment = 0;


        for (auto &col : columns_)
            alignment = std::max(alignment, col.name_.size());

        std::string bar = std::string((alignment + 3)*columns_.size() + 1, '-') + '\n';

        os << '\n';
        if (!name_.empty()) {
            os << "TABLE \"";
            os << name_ << "\"\n";
        }


        print_head_aligned(os, columns_, alignment);

        for (auto &[i, row] : rows_)
            print_row_aligned(os, *row, alignment);

        os << bar;     
    }



    // -----------------
    // | 12345 | 12345 |

} // namespace memdb