#ifndef  HEADER_GUARD_DATABASE_H
#define  HEADER_GUARD_DATABASE_H

#include <unordered_map>

#include "table.h"

namespace memdb 
{
	class Database
	{
	private:
		std::unordered_map<std::string, Table*>
			tables_;
	};
} // namespace memdb

#endif // HEADER_GUARD_DATABASE_H
