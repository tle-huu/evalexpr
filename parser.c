#include "parser.h"

#include <stdlib.h>
#include <stdio.h>

static Expr const *addition(Token const * const *, int *);

void print_expr(Expr const * expr)
{
    if (expr == NULL) return;

    printf("%s ", expr->operator->lexeme);
    print_expr(expr->left );
    print_expr(expr->right);
}


void free_expr(Expr * expression)
{
    if (!expression) return;

    free_expr(expression->left);
    free_token(expression->operator);
    free_expr(expression->right);
    free(expression);
}

Expr const * create_expr(const Expr * left, Token const * operator, const Expr * right)
{
    Expr * expr = malloc(sizeof(const Expr));

    expr->left = left;
    expr->operator = operator;
    expr->right = right;

    return expr;
}


static Expr const * primary(Token const * const * tokens_list, int *index)
{
    Expr const * expr;

    if (tokens_list[*index]->type == NUMBER)
    {
        return create_expr(NULL, tokens_list[(*index)++], NULL);
    }

    if (tokens_list[*index]->type == LEFT_PAREN)
    {
        ++(*index);
        Expr const * expr = addition(tokens_list, index);
        if (expr == NULL) return NULL;
        if (tokens_list[*index]->type != RIGHT_PAREN)
        {
            printf("[PARSER] Expected closing parenthesis\n");
            return NULL;
        }
        ++(*index);
        return expr;
    }
    
    printf("[PARSER] Expected expression\n");
    return NULL;
}

static Expr const * unary(Token const * const * tokens_list, int *index)
{
    if (tokens_list[*index]->type == MINUS)
    {
        Token const * operator = tokens_list[*index];
        ++(*index);
        Expr const * right = unary(tokens_list, index);
        if (right == NULL) return NULL;

        return create_expr(NULL, operator, right);
    }
    return primary(tokens_list, index);
}

static Expr const * multiplication(Token const * const * tokens_list, int *index)
{
    Expr const * expr = unary(tokens_list, index);
    if (expr == NULL) return NULL;

    Expr const * tmp = expr;

    while (tokens_list[*index]->type == STAR || tokens_list[*index]->type == SLASH)
    {
        Token const * operator = tokens_list[*index];
        ++(*index); 
        Expr const * right = unary(tokens_list, index);
        if (right == NULL) return NULL;

        expr = create_expr(expr, operator, right);
       // free_expr(tmp);
        tmp = expr;
    }

    return expr;
}


static Expr const * addition(Token const * const * tokens_list, int *index)
{
    Expr const * expr = multiplication(tokens_list, index);
    if (expr == NULL) return NULL;
    Expr const * tmp = expr;
    while (tokens_list[*index]->type == PLUS || tokens_list[*index]->type == MINUS)
    {
        Token const * operator = tokens_list[*index];
        ++(*index); 
        Expr const * right = multiplication(tokens_list, index);

        if (right == NULL) return NULL;

        expr = create_expr(expr, operator, right);
     //   free_expr(tmp);
        tmp = expr;
    }
    return expr;
}


Expr const * parse(Token const * const * tokens_list)
{
    int index = 0;

    Expr const * expr = addition(tokens_list, &index);

    if (expr == NULL || tokens_list[index]->type != EOFF)
    {
        printf("[PARSER] Parsing Error\n");
        return NULL;
    }

    return expr;

}
