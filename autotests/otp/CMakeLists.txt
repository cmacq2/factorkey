
add_subdirectory(base32)
add_subdirectory(oath)
add_subdirectory(skey)

set(base32_UUT_SRCS ../../src/otp/base32/base32.h $<TARGET_OBJECTS:Base32>)
set(skey_UUT_SRCS ../../src/otp/skey/skey.h $<TARGET_OBJECTS:SKey>)
set(oath_UUT_SRCS ../../src/otp/oath/oath.h $<TARGET_OBJECTS:OATH>)
set(token_UUT_SRCS ../../src/otp/token/token.h $<TARGET_OBJECTS:TOKEN>)
set(steam_UUT_SRCS ../../src/otp/steam/steam.h $<TARGET_OBJECTS:Steam>)

set(otp_UUT_SRCS ../../src/otp/otp.h $<TARGET_OBJECTS:OTP>)

add_test(sailotp-hotp-compat sailotp-hotp-compat)
add_executable(sailotp-hotp-compat sailotp-hotp-compat.cpp ${otp_UUT_SRCS} ${base32_UUT_SRCS} ${token_UUT_SRCS} ${oath_UUT_SRCS} ${steam_UUT_SRCS})
target_link_libraries(sailotp-hotp-compat Qt5::Core Qt5::Test)
