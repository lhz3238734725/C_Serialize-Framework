set(sf_target_name sf-pack)

set(sf_headers /
    sf-def.h /
    sf-pack.h /
    )

set(sf_sources /
    sf-pack.c /
    )

# 设置库生成路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_library(${sf_target_name} SHARED ${sf_headers} ${sf_sources})

# 使用 ${sf_target_name} 生成的目标文件编译静态库
add_library(${sf_target_name}_static STATIC $<TARGET_OBJECTS:${sf_target_name}>)

# 将静态库重命名
set_target_properties(${sf_target_name}_static PROPERTIES OUTPUT_NAME ${sf_target_name})

# 设置 debug 和 realse 模式目标所带后缀
# set_target_properties(${sf_target_name} PROPERTIES DEBUG_POSTFIX "_d") 
# set_target_properties(${sf_target_name} PROPERTIES RELEASE_POSTFIX "_r")