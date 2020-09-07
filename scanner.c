#include "scanner.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static const int DEFAULT_TOKEN_LIST_SIZE = 1024;

Token const* create_token(enum TokenType type, char const *lexeme, double literal)
{

    if (lexeme == NULL)
    {
        printf("[SCANNER] Error creating token: lexeme null\n");
        return NULL;
    }

    Token* token = malloc(sizeof(Token));
    token->lexeme = malloc(sizeof(char) * (1 + strlen(lexeme)));
    strcpy(token->lexeme, lexeme);
    token->literal = literal;
    token->type = type;

    return token;
}


void free_token(Token const * token)
{
    if (token)
    {
        free(token->lexeme);
    }
    free((Token *)token);
}

void add_token(Token const ** tokens_list, int *list_index, enum TokenType type, char const * lexeme, double literal)
{
    tokens_list[*list_index] = create_token(type, lexeme, literal);

    ++(*list_index);
}

Token const ** resize_tokens_list(Token const ** tokens_list, int size)
{
    Token const ** new_tokens_list = malloc(sizeof(Token*) * 2 * size);
    memcpy(new_tokens_list, tokens_list, size * sizeof(Token*));
    free(tokens_list);

    return new_tokens_list;

}

void handle_number(Token const ** tokens_list,  int *index, int *list_index, char const * source)
{
    char number_buffer[1024];
    memset(number_buffer, 0, 1024);

    --(*index);
    int start_number = *index;

    while (isdigit(source[*index]))
    {
        ++(*index);
    }
    
    if (source[*index] == '.' && isdigit(source[*index + 1]))
    {
        ++(*index);
        while (isdigit(source[*index]))
        {
            ++(*index);
        }
    }

    double literal = atof(source + start_number);
    strncpy(number_buffer, source + start_number, *index - start_number);
    add_token(tokens_list, list_index, NUMBER, number_buffer, literal);  
}

Token const * const * scan_tokens(char const * source)
{
    Token const ** tokens_list = (Token const **)malloc(sizeof(Token*) * DEFAULT_TOKEN_LIST_SIZE);
    int tokens_list_size = DEFAULT_TOKEN_LIST_SIZE;
    int list_index = 0;
    int index = 0;

    int error = 0;

    while (source[index] != 0)
    {
        if (list_index >= tokens_list_size)
        {
            tokens_list = resize_tokens_list(tokens_list, tokens_list_size);
            tokens_list_size *= 2;
        }
        
        char current_char = source[index];
        ++index;

        switch (current_char)
        {
            case '(': add_token(tokens_list, &list_index, LEFT_PAREN, "(", 0.0); break;
            case ')': add_token(tokens_list, &list_index, RIGHT_PAREN, ")", 0.0); break;
            case '+': add_token(tokens_list, &list_index, PLUS, "+", 0.0); break;
            case '-': add_token(tokens_list, &list_index, MINUS, "-", 0.0); break;
            case '/': add_token(tokens_list, &list_index, SLASH, "/", 0.0); break;
            case '*': add_token(tokens_list, &list_index, STAR, "*", 0.0); break;

            case ' ':   break;
            case '\n':  break;
            case '\r':  break;
            case '\t':  break;

            default:
                if (isdigit(current_char))
                {
                    handle_number(tokens_list, &index, &list_index, source);
                }
                else
                {
                    printf("[SCANNER] Unexpected character at index [%d] --> [%c]\n", index - 1, current_char);
                    error = 1;
                }

                break;

        }
    }
    
    if (error)
    {
        return NULL;
    }
    add_token(tokens_list, &list_index, EOFF, "EOF", 0.0);

    return tokens_list;
    
}

