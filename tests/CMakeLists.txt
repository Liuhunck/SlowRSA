include(GoogleTest)

function(add_unit_test TARGET_NAME)
    set(SOURCE_FILES ${ARGN})
    add_executable(${TARGET_NAME} ${SOURCE_FILES})
    target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(${TARGET_NAME}
        PRIVATE
        bigint_lib
        GTest::gtest_main
        # OpenMP::OpenMP_CXX
    )
    gtest_discover_tests(${TARGET_NAME})
endfunction()

add_unit_test(BigIntegerTest
    big_integer_test.cpp
)

add_unit_test(RandomTest
    random_test.cpp
)
