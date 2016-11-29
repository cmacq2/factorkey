
set(storage_UUT_SRCS $<TARGET_OBJECTS:Storage>)
set(Steam_MetaDB_UUT_SRCS $<TARGET_OBJECTS:Steam_MetaDB>)
set(MetadataDB_UUT_SRCS $<TARGET_OBJECTS:MetadataDB>)
set(MetadataDBSanityTest_SRCS $<TARGET_OBJECTS:MetadataDBSanityTest>)

set(SharedParameters_UUT_SRCS $<TARGET_OBJECTS:SharedParameters>)
set(SteamParameters_UUT_SRCS $<TARGET_OBJECTS:SteamParameters>)

add_test(steam-metadatastoragehandler-sanity steam-metadatastoragehandler-sanity)
add_executable(steam-metadatastoragehandler-sanity steam-metadatastoragehandler-sanity.cpp
    ${SharedParameters_UUT_SRCS} ${SteamParameters_UUT_SRCS}
    ${MetadataDB_UUT_SRCS} ${Steam_MetaDB_UUT_SRCS} ${MetadataDBSanityTest_SRCS} ${storage_UUT_SRCS})
target_link_libraries(steam-metadatastoragehandler-sanity Qt5::Core Qt5::Sql Qt5::Test)
