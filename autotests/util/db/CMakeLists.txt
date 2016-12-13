
set(MetadataDBSanityTest_SRCS metadatastoragehandler-sanity.cpp)
add_library(MetadataDBSanityTest OBJECT ${MetadataDBSanityTest_SRCS})
set_public_target_object_vars(MetadataDBSanityTest Qt5::Core Qt5::Test Qt5::Sql)
