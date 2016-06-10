
set(SKey_SRCS skey.cpp words.cpp ../token/token.h)
add_library(SKey OBJECT ${SKey_SRCS})
set_public_target_object_vars(SKey Qt5::Core)
