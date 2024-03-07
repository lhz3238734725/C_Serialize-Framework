set(test_sources /
    test-1.c /
    )

# 设置可执行文件的输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(helloworld ${test_sources})
target_link_libraries(helloworld sf-pack)
