function(windeployqt target)
get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)

add_custom_command(TARGET ${target} POST_BUILD
COMMAND "${_qt_bin_dir}/windeployqt.exe"         
            --verbose 1
            --release
            --no-svg
            --no-angle
            --no-opengl
            --no-opengl-sw
            --no-compiler-runtime
            --no-system-d3d-compiler
            --qmldir=${CMAKE_CURRENT_SOURCE_DIR}/rsc/               
            \"$<TARGET_FILE:${target}>\"
    COMMENT "Deploying Qt libraries using windeployqt for compilation target '${target}' ..."
    )

endfunction()