#ifndef HAXOR_FORMAT_HH
#define HAXOR_FORMAT_HH

#include <string>
#include <vector>
#include <map>

namespace haxor {
  enum struct fmt_token_type {
    integer,
    real,
    text,
    letter,
    pointer,
    free_text
  };

  struct fmt_token {
    std::string flags;
    std::string width;
    int64_t width_n;
    std::string precision;
    std::string length;
    std::string specifier;
    std::string free_text;
    fmt_token_type type;
  };

  std::vector<fmt_token> get_fmt_tokens(std::string fmt);
  std::string concat_fmt_token(const fmt_token item);
}

#endif
