
set(DummyMetadataDB_SRCS metadb.cpp metadatastoragehandler.cpp)
add_library(DummyMetadataDB OBJECT ${DummyMetadataDB_SRCS})
set_public_target_object_vars(DummyMetadataDB Qt5::Core Qt5::Test Qt5::Sql)
