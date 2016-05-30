# @TODO this looks like hack for me, maybe there is better way.
include_directories("${CMAKE_CURRENT_BINARY_DIR}/../src")

# hc - Haxor Compiler
add_executable(
  hc
  hc.cc
)
target_link_libraries(hc haxor_common haxor_hc)
install(TARGETS hc
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

# hvm - Haxor Virtual Machine
add_executable(
  hvm
  hvm.cc
)
target_link_libraries(hvm haxor_common haxor_hvm)
install(TARGETS hvm
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
