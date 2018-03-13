function(git_clone name url dir)
  if(NOT EXISTS ${dir})
    file(MAKE_DIRECTORY ${dir})
  endif()

  if(NOT EXISTS ${dir}/${name})
    execute_process(
      COMMAND git clone ${url} ${name}
      WORKING_DIRECTORY ${dir}
    )

    execute_process(
      COMMAND git submodule update --init --recursive
      WORKING_DIRECTORY ${dir}/${name}
    )
  endif()
endfunction(git_clone)
