
set(DB_SRCS secrets.cpp metadata.cpp storageprovider.cpp)
add_library(DB OBJECT ${DB_SRCS})
set_public_target_object_vars(DB Qt5::Core Qt5::Sql KF5::Wallet)
