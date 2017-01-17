
set(base32_obj_SRCS $<TARGET_OBJECTS:Base32>)
set(skey_obj_SRCS $<TARGET_OBJECTS:SKey>)
set(oath_obj_SRCS $<TARGET_OBJECTS:OATH>)
set(oath_token_obj_SRCS $<TARGET_OBJECTS:OATH_TOKEN>)
set(token_obj_SRCS $<TARGET_OBJECTS:TOKEN>)
set(otp_obj_SRCS $<TARGET_OBJECTS:OTP>)

set(generate_totp_token_SRCS generate-totp-token.cpp ${base32_obj_SRCS} ${token_obj_SRCS} ${oath_obj_SRCS} ${oath_token_obj_SRCS})
add_executable(generate-totp-token-sample ${generate_totp_token_SRCS})
target_link_libraries(generate-totp-token-sample Qt5::Core)
