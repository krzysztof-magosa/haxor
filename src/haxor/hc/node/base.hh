#ifndef HAXOR_HC_NODE_BASE_HH
#define HAXOR_HC_NODE_BASE_HH

#include <string>
#include <vector>
#include "haxor/common/haxor.hh"
#include "haxor/hvm/cpu.hh"

namespace haxor {
  namespace node {
    enum class type {
      program,
      instr,
      num,
      reg,
      label,
      section
    };

    class base {
      public:
      virtual ~base() {};
      virtual type get_type() const = 0;
      bool is_a(type type);
      word_t get_size() const;
      word_t get_addr() const;
      void set_addr(const word_t addr);
      void set_section(const std::string &section);
      std::string get_section() const;

      protected:
      word_t size = 0;
      word_t addr;
      std::string section;
    };
  }
}

#endif
