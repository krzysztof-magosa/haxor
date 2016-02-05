#include "haxor/hc/compiler.hh"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: hc <source-to-compile.hax>" << std::endl;
    return 1;
  }

  haxor::compiler compiler;
  compiler.compile(argv[1]);

  return 0;
}
