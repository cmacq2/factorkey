 
set(SecretsAPI_SRCS secrets.cpp)
add_library(SecretsAPI OBJECT ${SecretsAPI_SRCS})
set_public_target_object_vars(SecretsAPI Qt5::Core)
