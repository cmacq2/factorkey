 
cmake_minimum_required(VERSION 2.8.12)

cmake_policy(SET CMP0048 NEW)

set(MY_PROJECT_ID FactorKey)
set(MY_VAR_PREFIX FACTORKEY)
#
# soversion is not a very memorable construct.
# It works like this:
# ${ABI}.${RELEASE}.${COMPAT}
# With:
#  ABI:     the number of times ABI was changed (added/removed symbols, reordered fields)
#  RELEASE: the number of releases under the same ABI, e.g. simple code fixes.
#  COMPAT:  the number of ABI_COUNT that this library is backwards compatible with.
#           E.g. 2 to indicate that stuff made for version ${ABI} - 2 still works on ${ABI}.
#
set(MY_ABIVERSION "0.0.0")
set(MY_VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/factorkey-version.h")
set(MY_PACKAGE_CONFIG_CMAKE "${CMAKE_CURRENT_BINARY_DIR}/${MY_PROJECT_ID}Config.cmake")
set(MY_VERSION_CMAKE "${CMAKE_CURRENT_BINARY_DIR}/${MY_PROJECT_ID}ConfigVersion.cmake")

project(${MY_PROJECT_ID} VERSION "0.1.0.0")

# Silence a warning
cmake_policy(SET CMP0063 NEW)

include(FeatureSummary)
find_package(ECM 5.13.0  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://projects.kde.org/projects/kdesupport/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_MODULE_PATH})

set(REQUIRED_QT_VERSION "5.4.0")

#set(KF5_VERSION "5.13.0") # handled by release scripts
set(KF5_DEP_VERSION "5.1.0") # handled by release scripts

find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED COMPONENTS Core Sql CONFIG)
include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)

set(CMAKE_CXX_STANDARD 14)

include(KDECMakeSettings)

find_package(KF5 ${KF5_DEP_VERSION} REQUIRED COMPONENTS I18n)

if(DEFINED WITH_KWALLET AND "${WITH_KWALLET}")
    find_package(KF5 ${KF5_DEP_VERSION} REQUIRED COMPONENTS Wallet)
    set(FACTORKEY_WITH_KWALLET 1)
else()
    set(FACTORKEY_WITH_KWALLET 0)
endif()

if(DEFINED WITH_QT_KEYCHAIN AND "${WITH_QT_KEYCHAIN}")
    find_package(Qt5Keychain 0.7.0 REQUIRED)
    set_package_properties(Qt PROPERTIES TYPE REQUIRED DESCRIPTION "QtKeychain" URL "https://github.com/frankosterfeld/qtkeychain")
    set(FACTORKEY_WITH_QT_KEYCHAIN 1)
elseif(NOT FACTORKEY_WITH_KWALLET AND NOT DEFINED WITH_QT_KEYCHAIN)
    message(STATUS "No available backends for 'secrets' configured, will default to: Qt Keychain (portable)")
    find_package(Qt5Keychain 0.7.0 REQUIRED)
    set(FACTORKEY_WITH_QT_KEYCHAIN 1)
else()
    set(FACTORKEY_WITH_QT_KEYCHAIN 0)
endif()

include(GenerateExportHeader)
include(ECMSetupVersion)
include(ECMGenerateHeaders)

ecm_setup_version(PROJECT
                  VARIABLE_PREFIX ${MY_VAR_PREFIX}
                  SOVERSION ${MY_ABIVERSION}
                  VERSION_HEADER ${MY_VERSION_HEADER}
                  PACKAGE_VERSION_FILE ${MY_VERSION_CMAKE})

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/po)
  include(ECMPoQmTools)
  ecm_install_po_files_as_qm(po)
endif()

include(CheckCxxFeatures)
include(SetTargetObjectVars)

include_directories(BEFORE src)

add_subdirectory(src)
add_subdirectory(autotests)

if(DEFINED INCLUDE_SAMPLE_APPS AND "${INCLUDE_SAMPLE_APPS}")
  add_subdirectory(samples)
endif()

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/${MY_PROJECT_ID}")
set(MY_TARGETS_NAME "${MY_PROJECT_ID}Targets")
set(MY_TARGETS_CMAKE "${MY_TARGETS_NAME}.cmake")

include(ECMPackageConfigHelpers)

ecm_configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/${MY_PROJECT_ID}Config.cmake.in"
  ${MY_PACKAGE_CONFIG_CMAKE}
  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
  ${MY_PACKAGE_CONFIG_CMAKE}
  ${MY_VERSION_CMAKE}
  DESTINATION ${CMAKECONFIG_INSTALL_DIR}
  COMPONENT Devel)

# install(EXPORT ${MY_TARGETS_NAME} DESTINATION ${CMAKECONFIG_INSTALL_DIR} FILE ${MY_TARGETS_CMAKE})
install(FILES ${MY_VERSION_HEADER} DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5} COMPONENT Devel)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
