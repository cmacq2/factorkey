
#
# Export some build info from cmake to headers so the code can adapt.
# Note: .h extension is preserved to get syntax highlighting.
#
configure_file (config.in.h ${CMAKE_CURRENT_BINARY_DIR}/config.h)

if(DEFINED ALLOW_DUMPING AND "${ALLOW_DUMPING}")
    set(FACTORKEY_WITH_DUMPING 1)
else()
    set(FACTORKEY_WITH_DUMPING 0)
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(FACTORKEY_USE_LINUX_PRCTL 1)
    elseif(DEFINED ALLOW_DUMPING)
        message(FATAL_ERROR "\
You did not allowed dumping, and I do not know how to disable ptrace() and friends on this system/OS type: ${CMAKE_SYSTEM} \
Maybe you do? If so, you could patch factorkey to teach it how. \
Otherwise, if you don't mind the risk of other processes being able to attach to factorkey with ptrace() and possibly steal your passwords you can use -DALLOW_DUMPING=1 \
If you cannot accept that risk, it's back to (helping) patching factorkey unfortunately. \
")
    endif()
endif()

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

set(AppDebuggingSecurity_SRCS debugger.cpp)
add_library(AppDebuggingSecurity OBJECT ${AppDebuggingSecurity_SRCS})
set_public_target_object_vars(AppDebuggingSecurity Qt5::Core)

set(AppCliTemplate_SRCS cli.cpp)
add_library(AppCliTemplate OBJECT ${AppCliTemplate_SRCS})
set_public_target_object_vars(AppCliTemplate Qt5::Core)
