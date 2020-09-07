#ifndef __PARSER_H__

#define __PARSER_H__

#include "scanner.h"

typedef struct Expr
{
    struct Expr const   * left;
    Token const * operator;
    struct Expr const * right;
} Expr;


void free_expr(Expr  * expression);

Expr const * create_expr(Expr const * left, Token const * operator, Expr const * right);

Expr const * parse(Token const * const * tokens_list);

void print_expr(Expr const * expr);

#endif
