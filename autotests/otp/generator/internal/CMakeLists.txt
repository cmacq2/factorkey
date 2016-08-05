

set(base32_FIXME_SRCS $<TARGET_OBJECTS:Base32>)
set(token_FIXME_SRCS $<TARGET_OBJECTS:TOKEN>)

set(generator_internal_UUT_SRCS $<TARGET_OBJECTS:GENERATOR>)

add_test(locale-rt locale-rt)
add_executable(locale-rt locale-rt.cpp ${generator_internal_UUT_SRCS}
    ${base32_FIXME_SRCS} ${token_FIXME_SRCS})
target_link_libraries(locale-rt Qt5::Core Qt5::Test KF5::Wallet)
