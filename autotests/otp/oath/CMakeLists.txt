
set(oath_UUT_SRCS $<TARGET_OBJECTS:OATH>)
set(oath_token_UUT_SRCS $<TARGET_OBJECTS:OATH_TOKEN>)

set(base32_UUT_SRCS $<TARGET_OBJECTS:Base32>)
set(token_UUT_SRCS $<TARGET_OBJECTS:TOKEN>)

add_test(default-encoder-samples default-encoder-samples)
add_executable(default-encoder-samples default-encoder-samples.cpp ${oath_UUT_SRCS} ${oath_token_UUT_SRCS} ${base32_UUT_SRCS})
target_link_libraries(default-encoder-samples Qt5::Core Qt5::Test)

add_test(hotp-int-value hotp-int-value)
add_executable(hotp-int-value hotp-int-value.cpp ${oath_UUT_SRCS} ${base32_UUT_SRCS})
target_link_libraries(hotp-int-value Qt5::Core Qt5::Test)

add_test(totp-counter totp-counter)
add_executable(totp-counter totp-counter.cpp ${oath_UUT_SRCS} ${base32_UUT_SRCS})
target_link_libraries(totp-counter Qt5::Core Qt5::Test)

add_test(rfc4226-hotp-compat rfc4226-hotp-compat)
add_executable(rfc4226-hotp-compat rfc4226-hotp-compat.cpp ${oath_UUT_SRCS} ${base32_UUT_SRCS} ${oath_token_UUT_SRCS} ${token_UUT_SRCS})
target_link_libraries(rfc4226-hotp-compat Qt5::Core Qt5::Test)

#set(storage_STUB_SRCS $<TARGET_OBJECTS:DummyStorage>)
set(storage_MOCK_SRCS $<TARGET_OBJECTS:MockStorage>)
set(generator_UUT_SRCS $<TARGET_OBJECTS:GENERATOR>)
set(oath_generator_UUT_SRCS $<TARGET_OBJECTS:OATH_GENERATOR>)
set(storage_UUT_SRCS $<TARGET_OBJECTS:Storage>)
set(util_UUT_SRCS $<TARGET_OBJECTS:Util>)

set(mocking_SUPPORT_SRCS $<TARGET_OBJECTS:MockingFrameworkIntegration>)

add_test(hotp-generator-samples hotp-generator-samples)
add_executable(hotp-generator-samples hotp-generator-samples.cpp
    ${oath_UUT_SRCS} ${base32_UUT_SRCS} ${oath_token_UUT_SRCS} ${token_UUT_SRCS}
    ${generator_UUT_SRCS} ${oath_generator_UUT_SRCS} ${storage_UUT_SRCS}
    ${util_UUT_SRCS} ${storage_MOCK_SRCS} ${mocking_SUPPORT_SRCS})
target_link_libraries(hotp-generator-samples Qt5::Core Qt5::Test trompeloeil)
KDE_TARGET_ENABLE_EXCEPTIONS(hotp-generator-samples PRIVATE)

add_test(totp-generator-samples totp-generator-samples)
add_executable(totp-generator-samples totp-generator-samples.cpp
    ${oath_UUT_SRCS} ${base32_UUT_SRCS} ${oath_token_UUT_SRCS} ${token_UUT_SRCS}
    ${generator_UUT_SRCS} ${oath_generator_UUT_SRCS} ${storage_UUT_SRCS}
    ${util_UUT_SRCS} ${storage_MOCK_SRCS} ${mocking_SUPPORT_SRCS})
target_link_libraries(totp-generator-samples Qt5::Core Qt5::Test trompeloeil)
KDE_TARGET_ENABLE_EXCEPTIONS(totp-generator-samples PRIVATE)

set(SharedParameters_UUT_SRCS $<TARGET_OBJECTS:SharedParameters>)
set(oath_parameters_UUT_SRCS $<TARGET_OBJECTS:OATHParameters>)
set(oath_metadatastorage_UUT_SRCS $<TARGET_OBJECTS:OATH_MetaDB>)
set(MetadataDB_UUT_SRCS $<TARGET_OBJECTS:MetadataDB>)
set(MetadataDBSanityTest_SRCS $<TARGET_OBJECTS:MetadataDBSanityTest>)

add_test(hotp-metadatastoragehandler-sanity hotp-metadatastoragehandler-sanity)
add_executable(hotp-metadatastoragehandler-sanity hotp-metadatastoragehandler-sanity.cpp
    ${SharedParameters_UUT_SRCS} ${oath_parameters_UUT_SRCS}
    ${MetadataDB_UUT_SRCS} ${oath_metadatastorage_UUT_SRCS} ${MetadataDBSanityTest_SRCS} ${storage_UUT_SRCS})
target_link_libraries(hotp-metadatastoragehandler-sanity Qt5::Core Qt5::Sql Qt5::Test)

add_test(totp-metadatastoragehandler-sanity totp-metadatastoragehandler-sanity)
add_executable(totp-metadatastoragehandler-sanity totp-metadatastoragehandler-sanity.cpp
    ${SharedParameters_UUT_SRCS} ${oath_parameters_UUT_SRCS}
    ${MetadataDB_UUT_SRCS} ${oath_metadatastorage_UUT_SRCS} ${MetadataDBSanityTest_SRCS} ${storage_UUT_SRCS})
target_link_libraries(totp-metadatastoragehandler-sanity Qt5::Core Qt5::Sql Qt5::Test)
