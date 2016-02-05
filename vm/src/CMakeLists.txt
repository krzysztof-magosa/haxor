file(GLOB_RECURSE common_sources haxor/common/*.cc)

#

file(GLOB_RECURSE hvm_sources haxor/hvm/*.cc)

add_executable(
  hvm
  ${hvm_sources}
  ${common_sources}
)

install(TARGETS hvm
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

add_executable(
  hc
  ${BISON_parser_OUTPUTS}
  ${FLEX_lexer_OUTPUTS}
  ${hc_sources}
  ${common_sources}
)

install(TARGETS hc
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
