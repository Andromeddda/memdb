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
        // Expressions are not default constructible or copyalbe
        Expression()                                    = delete;
        Expression(const Expression& other)             = delete;
        Expression& operator=(const Expression& other)  = delete;

        // Expressions are movable
        Expression(Expression&& other)             = default;
        Expression& operator=(Expression&& other)  = default;

        Cell evaluate(Row* row) const;
        ~Expression() = default;
    private:
        friend class Parser;
        Expression(std::unique_ptr<ExpressionNode> root);

        std::unique_ptr<ExpressionNode> root_;
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

    // Node of ExpressionNode tree with one child
    class UnaryExpression : public ExpressionNode
    {
    public:
        UnaryExpression(std::unique_ptr<ExpressionNode> lhs, Operation op);
        ~UnaryExpression() override = default;

        Cell evaluate(Row* row) override;
    private:
        std::unique_ptr<ExpressionNode> lhs_;
        Operation op_;
    };

    // Node of ExpressionNode tree with two children
    class BinaryExpression : public ExpressionNode
    {
    public:
        BinaryExpression(std::unique_ptr<ExpressionNode> lhs, 
            std::unique_ptr<ExpressionNode> rhs, Operation op);
        ~BinaryExpression() override = default;

        Cell evaluate(Row* row) override;
    private:
        std::unique_ptr<ExpressionNode> lhs_;
        std::unique_ptr<ExpressionNode> rhs_;
        Operation op_;
    };

} // namespace memdb

#endif // HEADER_GUARD_EXPRESSION_EXPRESSION_H