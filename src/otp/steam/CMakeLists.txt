

set(steam_Imported_Headers ../oath/oath.h)
set(Steam_SRCS steam.cpp ${steam_Imported_Headers})
add_library(Steam OBJECT ${Steam_SRCS})
set_public_target_object_vars(Steam Qt5::Core)
