include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(bigint_lib
    big_integer.cpp
    random.cpp
)

add_executable(rsa
    main.cpp
    rsa_core.cpp
    key_gen.cpp
    utils.cpp
)

target_link_libraries(bigint_lib)

# 安装目标（可选）
install(TARGETS rsa DESTINATION bin)