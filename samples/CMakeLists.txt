
set(base32_obj_SRCS $<TARGET_OBJECTS:Base32>)
set(skey_obj_SRCS $<TARGET_OBJECTS:SKey>)
set(oath_obj_SRCS $<TARGET_OBJECTS:OATH>)
set(oath_token_obj_SRCS $<TARGET_OBJECTS:OATH_TOKEN>)
set(token_obj_SRCS $<TARGET_OBJECTS:TOKEN>)
set(otp_util_obj_SRCS $<TARGET_OBJECTS:Util>)

set(generate_totp_token_SRCS generate-totp-token.cpp ${base32_obj_SRCS} ${token_obj_SRCS} ${oath_obj_SRCS} ${oath_token_obj_SRCS})
add_executable(generate-totp-token-sample ${generate_totp_token_SRCS})
target_link_libraries(generate-totp-token-sample Qt5::Core)

set(dummy_obj_srcs $<TARGET_OBJECTS:DUMMY>)
set(steam_obj_srcs $<TARGET_OBJECTS:Steam>)
set(skey_token_obj_srcs $<TARGET_OBJECTS:SKeyToken>)
set(token_parameter_obj_srcs $<TARGET_OBJECTS:SharedParameters> $<TARGET_OBJECTS:OATHParameters> $<TARGET_OBJECTS:SKeyParameters> $<TARGET_OBJECTS:DummyParameters> $<TARGET_OBJECTS:SteamParameters>)
set(generator_obj_srcs $<TARGET_OBJECTS:GENERATOR> $<TARGET_OBJECTS:OATH_GENERATOR> $<TARGET_OBJECTS:SKeyGenerator> $<TARGET_OBJECTS:SteamGenerator> $<TARGET_OBJECTS:DUMMY_GENERATOR>)
set(storage_obj_srcs $<TARGET_OBJECTS:Storage>)
set(storagehandler_obj_srcs $<TARGET_OBJECTS:OATH_MetaDB> $<TARGET_OBJECTS:SKey_MetaDB> $<TARGET_OBJECTS:DUMMY_MetaDB> $<TARGET_OBJECTS:Steam_MetaDB>)
set(metadb_obj_srcs $<TARGET_OBJECTS:MetadataDB> $<TARGET_OBJECTS:DBProvider>)
set(secrets_api_obj_srcs $<TARGET_OBJECTS:SecretsAPI>)
set(app_storage_obj_srcs $<TARGET_OBJECTS:AppStorageProviderDefault>)
if(DEFINED FACTORKEY_WITH_KWALLET AND "${FACTORKEY_WITH_KWALLET}")
    set(wallet_obj_srcs $<TARGET_OBJECTS:WalletSecrets>)
else()
    unset(wallet_obj_srcs)
endif()
if(DEFINED FACTORKEY_WITH_QT_KEYCHAIN AND "${FACTORKEY_WITH_QT_KEYCHAIN}")
    set(keychain_obj_srcs $<TARGET_OBJECTS:KeychainSecrets>)
else()
    unset(keychain_obj_srcs)
endif()

set(obj_libs_SRCS shared-cli-options.cpp
    ${base32_obj_SRCS} ${dummy_obj_srcs} ${otp_util_obj_SRCS} ${token_obj_SRCS} ${token_parameter_obj_srcs}
    ${oath_obj_SRCS} ${oath_token_obj_SRCS} ${steam_obj_srcs} ${skey_obj_SRCS} ${skey_token_obj_srcs}
    ${storage_obj_srcs} ${metadb_obj_srcs} ${storagehandler_obj_srcs}
    ${secrets_api_obj_srcs} ${generator_obj_srcs}
    ${wallet_obj_srcs} ${keychain_obj_srcs} ${app_storage_obj_srcs})

set(lookup_token_SRCS lookup-token-sample.cpp ${obj_libs_SRCS})
set(create_token_SRCS create-token-sample.cpp ${obj_libs_SRCS})
add_executable(lookup-token-sample ${lookup_token_SRCS})
add_executable(create-token-sample ${create_token_SRCS})

target_link_libraries(lookup-token-sample Qt5::Core Qt5::Sql)
target_link_libraries(create-token-sample Qt5::Core Qt5::Sql)
if(DEFINED FACTORKEY_WITH_KWALLET AND "${FACTORKEY_WITH_KWALLET}")
    target_link_libraries(lookup-token-sample KF5::Wallet)
    target_link_libraries(create-token-sample KF5::Wallet)
endif()
if(DEFINED FACTORKEY_WITH_QT_KEYCHAIN AND "${FACTORKEY_WITH_QT_KEYCHAIN}")
    target_link_libraries(lookup-token-sample qt5keychain)
    target_link_libraries(create-token-sample qt5keychain)
endif()

