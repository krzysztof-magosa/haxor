#include "haxor/hc/compiler.hh"

int main(int argc, char **argv) {
  haxor::compiler compiler;
  compiler.compile(argv[1]);

  return 0;
}
