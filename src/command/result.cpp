#include "command/result.hpp"
#include "database/table.hpp"

namespace memdb 
{
    void Result::print(std::ostream& os)
    {
        if (!status_)
            os << error_;

        if (!table_) 
            return;

        table_->print(os);
    }


    Result::~Result()
    {
        if (table_ && table_->name().empty())
            delete table_;
    }
} // namespace memdb