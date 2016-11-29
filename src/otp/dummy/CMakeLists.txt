

set(DUMMY_SRCS dummy.cpp)
add_library(DUMMY OBJECT ${DUMMY_SRCS})
set_public_target_object_vars(DUMMY Qt5::Core)

set(DummyParameters_SRCS parameters.cpp)
add_library(DummyParameters OBJECT ${DummyParameters_SRCS})
set_public_target_object_vars(DummyParameters Qt5::Core)

set(DUMMY_GENERATOR_SRCS generator.cpp)
add_library(DUMMY_GENERATOR OBJECT ${DUMMY_GENERATOR_SRCS})
set_public_target_object_vars(DUMMY_GENERATOR Qt5::Core)

set(DUMMY_MetaDB_SRCS metadatastoragehandler.cpp)
add_library(DUMMY_MetaDB OBJECT ${DUMMY_MetaDB_SRCS})
set_public_target_object_vars(DUMMY_MetaDB Qt5::Core Qt5::Sql)
