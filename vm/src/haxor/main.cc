#include <iostream>
#include "haxor/haxor.hh"
#include "haxor/vm.hh"
#include "haxor/hdr.hh"

#include <fstream>

int main(int argc, char **argv) {
  haxor::vm vm;
  vm.load_exec(argv[1]);
  return vm.run();
}
