
set(skey_UUT_SRCS $<TARGET_OBJECTS:SKey>)

add_test(skey-token-value skey-token-value)
add_executable(skey-token-value skey-token-value.cpp ${skey_UUT_SRCS})
target_link_libraries(skey-token-value Qt5::Core Qt5::Test)

add_test(skey-parse-challenge skey-parse-challenge)
add_executable(skey-parse-challenge skey-parse-challenge.cpp ${skey_UUT_SRCS})
target_link_libraries(skey-parse-challenge Qt5::Core Qt5::Test)

add_test(skey-format-challenge skey-format-challenge)
add_executable(skey-format-challenge skey-format-challenge.cpp ${skey_UUT_SRCS})
target_link_libraries(skey-format-challenge Qt5::Core Qt5::Test)

add_test(skey-validate-seed skey-validate-seed)
add_executable(skey-validate-seed skey-validate-seed.cpp ${skey_UUT_SRCS})
target_link_libraries(skey-validate-seed Qt5::Core Qt5::Test)

add_test(skey-validate-default-dict skey-validate-default-dict)
add_executable(skey-validate-default-dict skey-validate-default-dict.cpp ${skey_UUT_SRCS})
target_link_libraries(skey-validate-default-dict Qt5::Core Qt5::Test)

add_test(skey-default-words-rt skey-default-words-rt)
add_executable(skey-default-words-rt skey-default-words-rt.cpp ${skey_UUT_SRCS})
target_link_libraries(skey-default-words-rt Qt5::Core Qt5::Test)

add_test(skey-rfc2289-words-compat skey-rfc2289-words-compat)
add_executable(skey-rfc2289-words-compat skey-rfc2289-words-compat.cpp ${skey_UUT_SRCS})
target_link_libraries(skey-rfc2289-words-compat Qt5::Core Qt5::Test)

add_test(skey-rfc2289-compat skey-rfc2289-compat)
add_executable(skey-rfc2289-compat skey-rfc2289-compat.cpp ${skey_UUT_SRCS})
target_link_libraries(skey-rfc2289-compat Qt5::Core Qt5::Test)

set(skey_token_UUT_SRCS $<TARGET_OBJECTS:SKeyToken>)
set(base32_UUT_SRCS $<TARGET_OBJECTS:Base32>)
set(token_UUT_SRCS $<TARGET_OBJECTS:TOKEN>)
set(storage_MOCK_SRCS $<TARGET_OBJECTS:MockStorage>)
set(generator_UUT_SRCS $<TARGET_OBJECTS:GENERATOR>)
set(skey_generator_UUT_SRCS $<TARGET_OBJECTS:SKeyGenerator>)
set(storage_UUT_SRCS $<TARGET_OBJECTS:Storage>)
set(util_UUT_SRCS $<TARGET_OBJECTS:Util>)

set(mocking_SUPPORT_SRCS $<TARGET_OBJECTS:MockingFrameworkIntegration>)

add_test(skey-generator-samples skey-generator-samples)
add_executable(skey-generator-samples skey-generator-samples.cpp
    ${skey_UUT_SRCS} ${base32_UUT_SRCS} ${skey_token_UUT_SRCS} ${token_UUT_SRCS}
    ${generator_UUT_SRCS} ${skey_generator_UUT_SRCS} ${storage_UUT_SRCS}
    ${util_UUT_SRCS} ${storage_MOCK_SRCS} ${mocking_SUPPORT_SRCS})
target_link_libraries(skey-generator-samples Qt5::Core Qt5::Test trompeloeil)
KDE_TARGET_ENABLE_EXCEPTIONS(skey-generator-samples PRIVATE)

set(skey_metadatastorage_UUT_SRCS $<TARGET_OBJECTS:SKey_MetaDB>)
set(MetadataDB_UUT_SRCS $<TARGET_OBJECTS:MetadataDB>)
set(MetadataDBSanityTest_SRCS $<TARGET_OBJECTS:MetadataDBSanityTest>)

set(SharedParameters_UUT_SRCS $<TARGET_OBJECTS:SharedParameters>)
set(SKeyParameters_UUT_SRCS $<TARGET_OBJECTS:SKeyParameters>)

add_test(skey-metadatastoragehandler-sanity skey-metadatastoragehandler-sanity)
add_executable(skey-metadatastoragehandler-sanity skey-metadatastoragehandler-sanity.cpp
    ${SharedParameters_UUT_SRCS} ${SKeyParameters_UUT_SRCS}
    ${MetadataDB_UUT_SRCS} ${skey_metadatastorage_UUT_SRCS} ${MetadataDBSanityTest_SRCS} ${storage_UUT_SRCS})
target_link_libraries(skey-metadatastoragehandler-sanity Qt5::Core Qt5::Sql Qt5::Test)
