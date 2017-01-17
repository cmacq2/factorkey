
enable_testing()
find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Test)
include_directories(BEFORE ..)

add_subdirectory(lib)
add_subdirectory(mock)
add_subdirectory(util)
add_subdirectory(otp)
