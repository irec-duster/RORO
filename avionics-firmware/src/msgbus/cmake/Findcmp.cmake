
set(CMP_SOURCES ${CMAKE_CURRENT_LIST_DIR}/../lib/cmp/cmp.c)
set(CMP_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/../lib)


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(cmp  DEFAULT_MSG
                                  CMP_SOURCES CMP_INCLUDE_DIR)

