
set(SKey_SRCS skey.cpp words.cpp)
add_library(SKey OBJECT ${SKey_SRCS})
set_public_target_object_vars(SKey Qt5::Core)

set(SKeyToken_SRCS token.cpp)
add_library(SKeyToken OBJECT ${SKeyToken_SRCS})
set_public_target_object_vars(SKeyToken Qt5::Core)

set(SKeyParameters_SRCS parameters.cpp)
add_library(SKeyParameters OBJECT ${SKeyParameters_SRCS})
set_public_target_object_vars(SKeyParameters Qt5::Core)

set(SKeyGenerator_SRCS generator.cpp)
add_library(SKeyGenerator OBJECT ${SKeyGenerator_SRCS})
set_public_target_object_vars(SKeyGenerator Qt5::Core)

set(SKey_MetaDB_SRCS metadatastoragehandler.cpp)
add_library(SKey_MetaDB OBJECT ${SKey_MetaDB_SRCS})
set_public_target_object_vars(SKey_MetaDB Qt5::Core Qt5::Sql)
