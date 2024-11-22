#ifndef HEADER_GUARD_EXPRESSION_EXPRESSION_H
#define HEADER_GUARD_EXPRESSION_EXPRESSION_H

#include <string>
#include <memory>
#include <unordered_map>

#include "cell/cell.hpp"
#include "database/table.hpp"

namespace memdb
{
    enum Operation
    {
        ADD, SUB, MUL, DIV, MOD, NEG,   // arithmetic  
        OR, AND, NOT,                   // logic
        XOR, BAND, BOR, BNEG,           // bitwise
        EQ, NEQ, LE, LEQ, GR, GEQ      // compare   
    };

    // Abstract class for expression tree node
    class Expression
    {
    public:
        Expression() = default;
        virtual ~Expression() = default;
        virtual Cell evaluate(Row* row) = 0;
    };

    // Leave of expression tree
    class ValueExpression : public Expression
    {
    public:
        ValueExpression(const std::string& column_name);
        ~ValueExpression() override = default;

        Cell evaluate(Row* row) override;
    private:
        std::string column_name_;
    };

    // Node of expression tree with one child
    class UnaryExpression : public Expression
    {
    public:
        UnaryExpression(std::unique_ptr<Expression> lhs, Operation op);
        ~UnaryExpression() override = default;

        Cell evaluate(Row* row) override;
    private:
        std::unique_ptr<Expression> lhs_;
        Operation op_;
    };

    // Node of expression tree with two children
    class BinaryExpression : public Expression
    {
    public:
        BinaryExpression(std::unique_ptr<Expression> lhs, 
            std::unique_ptr<Expression> rhs, Operation op);
        ~BinaryExpression() override = default;

        Cell evaluate(Row* row) override;
    private:
        std::unique_ptr<Expression> lhs_;
        std::unique_ptr<Expression> rhs_;
        Operation op_;
    };

} // namespace memdb

#endif // HEADER_GUARD_EXPRESSION_EXPRESSION_H