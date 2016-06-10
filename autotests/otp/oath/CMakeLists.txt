
set(oath_UUT_SRCS ../../../src/otp/oath/oath.h $<TARGET_OBJECTS:OATH>)

set(base32_UUT_SRCS ../../../src/otp/base32/base32.h $<TARGET_OBJECTS:Base32>)
set(token_UUT_SRCS ../../../src/otp/token/token.h $<TARGET_OBJECTS:TOKEN>)

add_test(default-encoder-samples default-encoder-samples)
add_executable(default-encoder-samples default-encoder-samples.cpp ${oath_UUT_SRCS} ${base32_UUT_SRCS})
target_link_libraries(default-encoder-samples Qt5::Core Qt5::Test)

add_test(hotp-int-value hotp-int-value)
add_executable(hotp-int-value hotp-int-value.cpp ${oath_UUT_SRCS} ${base32_UUT_SRCS})
target_link_libraries(hotp-int-value Qt5::Core Qt5::Test)

add_test(totp-counter totp-counter)
add_executable(totp-counter totp-counter.cpp ${oath_UUT_SRCS} ${base32_UUT_SRCS})
target_link_libraries(totp-counter Qt5::Core Qt5::Test)

add_test(rfc4226-hotp-compat rfc4226-hotp-compat)
add_executable(rfc4226-hotp-compat rfc4226-hotp-compat.cpp ${oath_UUT_SRCS} ${base32_UUT_SRCS} ${token_UUT_SRCS})
target_link_libraries(rfc4226-hotp-compat Qt5::Core Qt5::Test)

