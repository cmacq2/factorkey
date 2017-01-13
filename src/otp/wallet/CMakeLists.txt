
set(WalletSecrets_SRCS secrets.cpp)
add_library(WalletSecrets OBJECT ${WalletSecrets_SRCS})
set_public_target_object_vars(WalletSecrets Qt5::Core KF5::Wallet)

