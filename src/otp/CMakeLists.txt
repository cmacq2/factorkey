
add_subdirectory(base32)
add_subdirectory(token)
add_subdirectory(skey)
add_subdirectory(oath)
add_subdirectory(steam)
add_subdirectory(dummy)
add_subdirectory(storage)
add_subdirectory(secrets)
add_subdirectory(db)
add_subdirectory(util)

set(SharedParameters_SRCS parameters.cpp)
add_library(SharedParameters OBJECT ${SharedParameters_SRCS})
set_public_target_object_vars(SharedParameters Qt5::Core)

set(generator_SRCS generator.cpp generator_p.cpp)
add_library(GENERATOR OBJECT ${generator_SRCS})
set_public_target_object_vars(GENERATOR Qt5::Core)
