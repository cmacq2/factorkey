
set(base32_UUT_SRCS ../../../src/otp/base32/base32.h $<TARGET_OBJECTS:Base32>)

add_test(base32-byte-coding base32-byte-coding)
add_executable(base32-byte-coding base32-byte-coding.cpp ${base32_UUT_SRCS})
target_link_libraries(base32-byte-coding Qt5::Core Qt5::Test)

add_test(base32-coding-decoding base32-coding-decoding)
add_executable(base32-coding-decoding base32-coding-decoding.cpp ${base32_UUT_SRCS})
target_link_libraries(base32-coding-decoding Qt5::Core Qt5::Test)

add_test(base32-encoding-compat base32-encoding-compat)
add_executable(base32-encoding-compat base32-encoding-compat.cpp ${base32_UUT_SRCS})
target_link_libraries(base32-encoding-compat Qt5::Core Qt5::Test)