
set(storage_UUT_SRCS $<TARGET_OBJECTS:Storage>)
set(MetadataDB_UUT_SRCS $<TARGET_OBJECTS:MetadataDB>)

set(SharedParameters_UUT_SRCS $<TARGET_OBJECTS:SharedParameters>)
set(DummyParameters_UUT_SRCS $<TARGET_OBJECTS:DummyParameters>)
set(SKeyParameters_UUT_SRCS $<TARGET_OBJECTS:SKeyParameters>)
set(oath_parameters_UUT_SRCS $<TARGET_OBJECTS:OATHParameters>)
set(SteamParameters_UUT_SRCS $<TARGET_OBJECTS:SteamParameters>)
set(oath_metadatastorage_UUT_SRCS $<TARGET_OBJECTS:OATH_MetaDB>)
set(skey_metadatastorage_UUT_SRCS $<TARGET_OBJECTS:SKey_MetaDB>)
set(dummy_metadatastorage_UUT_SRCS $<TARGET_OBJECTS:DUMMY_MetaDB>)
set(Steam_MetaDB_UUT_SRCS $<TARGET_OBJECTS:Steam_MetaDB>)

set(MemoryMetadataDB_STUB_SRCS $<TARGET_OBJECTS:MemoryMetadataDB>)
set(MetadataDB_STUB_SRCS $<TARGET_OBJECTS:StubbedMetadataDB>)
set(MetadataDB_MOCK_SRCS $<TARGET_OBJECTS:MockMetadataDB>)

set(MetadataTestUtil_UUT_SRCS $<TARGET_OBJECTS:MetadataTestUtils>)
set(MemoryMetadataTestUtil_UUT_SRCS $<TARGET_OBJECTS:MemoryMetadataTestUtils>)

add_test(poll-metadata poll-metadata)
add_executable(poll-metadata poll-metadata.cpp
    ${SharedParameters_UUT_SRCS} ${DummyParameters_UUT_SRCS} ${oath_parameters_UUT_SRCS} ${SKeyParameters_UUT_SRCS} ${SteamParameters_UUT_SRCS}
    ${MetadataDB_UUT_SRCS} ${MetadataDB_STUB_SRCS} ${storage_UUT_SRCS} ${MetadataTestUtil_UUT_SRCS})
target_link_libraries(poll-metadata Qt5::Core Qt5::Sql Qt5::Test)

add_test(commit-metadata commit-metadata)
add_executable(commit-metadata commit-metadata.cpp
    ${SharedParameters_UUT_SRCS} ${DummyParameters_UUT_SRCS} ${oath_parameters_UUT_SRCS} ${SKeyParameters_UUT_SRCS} ${SteamParameters_UUT_SRCS}
    ${MetadataDB_UUT_SRCS} ${MetadataDB_STUB_SRCS} ${storage_UUT_SRCS} ${MetadataTestUtil_UUT_SRCS})
target_link_libraries(commit-metadata Qt5::Core Qt5::Sql Qt5::Test)

add_test(write-metadata write-metadata)
add_executable(write-metadata write-metadata.cpp
    ${SharedParameters_UUT_SRCS} ${DummyParameters_UUT_SRCS} ${oath_parameters_UUT_SRCS} ${SKeyParameters_UUT_SRCS} ${SteamParameters_UUT_SRCS}
    ${MetadataDB_UUT_SRCS} ${MetadataDB_STUB_SRCS} ${storage_UUT_SRCS} ${MetadataTestUtil_UUT_SRCS})
target_link_libraries(write-metadata Qt5::Core Qt5::Sql Qt5::Test)

add_test(convert-metadata convert-metadata)
add_executable(convert-metadata convert-metadata.cpp
    ${SharedParameters_UUT_SRCS} ${DummyParameters_UUT_SRCS} ${oath_parameters_UUT_SRCS} ${SKeyParameters_UUT_SRCS} ${SteamParameters_UUT_SRCS}
    ${MetadataDB_UUT_SRCS} ${MetadataDB_STUB_SRCS} ${storage_UUT_SRCS} ${MetadataTestUtil_UUT_SRCS})
target_link_libraries(convert-metadata Qt5::Core Qt5::Sql Qt5::Test)

