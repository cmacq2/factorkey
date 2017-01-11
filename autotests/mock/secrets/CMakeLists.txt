
set(MockSecrets_SRCS secrets.cpp)
add_library(MockSecrets OBJECT ${MockSecrets_SRCS})
set_public_target_object_vars(MockSecrets Qt5::Core gtest gmock)
