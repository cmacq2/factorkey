
set(MetadataDBSanityTest_SRCS metadatastoragehandler-sanity.cpp)
add_library(MetadataDBSanityTest OBJECT ${MetadataDBSanityTest_SRCS})
set_public_target_object_vars(MetadataDBSanityTest Qt5::Core Qt5::Test Qt5::Sql)

set(MetadataTestUtils_SRCS metadata-test-utils.cpp)
add_library(MetadataTestUtils OBJECT ${MetadataTestUtils_SRCS})
set_public_target_object_vars(MetadataTestUtils Qt5::Core Qt5::Test Qt5::Sql)
