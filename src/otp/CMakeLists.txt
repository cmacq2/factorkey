
add_subdirectory(base32)
add_subdirectory(token)
add_subdirectory(skey)
add_subdirectory(oath)
add_subdirectory(steam)

set(otp_Imported_Headers token/token.h)
set(otp_SRCS otp.cpp ${otp_Imported_Headers})
add_library(OTP OBJECT ${otp_SRCS})
set_public_target_object_vars(OTP Qt5::Core)

set(generator_Imported_Headers ${otp_Imported_Headers} otp.h oath/token.h oath/oath.h steam/steam.h)
set(generator_SRCS generator.cpp storage.cpp storageprovider.cpp storage_p.cpp ${generator_Imported_Headers})
add_library(GENERATOR OBJECT ${generator_SRCS})
set_public_target_object_vars(GENERATOR Qt5::Core KF5::Wallet)
