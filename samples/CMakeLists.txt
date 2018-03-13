function(sublist_dir result current_dir)
    file(GLOB children RELATIVE ${current_dir} ${current_dir}/*)
    set(dirlist "")
    foreach(child ${children})
        if (IS_DIRECTORY ${current_dir}/${child})
            list(APPEND dirlist ${child})
        endif()
    endforeach()
    set(${result} "${dirlist}" PARENT_SCOPE)
endfunction()

sublist_dir(sub_dirs ${CMAKE_CURRENT_SOURCE_DIR})
foreach(sub_dir ${sub_dirs})
    set(sample_dir ${CMAKE_CURRENT_SOURCE_DIR}/${sub_dir})
    set(sample_name ${sub_dir}-sample)

    file(GLOB_RECURSE sample_files
        ${sample_dir}/*.h
        ${sample_dir}/*.hpp
        ${sample_dir}/*.cpp
    )

    add_executable(${sample_name} "${sample_files}")
    target_link_libraries(${sample_name} libpride)
    add_filter_group("${sample_files}" "${sample_dir}")
endforeach()
