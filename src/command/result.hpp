#ifndef HEADER_GUARD_COMMAND_RESULT_H
#define HEADER_GUARD_COMMAND_RESULT_H

#include <ostream>

namespace memdb 
{
    class Table;

    class Result
    {
    public:
        Result() = delete;
        Result(const Result& other)             = default;
        Result(Result&& other)                  = default;
        Result& operator=(const Result& other)  = default;
        Result& operator=(Result&& other)       = default;

        Result(Table* table) 
        : table_(table), status_(true), error_()
        { }

        Result(const char* error)
        : table_(nullptr), status_(false), error_(error)
        { }

        Result(const std::string& error)
        : table_(nullptr), status_(false), error_(error)
        { }

        bool ok() const             { return status_; }
        Table* get_table() const    { return table_; }

        void print(std::ostream& os);
    private:
        Table* table_;
        bool   status_;
        std::string error_;
    };
} // namespace memdb


#endif // HEADER_GUARD_COMMAND_RESULT_H