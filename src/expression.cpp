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
	
	  // UnaryExpression  Expression::operator- () const { return UnaryExpression(*this, NEG); }
      // UnaryExpression  Expression::operator! () const { return UnaryExpression(*this, NOT); }
      // UnaryExpression  Expression::operator~ () const { return UnaryExpression(*this, BNEG);}
      // BinaryExpression Expression::operator== (const Expression& other) const { return BinaryExpression(*this, other, EQ);  }
      // BinaryExpression Expression::operator!= (const Expression& other) const { return BinaryExpression(*this, other, NEQ); }
      // BinaryExpression Expression::operator>= (const Expression& other) const { return BinaryExpression(*this, other, LEQ); }
      // BinaryExpression Expression::operator<= (const Expression& other) const { return BinaryExpression(*this, other, GEQ); }
      // BinaryExpression Expression::operator<  (const Expression& other) const { return BinaryExpression(*this, other, LE);  }
      // BinaryExpression Expression::operator>  (const Expression& other) const { return BinaryExpression(*this, other, GR);  }
      // BinaryExpression Expression::operator-  (const Expression& other) const { return BinaryExpression(*this, other, SUB); }
      // BinaryExpression Expression::operator/  (const Expression& other) const { return BinaryExpression(*this, other, DIV); }
      // BinaryExpression Expression::operator%  (const Expression& other) const { return BinaryExpression(*this, other, MOD); }
      // BinaryExpression Expression::operator*  (const Expression& other) const { return BinaryExpression(*this, other, MUL); }
      // BinaryExpression Expression::operator+  (const Expression& other) const { return BinaryExpression(*this, other, ADD); }
      // BinaryExpression Expression::operator&& (const Expression& other) const { return BinaryExpression(*this, other, AND); }
      // BinaryExpression Expression::operator|| (const Expression& other) const { return BinaryExpression(*this, other, OR);  }
      // BinaryExpression Expression::operator|  (const Expression& other) const { return BinaryExpression(*this, other, BAND);}
      // BinaryExpression Expression::operator&  (const Expression& other) const { return BinaryExpression(*this, other, BOR); }
      // BinaryExpression Expression::operator^  (const Expression& other) const { return BinaryExpression(*this, other, XOR); }

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