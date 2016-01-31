#include "haxor/hc/lexer.hh"

namespace haxor {
  lexer::lexer(class compiler &compiler) : compiler(compiler) {
  }

  void lexer::set_stream(std::istream &stream) {
    yyin = &stream;
  }
}
