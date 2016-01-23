file(GLOB_RECURSE hvm_sources haxor/hvm/*.cc)

add_executable(hvm ${hvm_sources})

install(TARGETS hvm
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
