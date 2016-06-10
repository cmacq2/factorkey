
set(Base32_SRCS base32.cpp)
add_library(Base32 OBJECT ${Base32_SRCS})
set_public_target_object_vars(Base32 Qt5::Core)