add_test(metadb-schema metadb-schema)
add_executable(metadb-schema metadb-schema.cpp
    ${SharedParameters_UUT_SRCS} ${DummyParameters_UUT_SRCS} ${oath_parameters_UUT_SRCS} ${SKeyParameters_UUT_SRCS} ${SteamParameters_UUT_SRCS}
    ${MetadataDB_UUT_SRCS} ${MemoryMetadataDB_STUB_SRCS}
    ${dummy_metadatastorage_UUT_SRCS} ${oath_metadatastorage_UUT_SRCS} ${skey_metadatastorage_UUT_SRCS} ${Steam_MetaDB_UUT_SRCS} ${storage_UUT_SRCS} ${MemoryMetadataTestUtil_UUT_SRCS})
target_link_libraries(metadb-schema Qt5::Core Qt5::Sql Qt5::Test)

add_test(metadb-poll-metadata metadb-poll-metadata)
add_executable(metadb-poll-metadata metadb-poll-metadata.cpp
    ${SharedParameters_UUT_SRCS} ${DummyParameters_UUT_SRCS} ${oath_parameters_UUT_SRCS} ${SKeyParameters_UUT_SRCS} ${SteamParameters_UUT_SRCS}
    ${MetadataDB_UUT_SRCS} ${MemoryMetadataDB_STUB_SRCS}
    ${dummy_metadatastorage_UUT_SRCS} ${oath_metadatastorage_UUT_SRCS} ${skey_metadatastorage_UUT_SRCS} ${Steam_MetaDB_UUT_SRCS} ${storage_UUT_SRCS} ${MemoryMetadataTestUtil_UUT_SRCS})
target_link_libraries(metadb-poll-metadata Qt5::Core Qt5::Sql Qt5::Test)

add_test(metadb-create-metadata metadb-create-metadata)
add_executable(metadb-create-metadata metadb-create-metadata.cpp
    ${SharedParameters_UUT_SRCS} ${DummyParameters_UUT_SRCS} ${oath_parameters_UUT_SRCS} ${SKeyParameters_UUT_SRCS} ${SteamParameters_UUT_SRCS}
    ${MetadataDB_UUT_SRCS} ${MemoryMetadataDB_STUB_SRCS}
    ${dummy_metadatastorage_UUT_SRCS} ${oath_metadatastorage_UUT_SRCS} ${skey_metadatastorage_UUT_SRCS} ${Steam_MetaDB_UUT_SRCS} ${storage_UUT_SRCS} ${MemoryMetadataTestUtil_UUT_SRCS})
target_link_libraries(metadb-create-metadata Qt5::Core Qt5::Sql Qt5::Test)

add_test(metadb-write-metadata metadb-write-metadata)
add_executable(metadb-write-metadata metadb-write-metadata.cpp
    ${SharedParameters_UUT_SRCS} ${DummyParameters_UUT_SRCS} ${oath_parameters_UUT_SRCS} ${SKeyParameters_UUT_SRCS} ${SteamParameters_UUT_SRCS}
    ${MetadataDB_UUT_SRCS} ${MemoryMetadataDB_STUB_SRCS}
    ${dummy_metadatastorage_UUT_SRCS} ${oath_metadatastorage_UUT_SRCS} ${skey_metadatastorage_UUT_SRCS} ${Steam_MetaDB_UUT_SRCS} ${storage_UUT_SRCS} ${MemoryMetadataTestUtil_UUT_SRCS})
target_link_libraries(metadb-write-metadata Qt5::Core Qt5::Sql Qt5::Test)

add_test(metadb-convert-metadata metadb-convert-metadata)
add_executable(metadb-convert-metadata metadb-convert-metadata.cpp
    ${SharedParameters_UUT_SRCS} ${DummyParameters_UUT_SRCS} ${oath_parameters_UUT_SRCS} ${SKeyParameters_UUT_SRCS} ${SteamParameters_UUT_SRCS}
    ${MetadataDB_UUT_SRCS} ${MemoryMetadataDB_STUB_SRCS}
    ${dummy_metadatastorage_UUT_SRCS} ${oath_metadatastorage_UUT_SRCS} ${skey_metadatastorage_UUT_SRCS} ${Steam_MetaDB_UUT_SRCS} ${storage_UUT_SRCS} ${MemoryMetadataTestUtil_UUT_SRCS})
target_link_libraries(metadb-convert-metadata Qt5::Core Qt5::Sql Qt5::Test)
