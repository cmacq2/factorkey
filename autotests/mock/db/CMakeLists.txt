
set(MockMetadataDB_SRCS metadb.cpp metadatastoragehandler.cpp memory-metadb.cpp dummy-metadb.cpp)
add_library(MockMetadataDB OBJECT ${MockMetadataDB_SRCS})
set_public_target_object_vars(MockMetadataDB Qt5::Core Qt5::Test Qt5::Sql)

