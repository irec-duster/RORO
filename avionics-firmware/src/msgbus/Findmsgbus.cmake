
set(MSGBUS_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/msgbus.c
    ${CMAKE_CURRENT_LIST_DIR}/type_print.c
    ${CMAKE_CURRENT_LIST_DIR}/serialization_msgpack.c)
set(MSGBUS_PORT_POSIX_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/ports/posix/msgbus_port.c)
set(MSGBUS_PORT_UNITTEST_MOCK_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/tests/mocks/synchronization.cpp)

set(MSGBUS_INCLUDE_DIR
    ${CMAKE_CURRENT_LIST_DIR}/..)
set(MSGBUS_PORT_POSIX_INCLUDE_DIR
    ${CMAKE_CURRENT_LIST_DIR}/ports/posix)
set(MSGBUS_PORT_UNITTEST_MOCK_INCLUDE_DIR
    ${CMAKE_CURRENT_LIST_DIR}/tests)


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(msgbus  DEFAULT_MSG
                                  MSGBUS_SOURCES MSGBUS_INCLUDE_DIR)

