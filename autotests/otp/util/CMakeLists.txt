

set(util_UUT_SRCS $<TARGET_OBJECTS:Util>)

add_test(locale-rt locale-rt)
add_executable(locale-rt locale-rt.cpp ${util_UUT_SRCS})
target_link_libraries(locale-rt Qt5::Core Qt5::Test)
