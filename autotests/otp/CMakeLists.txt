
set(base32_UUT_SRCS ../../src/otp/base32.h $<TARGET_OBJECTS:Base32>)
set(token_UUT_SRCS ../../src/otp/token.h $<TARGET_OBJECTS:OTPToken> ${base32_UUT_SRCS})

add_test(base32-byte-coding base32-byte-coding)
add_executable(base32-byte-coding base32-byte-coding.cpp ${base32_UUT_SRCS})
target_link_libraries(base32-byte-coding Qt5::Core Qt5::Test)


add_test(base32-coding-decoding base32-coding-decoding)
add_executable(base32-coding-decoding base32-coding-decoding.cpp ${base32_UUT_SRCS})
target_link_libraries(base32-coding-decoding Qt5::Core Qt5::Test)

add_test(base32-encoding-compat base32-encoding-compat)
add_executable(base32-encoding-compat base32-encoding-compat.cpp ${base32_UUT_SRCS})
target_link_libraries(base32-encoding-compat Qt5::Core Qt5::Test)

add_test(hotp-int-value hotp-int-value)
add_executable(hotp-int-value hotp-int-value.cpp ${token_UUT_SRCS})
target_link_libraries(hotp-int-value Qt5::Core Qt5::Test)

add_test(default-encoder-samples default-encoder-samples)
add_executable(default-encoder-samples default-encoder-samples.cpp ${token_UUT_SRCS})
target_link_libraries(default-encoder-samples Qt5::Core Qt5::Test)

add_test(totp-counter totp-counter)
add_executable(totp-counter totp-counter.cpp ${token_UUT_SRCS})
target_link_libraries(totp-counter Qt5::Core Qt5::Test)

add_test(sailotp-hotp-compat sailotp-hotp-compat)
add_executable(sailotp-hotp-compat sailotp-hotp-compat.cpp ${token_UUT_SRCS})
target_link_libraries(sailotp-hotp-compat Qt5::Core Qt5::Test)