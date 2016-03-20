
include_directories(BEFORE ../src)

set(generate_totp_token_SRCS generate-totp-token.cpp ../src/otp/token.h $<TARGET_OBJECTS:OTPToken>)
add_executable(generate-totp-token-sample ${generate_totp_token_SRCS})
target_link_libraries(generate-totp-token-sample Qt5::Core)