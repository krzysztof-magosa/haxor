%option nodefault
%option noyywrap
%option c++
%option yyclass="lexer"

%option prefix="haxor_"

%{
#include <iostream>
#include <cstdlib>
#include "haxor/hc/lexer.hh"
#include "haxor/hc/compiler.hh"
#include "parser.hh"
#include "location.hh"

#define yyterminate() haxor::parser::make_END(haxor::location());
%}

%%

[ \t]

\$[a-zA-Z0-9]+              {
    return haxor::parser::make_REGISTER(yytext, haxor::location());
}

"section"                   {
    return haxor::parser::make_SECTION(haxor::location());
}

"dw"                        {
    return haxor::parser::make_DATA(sizeof(int64_t), haxor::location());
}

\.[a-zA-Z]+                 {
    return haxor::parser::make_SECTION_NAME(yytext, haxor::location());
}

[a-zA-Z][a-zA-Z_0-9]*:      {
    std::string temp(yytext);
    return haxor::parser::make_LABEL(temp.substr(0, temp.size() - 1), haxor::location());
}

[a-zA-z][a-zA-Z_0-9]*       {
    return haxor::parser::make_SYMBOL(yytext, haxor::location());
}

","                         {
    return haxor::parser::make_COMMA(haxor::location());
}

\"([^\\\"]|\\.)*\"          {
    std::string tmp(yytext);
    return haxor::parser::make_STRING(tmp.substr(1, tmp.size() - 2), haxor::location());
}

[0-9]+                      {
    return haxor::parser::make_INT(std::stoi(yytext), haxor::location());
}

[0-9]+h                     {
    std::string tmp(yytext);
    int64_t num = std::stoi(tmp.substr(0, tmp.size() - 1), nullptr, 16);
    return haxor::parser::make_INT(num, haxor::location());
}

\n                          {
    return haxor::parser::make_NEWLINE(haxor::location());
}


<<EOF>>                     {
    return yyterminate();
}

.	                    {
    printf("bad input character '%s' at line %d\n", yytext, yylineno);
}
