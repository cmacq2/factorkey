
set(MemoryMetadataDB_SRCS memory-metadb.cpp)
add_library(MemoryMetadataDB OBJECT ${MemoryMetadataDB_SRCS})
set_public_target_object_vars(MemoryMetadataDB Qt5::Core Qt5::Sql)

set(MockMetadataDB_SRCS metadb.cpp metadatastoragehandler.cpp dummy-metadb.cpp)
add_library(MockMetadataDB OBJECT ${MockMetadataDB_SRCS})
set_public_target_object_vars(MockMetadataDB Qt5::Core Qt5::Sql gtest gmock)
