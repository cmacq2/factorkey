
set(uut_SRCS ../../src/otp/token.h $<TARGET_OBJECTS:OTPToken>)

add_test(hotp-int-value hotp-int-value)
add_executable(hotp-int-value hotp-int-value.cpp ${uut_SRCS})
target_link_libraries(hotp-int-value Qt5::Core Qt5::Test)

add_test(default-encoder-samples default-encoder-samples)
add_executable(default-encoder-samples default-encoder-samples.cpp ${uut_SRCS})
target_link_libraries(default-encoder-samples Qt5::Core Qt5::Test)

add_test(totp-counter totp-counter)
add_executable(totp-counter totp-counter.cpp ${uut_SRCS})
target_link_libraries(totp-counter Qt5::Core Qt5::Test)