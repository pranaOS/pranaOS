function(compile_gml source output string_name)
    set(source ${CMAKE_CURRENT_SOURCE_DIR}/${source})
    add_custom_command(
        OUTPUT ${output}
        COMMAND ${pranaOS_SOURCE_DIR}/toolchain/text-to-cpp-string.sh ${string_name} ${source} > ${output}.tmp
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different ${output}.tmp ${output}
        COMMAND "${CMAKE_COMMAND}" -E remove ${output}.tmp
        VERBATIM
        DEPENDS ${pranaOS_SOURCE_DIR}/toolchain/text-to-cpp-string.sh
        MAIN_DEPENDENCY ${source}
    )
    get_filename_component(output_name ${output} NAME)
    add_custom_target(generate_${output_name} DEPENDS ${output})
    add_dependencies(all_generated generate_${output_name})
endfunction()

function(compile_ipc source output)
    set(source ${CMAKE_CURRENT_SOURCE_DIR}/${source})
    add_custom_command(
        OUTPUT ${output}
        COMMAND $<TARGET_FILE:Lagom::IPCCompiler> ${source} > ${output}.tmp
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different ${output}.tmp ${output}
        COMMAND "${CMAKE_COMMAND}" -E remove ${output}.tmp
        VERBATIM
        DEPENDS Lagom::IPCCompiler
        MAIN_DEPENDENCY ${source}
    )
    get_filename_component(output_name ${output} NAME)
    add_custom_target(generate_${output_name} DEPENDS ${output})
    add_dependencies(all_generated generate_${output_name})

    string(LENGTH ${pranaOS_SOURCE_DIR} root_source_dir_length)
    string(SUBSTRING ${CMAKE_CURRENT_SOURCE_DIR} ${root_source_dir_length} -1 current_source_dir_relative)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${output} DESTINATION usr/include${current_source_dir_relative} OPTIONAL)
endfunction()

function(generate_state_machine source header)
    get_filename_component(header_name ${header} NAME)
    set(target_name "generate_${header_name}")
    if(NOT TARGET ${target_name})
        set(source ${CMAKE_CURRENT_SOURCE_DIR}/${source})
        set(output ${CMAKE_CURRENT_BINARY_DIR}/${header})
        add_custom_command(
            OUTPUT ${output}
            COMMAND $<TARGET_FILE:Lagom::StateMachineGenerator> ${source} > ${output}.tmp
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different ${output}.tmp ${output}
            COMMAND "${CMAKE_COMMAND}" -E remove ${output}.tmp
            VERBATIM
            DEPENDS Lagom::StateMachineGenerator
            MAIN_DEPENDENCY ${source}
        )
        add_custom_target(${target_name} DEPENDS ${output})
        add_dependencies(all_generated ${target_name})
    endif()
endfunction()
