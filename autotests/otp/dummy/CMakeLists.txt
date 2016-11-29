
set(storage_UUT_SRCS $<TARGET_OBJECTS:Storage>)
set(dummy_metadatastorage_UUT_SRCS $<TARGET_OBJECTS:DUMMY_MetaDB>)
set(MetadataDB_UUT_SRCS $<TARGET_OBJECTS:MetadataDB>)
set(MetadataDBSanityTest_SRCS $<TARGET_OBJECTS:MetadataDBSanityTest>)

set(SharedParameters_UUT_SRCS $<TARGET_OBJECTS:SharedParameters>)
set(DummyParameters_UUT_SRCS $<TARGET_OBJECTS:DummyParameters>)

add_test(dummy-metadatastoragehandler-sanity dummy-metadatastoragehandler-sanity)
add_executable(dummy-metadatastoragehandler-sanity dummy-metadatastoragehandler-sanity.cpp
    ${SharedParameters_UUT_SRCS} ${DummyParameters_UUT_SRCS}
    ${MetadataDB_UUT_SRCS} ${dummy_metadatastorage_UUT_SRCS} ${MetadataDBSanityTest_SRCS} ${storage_UUT_SRCS})
target_link_libraries(dummy-metadatastoragehandler-sanity Qt5::Core Qt5::Sql Qt5::Test)
