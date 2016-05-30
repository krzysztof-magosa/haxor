file(GLOB_RECURSE common_sources haxor/common/*.cc)
add_library(
  haxor_common
  ${common_sources}
)

#

file(GLOB_RECURSE hvm_sources haxor/hvm/*.cc)

add_library(
  haxor_hvm
  ${hvm_sources}
)

target_link_libraries(haxor_hvm haxor_common)

install(TARGETS haxor_hvm
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

#

find_package(BISON)
find_package(FLEX)

bison_target(parser haxor/hc/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cc)
flex_target(lexer haxor/hc/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
file(GLOB_RECURSE hc_sources haxor/hc/*.cc)

add_library(
  haxor_hc
  ${BISON_parser_OUTPUTS}
  ${FLEX_lexer_OUTPUTS}
  ${hc_sources}
)

target_link_libraries(haxor_hc haxor_common)

install(TARGETS haxor_hc
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
