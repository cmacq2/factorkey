
set(skey_UUT_SRCS $<TARGET_OBJECTS:SKey>)

add_test(skey-token-value skey-token-value)
add_executable(skey-token-value skey-token-value.cpp ${skey_UUT_SRCS})
target_link_libraries(skey-token-value Qt5::Core Qt5::Test)

add_test(skey-validate-seed skey-validate-seed)
add_executable(skey-validate-seed skey-validate-seed.cpp ${skey_UUT_SRCS})
target_link_libraries(skey-validate-seed Qt5::Core Qt5::Test)

add_test(skey-validate-default-dict skey-validate-default-dict)
add_executable(skey-validate-default-dict skey-validate-default-dict.cpp ${skey_UUT_SRCS})
target_link_libraries(skey-validate-default-dict Qt5::Core Qt5::Test)

add_test(skey-default-words-rt skey-default-words-rt)
add_executable(skey-default-words-rt skey-default-words-rt.cpp ${skey_UUT_SRCS})
target_link_libraries(skey-default-words-rt Qt5::Core Qt5::Test)

add_test(skey-rfc2289-words-compat skey-rfc2289-words-compat)
add_executable(skey-rfc2289-words-compat skey-rfc2289-words-compat.cpp ${skey_UUT_SRCS})
target_link_libraries(skey-rfc2289-words-compat Qt5::Core Qt5::Test)

add_test(skey-rfc2289-compat skey-rfc2289-compat)
add_executable(skey-rfc2289-compat skey-rfc2289-compat.cpp ${skey_UUT_SRCS})
target_link_libraries(skey-rfc2289-compat Qt5::Core Qt5::Test)
