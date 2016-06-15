#ifndef HAXOR_HC_NODE_BASE_HH
#define HAXOR_HC_NODE_BASE_HH

#include <string>
#include <vector>
#include "location.hh"
#include "haxor/common/haxor.hh"
#include "haxor/hvm/cpu.hh"

namespace haxor {
  enum class node_type {
    program,
    instr,
    num,
    reg,
    label,
    section
  };

  std::string type_to_string(node_type nodeType);

  class node_base {
    public:
    virtual ~node_base() {};
    virtual node_type get_type() const = 0;
    bool is_a(node_type type);
    word_t get_size() const;
    word_t get_addr() const;
    void set_addr(const word_t addr);
    void set_section(const std::string &section);
    std::string get_section() const;
    void set_location(const class location &location);
    bool has_location();
    class location& get_location();

    virtual node_base* clone() {
      throw std::logic_error("Not implemented.");
    }

    protected:
    word_t size = 0;
    word_t addr;
    std::string section;
    class location *location;
  };
}

#endif
