

set(oath_Imported_Headers ../token/token.h ../base32/base32.h)
set(OATH_SRCS oath.cpp token.cpp ${oath_Imported_Headers})
add_library(OATH OBJECT ${OATH_SRCS})
set_public_target_object_vars(OATH Qt5::Core)
