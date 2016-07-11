

set(base32_UUT_SRCS $<TARGET_OBJECTS:Base32>)
set(token_UUT_SRCS $<TARGET_OBJECTS:TOKEN>)
set(oath_UUT_SRCS $<TARGET_OBJECTS:OATH> $<TARGET_OBJECTS:OATH_TOKEN> $<TARGET_OBJECTS:OATH_GENERATOR>)
set(dummy_UUT_SRCS $<TARGET_OBJECTS:DUMMY> $<TARGET_OBJECTS:DUMMY_GENERATOR>)
set(steam_UUT_SRCS $<TARGET_OBJECTS:Steam> $<TARGET_OBJECTS:SteamGenerator>)

set(generator_internal_UUT_SRCS $<TARGET_OBJECTS:GENERATOR>)

add_test(locale-rt locale-rt)
add_executable(locale-rt locale-rt.cpp ${generator_internal_UUT_SRCS} ${base32_UUT_SRCS} ${token_UUT_SRCS} ${oath_UUT_SRCS} ${steam_UUT_SRCS} ${dummy_UUT_SRCS})
target_link_libraries(locale-rt Qt5::Core Qt5::Test KF5::Wallet)
