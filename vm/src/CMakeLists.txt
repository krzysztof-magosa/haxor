file(GLOB_RECURSE haxor_sources *.cc)

add_executable(hvm ${haxor_sources})

install(TARGETS hvm
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
