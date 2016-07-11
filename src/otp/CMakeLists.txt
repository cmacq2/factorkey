
add_subdirectory(base32)
add_subdirectory(token)
add_subdirectory(skey)
add_subdirectory(oath)
add_subdirectory(steam)
add_subdirectory(dummy)

set(generator_SRCS generator.cpp storage.cpp storageprovider.cpp generator_p.cpp storage_p.cpp)
add_library(GENERATOR OBJECT ${generator_SRCS})
set_public_target_object_vars(GENERATOR Qt5::Core KF5::Wallet)
