#include "haxor/hc/compiler.hh"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: hc <source-to-compile.hax>" << std::endl;
    return 1;
  }

  try {
    haxor::compiler compiler;
    compiler.compile(argv[1]);
  }
  catch (haxor::hc_syntax_error &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  catch (haxor::hc_missing_entry_point &e) {
    std::cerr << e.what() << std::endl;
    return 2;
  }

  return 0;
}
