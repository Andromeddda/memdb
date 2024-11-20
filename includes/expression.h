#ifndef HEADER_GUARD_EXPRESSION_H
#define HEADER_GUARD_EXPRESSION_H

#include <string>
#include <memory>

#include "cell.h"
#include "database.h"

namespace memdb
{
    enum Operation
    {
        ADD, SUB, MUL, DIV, MOD, NEG,   // arithmetic  
        OR, AND, NOT,                   // logic
        XOR, BAND, BOR, BNEG,           // bitwise
        EQ, NEQ, LE, LEQ, GR, GEQ       // compare
    };

    class Expression
    {
    public:
        Expression() = default;
        virtual ~Expression() = default;
        virtual Cell evaluate(Database* database) = 0;
    };

    class ValueExpression : public Expression
    {
    public:
        ValueExpression(const std::string& table_name, 
            const std::string& column_name);
        ~ValueExpression() override = default;

        Cell evaluate(Database* database) override;
    private:
        std::string table_name_;
        std::string column_name_;
    };

    class UnaryExpression : public Expression
    {
    public:
        UnaryExpression(std::unique_ptr<Expression> lhs, Operation op);
        ~UnaryExpression() override = default;

        Cell evaluate(Database* database) override;
    private:
        std::unique_ptr<Expression> lhs_;
        Operation op_;
    };

    class BinaryExpression : public Expression
    {
    public:
        BinaryExpression(std::unique_ptr<Expression> lhs, 
            std::unique_ptr<Expression> rhs, Operation op);
        ~BinaryExpression() override = default;

        Cell evaluate(Database* database) override;
    private:
        std::unique_ptr<Expression> lhs_;
        std::unique_ptr<Expression> rhs_;
        Operation op_;
    };

} // namespace memdb

#endif // HEADER_GUARD_EXPRESSION_H