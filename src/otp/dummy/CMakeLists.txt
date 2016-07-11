

set(DUMMY_SRCS dummy.cpp)
add_library(DUMMY OBJECT ${DUMMY_SRCS})
set_public_target_object_vars(DUMMY Qt5::Core)

set(DUMMY_GENERATOR_SRCS generator.cpp)
add_library(DUMMY_GENERATOR OBJECT ${DUMMY_GENERATOR_SRCS})
set_public_target_object_vars(DUMMY_GENERATOR Qt5::Core)
