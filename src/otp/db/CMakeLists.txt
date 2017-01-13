
set(MetadataDB_SRCS metadata.cpp metadb.cpp metadatastoragehandler.cpp)
add_library(MetadataDB OBJECT ${MetadataDB_SRCS})
set_public_target_object_vars(MetadataDB Qt5::Core Qt5::Sql)

set(DBProvider_SRCS storageprovider_p.cpp storageprovider.cpp)
add_library(DBProvider OBJECT ${DBProvider_SRCS})
set_public_target_object_vars(DBProvider Qt5::Core Qt5::Sql KF5::Wallet)
