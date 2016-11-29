
add_subdirectory(base32)
add_subdirectory(oath)
add_subdirectory(skey)
add_subdirectory(dummy)
add_subdirectory(steam)
add_subdirectory(storage)
add_subdirectory(db)

set(base32_UUT_SRCS $<TARGET_OBJECTS:Base32>)
set(oath_UUT_SRCS $<TARGET_OBJECTS:OATH>)
set(oath_token_UUT_SRCS $<TARGET_OBJECTS:OATH_TOKEN>)
set(token_UUT_SRCS $<TARGET_OBJECTS:TOKEN>)
set(steam_UUT_SRCS $<TARGET_OBJECTS:Steam>)

set(otp_UUT_SRCS $<TARGET_OBJECTS:OTP>)

add_test(sailotp-hotp-compat sailotp-hotp-compat)
add_executable(sailotp-hotp-compat sailotp-hotp-compat.cpp ${base32_UUT_SRCS} ${token_UUT_SRCS} ${oath_UUT_SRCS} ${steam_UUT_SRCS} ${oath_token_UUT_SRCS})
target_link_libraries(sailotp-hotp-compat Qt5::Core Qt5::Test)
