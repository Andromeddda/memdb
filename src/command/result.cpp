#include "command/result.hpp"
#include "database/table.hpp"

namespace memdb 
{
    void Result::print(std::ostream& os)
    {
        if (!table_ || !status_) return;
        table_->print(os);
    }
} // namespace memdb