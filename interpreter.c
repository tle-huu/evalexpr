#include "parser.h"

#include <stdio.h>


double interpret(Expr const * expr)
{
    if (expr == NULL)
    {
        return 0.0;
    }

    if (expr->operator->type == NUMBER)
    {
        return expr->operator->literal;
    }
    double left = interpret(expr->left);
    double right = interpret(expr->right);

    switch (expr->operator->type)
    {
        case PLUS:
            return left + right;
        case MINUS:
            return left - right;
        case STAR:
            return left * right;
        case SLASH:
            return left / right;
        default:
            printf("Error evaluating expression\n");
            return 0.0;
    }
    return 0.0;
}
