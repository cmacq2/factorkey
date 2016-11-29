

set(Steam_SRCS steam.cpp)
add_library(Steam OBJECT ${Steam_SRCS})
set_public_target_object_vars(Steam Qt5::Core)

set(SteamParameters_SRCS parameters.cpp)
add_library(SteamParameters OBJECT ${SteamParameters_SRCS})
set_public_target_object_vars(SteamParameters Qt5::Core)

set(SteamGenerator_SRCS generator.cpp)
add_library(SteamGenerator OBJECT ${SteamGenerator_SRCS})
set_public_target_object_vars(SteamGenerator Qt5::Core)

set(Steam_MetaDB_SRCS metadatastoragehandler.cpp)
add_library(Steam_MetaDB OBJECT ${Steam_MetaDB_SRCS})
set_public_target_object_vars(Steam_MetaDB Qt5::Core Qt5::Sql)
