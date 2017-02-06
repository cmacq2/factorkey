
#
# Export some build info from cmake to headers so the code can adapt.
# Note: .h extension is preserved to get syntax highlighting.
#
configure_file (config.in.h ${CMAKE_CURRENT_BINARY_DIR}/config.h)

set(AppStorageProviderDefault_SRCS storageprovider.cpp)
add_library(AppStorageProviderDefault OBJECT ${AppStorageProviderDefault_SRCS})
set_public_target_object_vars(AppStorageProviderDefault Qt5::Core Qt5::Sql)

if(DEFINED FACTORKEY_WITH_KWALLET AND "${FACTORKEY_WITH_KWALLET}")
    set_public_target_object_vars(AppStorageProviderDefault KF5::Wallet)
endif()

if(DEFINED FACTORKEY_WITH_QT_KEYCHAIN AND "${FACTORKEY_WITH_QT_KEYCHAIN}")
    #
    # Lowercasing of "qtkeychain" is intentional: it appears QtKeychain doesn't spell/case its own name consistently.
    #
    set_public_target_object_vars(AppStorageProviderDefault qt5keychain)
endif()
