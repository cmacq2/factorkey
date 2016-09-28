

set(storage_SRCS storage.cpp storageprovider.cpp storage_p.cpp util.cpp)
add_library(Storage OBJECT ${storage_SRCS})
set_public_target_object_vars(Storage Qt5::Core)
