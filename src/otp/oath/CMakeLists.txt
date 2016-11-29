
set(OATH_SRCS oath.cpp)
add_library(OATH OBJECT ${OATH_SRCS})
set_public_target_object_vars(OATH Qt5::Core)

set(OATH_TOKEN_SRCS token.cpp)
add_library(OATH_TOKEN OBJECT ${OATH_TOKEN_SRCS})
set_public_target_object_vars(OATH_TOKEN Qt5::Core)

set(OATHParameters_SRCS parameters.cpp)
add_library(OATHParameters OBJECT ${OATHParameters_SRCS})
set_public_target_object_vars(OATHParameters Qt5::Core)

set(OATH_GENERATOR_SRCS generator.cpp)
add_library(OATH_GENERATOR OBJECT ${OATH_GENERATOR_SRCS})
set_public_target_object_vars(OATH_GENERATOR Qt5::Core)

set(OATH_MetaDB_SRCS metadatastoragehandler.cpp)
add_library(OATH_MetaDB OBJECT ${OATH_MetaDB_SRCS})
set_public_target_object_vars(OATH_MetaDB Qt5::Core Qt5::Sql)
