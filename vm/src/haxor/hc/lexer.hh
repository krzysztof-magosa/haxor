#ifndef HAXOR_HC_LEXER_HH
#define HAXOR_HC_LEXER_HH

#if ! defined(yyFlexLexerOnce)
#undef yyFlexLexer
#define yyFlexLexer haxor_FlexLexer
#include <FlexLexer.h>
#endif

#undef YY_DECL
#define YY_DECL haxor::parser::symbol_type haxor::lexer::get_next_token()

#include "parser.hh"

namespace haxor {
  class lexer : public yyFlexLexer {
    public:
    lexer(compiler &compiler) : compiler(compiler) {}
    virtual ~lexer() {}
    virtual haxor::parser::symbol_type get_next_token();

    private:
    compiler &compiler;
  };
}

#endif
