#include "interpreter.h"
#include "scanner.h"
#include "parser.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEBUG 0

int main(int argc, char ** argv)
{
    char *expression = "( 4 * ( 1 + 2)   )";

    if (argc > 1)
    {
        expression = argv[1];
    }
    Token const * const * tokens_list = scan_tokens(expression);

    if (tokens_list == NULL)
    {
        printf("An error occured\n");
        return 2;
    }

    int index = 0;

#if DEBUG
    printf("Tokens: ");
    while (tokens_list[index]->type != EOFF)
    {
        printf("%s ", tokens_list[index]->lexeme);
        index++;
    }
    printf("\n");
#endif
    Expr const * expr = parse(tokens_list);
    
    if (expr == NULL)
    {
        return 2;
    }

#if DEBUG
    print_expr(expr);
    printf("\n");
#endif

    double result = interpret(expr);

    while (tokens_list[index]->type != EOFF)
    {
        printf("%s ", tokens_list[index]->lexeme);
        index++;
    }
    printf("= %f\n", result);

    return 0;
}
