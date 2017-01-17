
set(KeychainSecrets_SRCS secrets.cpp)
add_library(KeychainSecrets OBJECT ${KeychainSecrets_SRCS})
#
# Lowercasing of "qt5keychain" is intentional: it appears the name of Qt5Keychain is not spelled/cased consistently.
#
set_public_target_object_vars(KeychainSecrets Qt5::Core qt5keychain)
