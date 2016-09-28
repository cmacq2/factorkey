
set(MetadataDB_SRCS metadata.cpp metadb.cpp metadatastoragehandler.cpp)
add_library(MetadataDB OBJECT ${MetadataDB_SRCS})
set_public_target_object_vars(MetadataDB Qt5::Core Qt5::Sql)

set(SecretsDB_SRCS secrets.cpp)
add_library(SecretsDB OBJECT ${SecretsDB_SRCS})
set_public_target_object_vars(SecretsDB Qt5::Core KF5::Wallet)

set(DBProvider_SRCS storageprovider.cpp)
add_library(DBProvider OBJECT ${DBProvider_SRCS})
set_public_target_object_vars(DBProvider Qt5::Core Qt5::Sql KF5::Wallet)
