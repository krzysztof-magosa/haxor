#ifndef HAXOR_HC_NODE_SECTION_HH
#define HAXOR_HC_NODE_SECTION_HH

#include "haxor/hc/node/base.hh"

namespace haxor {
  namespace node {
    class section : public base {
      public:
      section(const std::string &name);
      enum type get_type() const;
      std::string get_name() const;

      private:
      const std::string name;
    };
  }
}

#endif
