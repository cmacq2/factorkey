
add_subdirectory(otp)

if(DEFINED FACTORKEY_WITH_KWALLET AND "${FACTORKEY_WITH_KWALLET}")
    add_subdirectory(wallet)
endif()
if(DEFINED FACTORKEY_WITH_QT_KEYCHAIN AND "${FACTORKEY_WITH_QT_KEYCHAIN}")
    add_subdirectory(keychain)
endif()

