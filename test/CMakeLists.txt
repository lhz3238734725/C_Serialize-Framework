set(test_sources /
    test-sf.c /
    )

# 设置可执行文件的输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(test-sf ${test_sources})
target_link_libraries(test-sf sf-pack)

add_executable(test-2 test-2.c)
