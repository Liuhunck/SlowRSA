include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(bigint_lib
    big_integer.cpp
)

add_library(rsa_lib
    random.cpp
    algorithm.cpp
    rsa_core.cpp
)

add_executable(rsa
    main.cpp
    utils.cpp
    argsparser.cpp
)

target_link_libraries(rsa_lib
    bigint_lib
)

target_link_libraries(rsa
    rsa_lib
)

# 安装目标（可选）
install(TARGETS rsa DESTINATION bin)