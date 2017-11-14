
set(MockStorage_SRCS storage.cpp storageprovider.cpp)
add_library(MockStorage OBJECT ${MockStorage_SRCS})
set_public_target_object_vars(MockStorage Qt5::Core trompeloeil)
KDE_TARGET_ENABLE_EXCEPTIONS(MockStorage PUBLIC)
