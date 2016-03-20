
set(OTPToken_SRCS token.cpp)
add_library(OTPToken OBJECT ${OTPToken_SRCS})
set_public_target_object_vars(OTPToken Qt5::Core)

set(OTPGenerator_SRCS generator.cpp storage.cpp storageprovider.cpp token.cpp storage_p.cpp token.h)
add_library(OTPGenerator OBJECT ${OTPGenerator_SRCS})
set_public_target_object_vars(OTPGenerator Qt5::Core KF5::Wallet)