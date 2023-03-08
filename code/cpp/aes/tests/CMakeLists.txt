project(aes_tests)

add_executable(aes_tests_run tests.cpp)

#target_link_libraries(aes_tests_run aes) #TODO: remove

target_link_libraries(aes_tests_run gtest gtest_main)