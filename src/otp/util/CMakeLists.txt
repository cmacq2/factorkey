
set(util_SRCS localeconversion.cpp)

add_library(Util OBJECT ${util_SRCS})
set_public_target_object_vars(Util Qt5::Core)
