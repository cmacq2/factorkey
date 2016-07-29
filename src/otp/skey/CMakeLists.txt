
set(SKey_SRCS skey.cpp words.cpp)
add_library(SKey OBJECT ${SKey_SRCS})
set_public_target_object_vars(SKey Qt5::Core)

set(SKeyToken_SRCS token.cpp)
add_library(SKeyToken OBJECT ${SKeyToken_SRCS})
set_public_target_object_vars(SKeyToken Qt5::Core)

