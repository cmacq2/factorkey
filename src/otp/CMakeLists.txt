
add_subdirectory(base32)
add_subdirectory(token)
add_subdirectory(skey)
add_subdirectory(oath)
add_subdirectory(steam)
add_subdirectory(dummy)
add_subdirectory(storage)
add_subdirectory(db)
add_subdirectory(util)

set(generator_SRCS generator.cpp generator_p.cpp)
add_library(GENERATOR OBJECT ${generator_SRCS})
set_public_target_object_vars(GENERATOR Qt5::Core)
