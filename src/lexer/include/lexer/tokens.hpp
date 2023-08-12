#ifndef STACKLANG_TOKENS_HPP
#define STACKLANG_TOKENS_HPP

namespace stacklang::lexer
{
#define TOKEN_LIST_KEYWORD \
    CREATE_TOKEN_KEYWORD(PUSH, "push") \
    CREATE_TOKEN_KEYWORD(POP, "pop") \
    CREATE_TOKEN_KEYWORD(JI, "ji") \
    CREATE_TOKEN_KEYWORD(JMP, "jmp") \
    CREATE_TOKEN_KEYWORD(WRITE, "write") \
    CREATE_TOKEN_KEYWORD(READ, "read") \
    CREATE_TOKEN_KEYWORD(END, "end")

#define TOKEN_LIST \
    CREATE_TOKEN(VAR, "variable") \
    CREATE_TOKEN(CONST, "constant") \
    CREATE_TOKEN(ERROR, "error") \
    CREATE_TOKEN(NEWLINE, "newline") \
    CREATE_TOKEN(END_OF_FILE, "EOF") \
    CREATE_TOKEN(PLUS, "+") \
    CREATE_TOKEN(MINUS, "-") \
    CREATE_TOKEN(MULTIPLY, "*") \
    CREATE_TOKEN(DIV, "/") \
    CREATE_TOKEN(MOD, "%") \
    CREATE_TOKEN(LESS, "<") \
    CREATE_TOKEN(GREATER, ">") \
    CREATE_TOKEN(EQUAL, "=") \
    CREATE_TOKEN(LESS_EQUAL, "<=") \
    CREATE_TOKEN(GREATER_EQUAL, ">=") \
    CREATE_TOKEN(NOT_EQUAL, "!=") \
    CREATE_TOKEN(NOT, "!")
}

#endif //STACKLANG_TOKENS_HPP
