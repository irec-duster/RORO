
set(CMP_MEM_ACCESS_SOURCES ${CMAKE_CURRENT_LIST_DIR}/../lib/cmp_mem_access/cmp_mem_access.c)
set(CMP_MEM_ACCESS_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/../lib)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(cmp_mem_access  DEFAULT_MSG
                                  CMP_MEM_ACCESS_SOURCES CMP_MEM_ACCESS_INCLUDE_DIR)

