%skeleton "lalr1.cc"
%require "3.0"
%defines
%define parser_class_name { parser }

%define api.token.constructor
%define api.value.type variant
%define parse.assert
%define api.namespace { haxor }

%code requires
{
    #include <iostream>
    #include <string>
    #include <vector>
    #include <stdint.h>
    #include "haxor/hc/node/all.hh"

    namespace haxor {
        class lexer;
        class compiler;
    }
}

%code top
{
    #include <iostream>
    #include <sstream>
    #include "haxor/hc/lexer.hh"
    #include "parser.hh"
    #include "haxor/hc/compiler.hh"
    #include "location.hh"

    static haxor::parser::symbol_type yylex(haxor::lexer &lexer, haxor::compiler &driver) {
        return lexer.get_next_token();
    }

    using namespace haxor;
}

%lex-param { haxor::lexer &lexer }
%lex-param { haxor::compiler &compiler }
%parse-param { haxor::lexer &lexer }
%parse-param { haxor::compiler &compiler }
%locations
%define parse.trace
%define parse.error verbose

%define api.token.prefix {T_}

%token END 0 "end of file"
%token <std::string> SYMBOL
%token SECTION
%token <std::string> SECTION_NAME
%token <std::string> LABEL
%token <std::string> REGISTER
%token COMMA
%token <std::string> STRING
%token <word_t> INT
%token <word_t> DATA;
%token NEWLINE

%type <std::vector<node::base*>*> program
%type <std::vector<node::base*>*> line_list
%type <std::vector<node::base*>*> line
%type <node::base*> label
%type <std::vector<node::base*>*> instr_arg_list
%type <node::base*> instr_arg
%type <std::vector<word_t>*> data_list
%type <std::vector<word_t>*> data

%start program

%%

program:        line_list
                {
                    $$ = $1;
                    compiler.set_ast($1);
                }
        ;

line_list:      line_list line NEWLINE
                {
                    $$ = $1;
                    $$->insert($$->end(), $2->begin(), $2->end());
                }
        |       line
                {
                    $$ = new std::vector<node::base*>();
                    $$->insert($$->end(), $1->begin(), $1->end());
                }
        ;

line:           label
                {
                    $$ = new std::vector<node::base*>();
                    if ($1 != nullptr) {
                        $$->push_back($1);
                    }
                }
        |       label SYMBOL instr_arg_list
                {
                    $$ = new std::vector<node::base*>();
                    if ($1 != nullptr) {
                        $$->push_back($1);
                    }

                    auto *node = new node::instr($2, $3);
                    node->set_location(@$);
                    $$->push_back(node);
                }
        |       label DATA data_list
                {
                    $$ = new std::vector<node::base*>();

                    if ($1 != nullptr) {
                        $$->push_back($1);
                    }

                    for (auto item : *$3) {
                        $$->push_back(new node::num(item, $2));
                    }
                }
        |       label DATA
                {
                    $$ = new std::vector<node::base*>();

                    if ($1 != nullptr) {
                        $$->push_back($1);
                    }

                    $$->push_back(new node::num(0, $2));
                }
        |       SECTION SECTION_NAME
                {
                    $$ = new std::vector<node::base*>();
                    $$->push_back(new node::section($2));
                }
        ;

label:
                {
                    $$ = nullptr;
                }
        |       LABEL
                {
                    $$ = new node::label($1);
                }
        ;

instr_arg_list: instr_arg_list COMMA instr_arg
                {
                    $$ = $1;
                    $1->push_back($3);
                }
        |       instr_arg
                {
                    $$ = new std::vector<node::base*>();
                    $$->push_back($1);
                }
        |
                {
                    $$ = new std::vector<node::base*>();
                }
        ;

instr_arg:      REGISTER
                {
                    $$ = new node::reg($1);
                    $$->set_location(@1);
                }
        |       SYMBOL
                {
                    $$ = new node::label($1);
                    $$->set_location(@1);
                }
        |       INT
                {
                    $$ = new node::num($1);
                    $$->set_location(@1);
                }
        ;

data_list:      data_list COMMA data
                {
                    $$ = $1;
                    $$->insert($$->end(), $3->begin(), $3->end());
                }
        |       data
                {
                    $$ = new std::vector<word_t>();
                    $$->insert($$->end(), $1->begin(), $1->end());
                }
        ;

data:           INT
                {
                    $$ = new std::vector<word_t>();
                    $$->push_back($1);
                }
        |       STRING
                {
                    $$ = new std::vector<word_t>();
                    for (size_t i = 0; i < $1.size(); i++) {
                        $$->push_back($1[i]);
                    }
                }
        ;

%%

void haxor::parser::error(const location &loc , const std::string &message) {
    std::stringstream ss;
    ss << loc;

    const std::string error = message
        + " at "
        + ss.str();

    throw haxor::hc_syntax_error(error);
}
