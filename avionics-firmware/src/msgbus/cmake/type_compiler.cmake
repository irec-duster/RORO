
set(TYPE_COMPILER ${CMAKE_CURRENT_LIST_DIR}/../type_compiler/type_compiler.py)
set(TYPE_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/types)
set(TYPE_FILE_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR})

function(type_compile filename)
    get_filename_component(base ${filename} NAME_WE)
    set(base_abs ${TYPE_OUTPUT_DIR}/${base})
    file(MAKE_DIRECTORY ${TYPE_OUTPUT_DIR})
    set(output ${base_abs}.c ${base_abs}.h)
    add_custom_command(
        OUTPUT ${output}
        COMMAND python3 ${TYPE_COMPILER} ${filename} -o ${base_abs}
        DEPENDS ${filename})
    set_source_files_properties(${output} PROPERTIES GENERATED TRUE)
endfunction()
