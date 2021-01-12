#include <memory>
#include <string>
#include <iostream>

#include "Common.h"

using namespace::std;

enum class Operations {
    SUM,
    PRODUCT,
    NONE
};

class Exp : public Expression{
    int m_val;
    Operations m_operation = Operations::NONE;
    ExpressionPtr m_left, m_right;
public:
    Exp(int val) : m_val(val) {
        //
    }
    Exp(ExpressionPtr left, ExpressionPtr right, Operations operation) :
            m_operation(operation),
            m_left(move(left)),
            m_right(move(right))
    {
        //
    }
    // Вычисляет значение выражения
    virtual int Evaluate() const {
        if (m_operation == Operations::NONE) {
            return m_val;
        }
        if (m_operation == Operations::SUM) {
            return m_left->Evaluate() + m_right->Evaluate(); 
        }
        return m_left->Evaluate() * m_right->Evaluate();
    }

    // Форматирует выражение как строку
    // Каждый узел берётся в скобки, независимо от приоритета
    virtual std::string ToString() const {
        if (m_operation == Operations::NONE) {
            return to_string(m_val);
        }
        char sign = m_operation == Operations::SUM ? '+' : '*';

        return "(" + m_left->ToString() + ")" + sign + "("  + m_right->ToString() + ")";
    }
};

ExpressionPtr Value(int value) {
    return make_unique<Exp> (value);
}

ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
    return make_unique<Exp> (move(left), move(right), Operations::SUM);
}

ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right) {
    return make_unique<Exp> (move(left), move(right), Operations::PRODUCT);
}