#include "expression.h"

namespace memdb
{
	static const std::unordered_map<Operation, std::string>
		op_to_str = {
			{ ADD, "+"},
			{ SUB, "-"},
			{ MUL, "*"},
			{ DIV, "/"},
			{ MOD, "%"},
			{ NEG, "-"}, //
        	{ OR,  "||"},
        	{ AND, "&&"},
        	{ NOT, "!"}, //
        	{ XOR, "^"},
        	{ BAND, "&"},
        	{ BOR, "|"},
        	{ BNEG, "~"}, //
        	{ EQ,  "=="},
        	{ NEQ, "!="},
        	{ LE,  "<"},
        	{ LEQ, "<="},
         	{ GR,  ">"},
        	{ GEQ, ">=" } 
		};
	
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

	Cell ValueExpression::evaluate(Table* table, const row_t& row)
	{
		return row[table->column_index(column_name_)];
	}

	Cell UnaryExpression::evaluate(Table* table, const row_t& row)
	{
		switch (op_)
		{
		case NEG: 	return -(lhs_->evaluate(table, row));
		case BNEG: 	return ~(lhs_->evaluate(table, row));
		case NOT:	return !(lhs_->evaluate(table, row));
		default: throw InvaludNumberOfOperandsException(op_to_str.at(op_));
		}
	}

	Cell BinaryExpression::evaluate(Table* table, const row_t& row)
	{
		switch (op_)
		{
		case ADD: 	return (lhs_->evaluate(table, row)) +  (rhs_->evaluate(table, row));
		case SUB: 	return (lhs_->evaluate(table, row)) -  (rhs_->evaluate(table, row));
		case MUL: 	return (lhs_->evaluate(table, row)) *  (rhs_->evaluate(table, row));
		case DIV: 	return (lhs_->evaluate(table, row)) /  (rhs_->evaluate(table, row));
		case MOD: 	return (lhs_->evaluate(table, row)) %  (rhs_->evaluate(table, row));
		case  OR: 	return (lhs_->evaluate(table, row)) || (rhs_->evaluate(table, row));
		case AND: 	return (lhs_->evaluate(table, row)) && (rhs_->evaluate(table, row));
		case XOR: 	return (lhs_->evaluate(table, row)) ^  (rhs_->evaluate(table, row));
		case BAND: 	return (lhs_->evaluate(table, row)) &  (rhs_->evaluate(table, row));
		case BOR: 	return (lhs_->evaluate(table, row)) |  (rhs_->evaluate(table, row));
		case  EQ: 	return (lhs_->evaluate(table, row)) == (rhs_->evaluate(table, row));
		case NEQ: 	return (lhs_->evaluate(table, row)) != (rhs_->evaluate(table, row));
		case  LE: 	return (lhs_->evaluate(table, row)) <  (rhs_->evaluate(table, row));
		case LEQ: 	return (lhs_->evaluate(table, row)) <= (rhs_->evaluate(table, row));
		case  GR: 	return (lhs_->evaluate(table, row)) >  (rhs_->evaluate(table, row));
		case GEQ: 	return (lhs_->evaluate(table, row)) >= (rhs_->evaluate(table, row));
		default: throw InvaludNumberOfOperandsException(op_to_str.at(op_));
		}
	}
	
} // namespace memdb