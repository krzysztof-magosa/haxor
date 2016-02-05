#include <iostream>
#include <stdexcept>
#include "haxor/common/haxor.hh"
#include "haxor/hvm/vm.hh"
#include "haxor/common/hdr.hh"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: hvm <program-to-execute.hax.e>" << std::endl;
    return 1;
  }

  //  try {
    haxor::vm vm;
    vm.load_exec(argv[1]);
    return vm.run();
    //  }
  //  catch (const std::exception &e) {
    //    std::cerr << "An error occurred: " << e.what() << std::endl;
    //return 2;
    //}
}
