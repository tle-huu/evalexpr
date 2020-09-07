#ifndef __SCANNER_H__

#define __SCANNER_H__

enum TokenType
{

    LEFT_PAREN,
    RIGHT_PAREN,

    PLUS,
    MINUS,
    SLASH,
    STAR,
    PERCENT,

    NUMBER,
    EOFF,

};


typedef struct Token
{
    enum TokenType type;
    char *lexeme;
    double literal;
} Token;


Token const * create_token(enum TokenType type, char const * lexeme, double literal);
void free_token(Token const * token);


Token const* const * scan_tokens(char const * source);


#endif


