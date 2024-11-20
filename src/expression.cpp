#include "expression.h"

namespace memdb
{

	ValueExpression::ValueExpression(const std::string& column_name) :
		column_name_(column_name)
	{ }

	UnaryExpression::UnaryExpression(std::unique_ptr<Expression> lhs, Operation op) :
		lhs_(std::move(lhs)), op_(op)
	{ }

	BinaryExpression::BinaryExpression(std::unique_ptr<Expression> lhs, 
            std::unique_ptr<Expression> rhs, Operation op) :
		lhs_(std::move(lhs)), rhs_(std::move(rhs)), op_(op)
	{ }

	Cell ValueExpression::evaluate(Database* database)
	{
		(void)database;
		return Cell(0);
	}


	Cell UnaryExpression::evaluate(Database* database)
	{
		(void)database;
		return Cell(0);
	}

	Cell BinaryExpression::evaluate(Database* database)
	{
		(void)database;
		return Cell(0);
	}
	
} // namespace memdb