%option nodefault
%option noyywrap
%option c++
%option yyclass="lexer"

%option prefix="haxor_"

%{
#include <iostream>
#include <cstdlib>
#include <inttypes.h>
#include "haxor/hc/lexer.hh"
#include "haxor/hc/compiler.hh"
#include "parser.hh"
#include "location.hh"

#define yyterminate() haxor::parser::make_END(haxor::location(location));
#define YY_USER_ACTION location.step(); location.columns(yyleng);
%}

%%

[ \t]

\$[a-zA-Z0-9]+              {
    return haxor::parser::make_REGISTER(yytext, haxor::location(location));
}

"section"                   {
    return haxor::parser::make_SECTION(haxor::location(location));
}

"dw"                        {
    return haxor::parser::make_DATA(sizeof(int64_t), haxor::location(location));
}

\.[a-zA-Z]+                 {
    return haxor::parser::make_SECTION_NAME(yytext, haxor::location(location));
}

[a-zA-Z][a-zA-Z_0-9]*:      {
    std::string temp(yytext);
    return haxor::parser::make_LABEL(temp.substr(0, temp.size() - 1), haxor::location(location));
}

[a-zA-z][a-zA-Z_0-9]*       {
    return haxor::parser::make_SYMBOL(yytext, haxor::location(location));
}

","                         {
    return haxor::parser::make_COMMA(haxor::location(location));
}

\"([^\\\"]|\\.)*\"          {
    std::string tmp(yytext);
    return haxor::parser::make_STRING(tmp.substr(1, tmp.size() - 2), haxor::location(location));
}

[0-9]+                      {
    return haxor::parser::make_INT(strtoimax(yytext, nullptr, 10), haxor::location(location));
}

[0-9]+h                     {
    word_t num = strtoimax(yytext, nullptr, 16); // strtoimax ignores suffix 'h'
    return haxor::parser::make_INT(num, haxor::location(location));
}

\n                          {
    location.lines();
    return haxor::parser::make_NEWLINE(haxor::location(location));
}

#.*                         {
}


<<EOF>>                     {
    return yyterminate();
}

.	                    {
    printf("bad input character '%s' at line %d\n", yytext, yylineno);
}
