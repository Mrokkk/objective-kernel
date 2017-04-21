function(add_run_target executable target-name)
    add_custom_target(
        ${target-name}
        DEPENDS ${executable}
        COMMAND ./${executable}
        COMMENT "Running ${executable}"
    )
endfunction(add_run_target)

function(add_valgrind_target executable target-name)
    add_custom_target(
        ${target-name}
        DEPENDS ${executable}
        COMMAND valgrind ./${executable}
        COMMENT "Running ${executable} with Valgrind"
    )
endfunction(add_valgrind_target)

function(add_sanitizers_flags)
    set(SANITIZERS_FLAGS "-fsanitize=address,undefined")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${SANITIZERS_FLAGS}" PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZERS_FLAGS}")
endfunction(add_sanitizers_flags)

