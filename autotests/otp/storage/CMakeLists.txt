

set(DummyStorage_SRCS storage.cpp)
add_library(DummyStorage OBJECT ${DummyStorage_SRCS})
set_public_target_object_vars(DummyStorage Qt5::Core Qt5::Test KF5::Wallet)
