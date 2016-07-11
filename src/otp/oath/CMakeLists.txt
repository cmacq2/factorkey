

set(OATH_SRCS oath.cpp)
add_library(OATH OBJECT ${OATH_SRCS})
set_public_target_object_vars(OATH Qt5::Core)

set(OATH_TOKEN_SRCS token.cpp)
add_library(OATH_TOKEN OBJECT ${OATH_TOKEN_SRCS})
set_public_target_object_vars(OATH_TOKEN Qt5::Core)

set(OATH_GENERATOR_SRCS generator.cpp)
add_library(OATH_GENERATOR OBJECT ${OATH_GENERATOR_SRCS})
set_public_target_object_vars(OATH_GENERATOR Qt5::Core)
