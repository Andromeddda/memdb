#ifndef HEADER_GUARD_EXPRESSION_EXPRESSION_H
#define HEADER_GUARD_EXPRESSION_EXPRESSION_H

#include <string>
#include <memory>
#include <unordered_map>

#include "cell/cell.hpp"
#include "parser/parser.hpp"
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

    // Abstract class for ExpressionNode tree node
    class ExpressionNode
    {
    public:
        ExpressionNode() = default;
        virtual ~ExpressionNode() = default;
        virtual Cell evaluate(Row* row) = 0;
    };

    class Expression
    {
    public:
        // Expressions are not default constructible
        Expression() = default;

        Expression(const Expression& other) = default;
        Expression(Expression&& other)      = default;

        Expression& operator=(const Expression& other)  = default;
        Expression& operator=(Expression&& other)       = default;

        Cell evaluate(Row* row) const;
        ~Expression() = default;
    private:
        friend class Parser;
        Expression(ExpressionNodePointer root);

        ExpressionNodePointer root_;
    };

    // Leave of ExpressionNode tree
    class ValueExpression : public ExpressionNode
    {
    public:
        ValueExpression(const std::string& column_name);
        ~ValueExpression() override = default;

        Cell evaluate(Row* row) override;
    private:
        std::string column_name_;
    };

    class ConstExpression : public ExpressionNode
    {
    public:
        ConstExpression(const Cell& data);
        ~ConstExpression() override = default;

        Cell evaluate(Row* row) override;
    private:
        Cell data_;
    };

    // Node of ExpressionNode tree with one child
    class UnaryExpression : public ExpressionNode
    {
    public:
        UnaryExpression(ExpressionNodePointer lhs, Operation op);
        ~UnaryExpression() override = default;

        Cell evaluate(Row* row) override;
    private:
        ExpressionNodePointer lhs_;
        Operation op_;
    };

    // Node of ExpressionNode tree with two children
    class BinaryExpression : public ExpressionNode
    {
    public:
        BinaryExpression(ExpressionNodePointer lhs, 
            ExpressionNodePointer rhs, Operation op);
        ~BinaryExpression() override = default;

        Cell evaluate(Row* row) override;
    private:
        ExpressionNodePointer lhs_;
        ExpressionNodePointer rhs_;
        Operation op_;
    };

} // namespace memdb

#endif // HEADER_GUARD_EXPRESSION_EXPRESSION_H