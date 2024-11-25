#include "expression/expression.hpp"

namespace memdb
{

    Expression::Expression(ExpressionNodePointer root)
    : root_(root)
    { }

    Cell Expression::evaluate(Row* row) const
    {
        if (!row) return false;
        if (!root_) return true;
        return root_->evaluate(row);
    }

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

    UnaryExpression::UnaryExpression(ExpressionNodePointer lhs, Operation op) :
        lhs_(std::move(lhs)), op_(op)
    { }

    BinaryExpression::BinaryExpression(ExpressionNodePointer lhs, 
            ExpressionNodePointer rhs, Operation op) :
        lhs_(std::move(lhs)), rhs_(std::move(rhs)), op_(op)
    { }

    Cell ValueExpression::evaluate(Row* row)
    {
        Table* table = row->get_table();
        return (*row)[table->column_position(column_name_)];
    }

    Cell UnaryExpression::evaluate(Row* row)
    {
        switch (op_)
        {
        case NEG:   return -(lhs_->evaluate(row));
        case BNEG:  return ~(lhs_->evaluate(row));
        case NOT:   return !(lhs_->evaluate(row));
        default: throw InvaludNumberOfOperandsException(op_to_str.at(op_));
        }
    }

    Cell BinaryExpression::evaluate(Row* row)
    {
        switch (op_)
        {
        case ADD:   return (lhs_->evaluate(row)) +  (rhs_->evaluate(row));
        case SUB:   return (lhs_->evaluate(row)) -  (rhs_->evaluate(row));
        case MUL:   return (lhs_->evaluate(row)) *  (rhs_->evaluate(row));
        case DIV:   return (lhs_->evaluate(row)) /  (rhs_->evaluate(row));
        case MOD:   return (lhs_->evaluate(row)) %  (rhs_->evaluate(row));
        case  OR:   return (lhs_->evaluate(row)) || (rhs_->evaluate(row));
        case AND:   return (lhs_->evaluate(row)) && (rhs_->evaluate(row));
        case XOR:   return (lhs_->evaluate(row)) ^  (rhs_->evaluate(row));
        case BAND:  return (lhs_->evaluate(row)) &  (rhs_->evaluate(row));
        case BOR:   return (lhs_->evaluate(row)) |  (rhs_->evaluate(row));
        case  EQ:   return (lhs_->evaluate(row)) == (rhs_->evaluate(row));
        case NEQ:   return (lhs_->evaluate(row)) != (rhs_->evaluate(row));
        case  LE:   return (lhs_->evaluate(row)) <  (rhs_->evaluate(row));
        case LEQ:   return (lhs_->evaluate(row)) <= (rhs_->evaluate(row));
        case  GR:   return (lhs_->evaluate(row)) >  (rhs_->evaluate(row));
        case GEQ:   return (lhs_->evaluate(row)) >= (rhs_->evaluate(row));
        default: throw InvaludNumberOfOperandsException(op_to_str.at(op_));
        }
    }
    
} // namespace memdb