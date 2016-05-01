
set(Base32_SRCS base32.cpp)
add_library(Base32 OBJECT ${Base32_SRCS})
set_public_target_object_vars(Base32 Qt5::Core)

set(OTPToken_SRCS token.cpp base32.h)
add_library(OTPToken OBJECT ${OTPToken_SRCS})
set_public_target_object_vars(OTPToken Qt5::Core)

set(OTPGenerator_SRCS generator.cpp storage.cpp storageprovider.cpp storage_p.cpp token.h base32.h)
add_library(OTPGenerator OBJECT ${OTPGenerator_SRCS})
set_public_target_object_vars(OTPGenerator Qt5::Core KF5::Wallet)