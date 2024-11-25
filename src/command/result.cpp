#include "command/result.hpp"

namespace memdb 
{
    ostream Result::operator<< (std::ostream& os) const
    {
        if (!table_ || !status_) return os;
        os << *table_;

        return os;
    }
} // namespace memdb