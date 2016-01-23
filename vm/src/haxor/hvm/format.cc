#include "haxor/hvm/haxor.hh"
#include "haxor/hvm/format.hh"
#include <string>
#include <vector>
#include <regex>
#include <stdexcept>
#include <iostream>

namespace haxor {
  static std::map<std::string, fmt_token_type> specifier_types = {
    {"d", fmt_token_type::integer},
    {"i", fmt_token_type::integer},
    {"u", fmt_token_type::integer},
    {"o", fmt_token_type::integer},
    {"x", fmt_token_type::integer},
    {"X", fmt_token_type::integer},
    {"n", fmt_token_type::integer}, // n ! http://www.cplusplus.com/reference/cstdio/fprintf/

    {"f", fmt_token_type::real},
    {"F", fmt_token_type::real},
    {"e", fmt_token_type::real},
    {"E", fmt_token_type::real},
    {"g", fmt_token_type::real},
    {"G", fmt_token_type::real},
    {"a", fmt_token_type::real},
    {"A", fmt_token_type::real},

    {"s", fmt_token_type::text},

    {"c", fmt_token_type::letter},

    {"p", fmt_token_type::pointer}
  };

  std::vector<fmt_token> get_fmt_tokens(std::string fmt) {
    std::regex fmtr("%(\\-|\\+|\\#|0|\\ )*(\\d+|\\*){0,1}(\\.\\d+|\\.\\*){0,1}(hh|ll|h|l|j|z|t|L){0,1}(d|i|u|o|x|X|f|F|e|E|g|G|a|A|c|s|p|n|%){1}");

    std::vector<fmt_token> result;

    while (!fmt.empty()) {
      std::smatch m;
      if (std::regex_search(fmt, m, fmtr)) {
        if (m.position(0) > 0) {
          // everything before found pattern must be free text
          fmt_token ft;
          ft.free_text = fmt.substr(0, m.position(0));
          ft.type = fmt_token_type::free_text;
          result.push_back(ft);
        }

        auto type = specifier_types.find(m[5]);
        if (type == specifier_types.end()) {
          throw std::invalid_argument("Bad format token.");
        }

        fmt_token t;
        t.flags     = m[1];
        t.width     = m[2];
        if (t.width.empty()) {
          t.width_n = -1;
        } else {
          t.width_n   = std::stoi(m[2]);
        }
        t.precision = m[3];
        t.length    = m[4];
        t.specifier = m[5];
        t.type      = type->second;
        result.push_back(t);

        fmt = fmt.substr(m.position(0) + m.length(0), fmt.size() - m.position(0) + m.length(0));
      } else {
        fmt_token t;
        t.type = fmt_token_type::free_text;
        t.free_text = fmt;
        result.push_back(t);
        break;
      }
    }

    return result;
  }

  std::string concat_fmt_token(const fmt_token item) {
    return "%" + item.flags + item.width + item.precision + item.length + item.specifier;
  }
}
