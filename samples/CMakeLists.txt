
include_directories(BEFORE ../src)

set(base32_obj_SRCS ../src/otp/base32/base32.h $<TARGET_OBJECTS:Base32>)
set(skey_obj_SRCS ../src/otp/skey/skey.h $<TARGET_OBJECTS:SKey>)
set(oath_obj_SRCS ../src/otp/oath/oath.h $<TARGET_OBJECTS:OATH>)
set(token_obj_SRCS ../src/otp/token/token.h $<TARGET_OBJECTS:TOKEN>)
set(otp_obj_SRCS ../src/otp/otp.h $<TARGET_OBJECTS:OTP> ${base32_obj_SRCS} ${token_obj_SRCS} ${skey_obj_SRCS} ${oath_obj_SRCS})

set(generate_totp_token_SRCS generate-totp-token.cpp ${otp_obj_SRCS})
add_executable(generate-totp-token-sample ${generate_totp_token_SRCS})
target_link_libraries(generate-totp-token-sample Qt5::Core)