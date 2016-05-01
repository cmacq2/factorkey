
include_directories(BEFORE ../src)

set(base32_obj_SRCS ../src/otp/base32.h $<TARGET_OBJECTS:Base32>)
set(token_obj_SRCS ../src/otp/token.h $<TARGET_OBJECTS:OTPToken> ${base32_obj_SRCS})

set(generate_totp_token_SRCS generate-totp-token.cpp ${token_obj_SRCS})
add_executable(generate-totp-token-sample ${generate_totp_token_SRCS})
target_link_libraries(generate-totp-token-sample Qt5::Core)