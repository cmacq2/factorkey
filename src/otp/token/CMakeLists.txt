
set(TOKEN_SRCS token.cpp)
add_library(TOKEN OBJECT ${TOKEN_SRCS})
set_public_target_object_vars(TOKEN Qt5::Core)