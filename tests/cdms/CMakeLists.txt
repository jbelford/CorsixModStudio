find_package(GTest CONFIG REQUIRED)
find_package(wxWidgets REQUIRED COMPONENTS core base aui html xml xrc propgrid stc adv)
include(${wxWidgets_USE_FILE})

# CDMS source files under test (non-UI utility modules)
set(CDMS_SRC_DIR ${CMAKE_SOURCE_DIR}/src/cdms)

add_executable(cdms_tests
    placeholder_test.cpp
    strconv_test.cpp
    config_test.cpp
    strings_test.cpp
    utility_test.cpp
    result_test.cpp
    fileservice_test.cpp
    moduleservice_test.cpp
    formatservice_test.cpp
    hashservice_test.cpp
    cancellationtoken_test.cpp
    progresschannel_test.cpp
    taskrunner_test.cpp
    moduleloadpresenter_test.cpp
    sgamakepresenter_test.cpp
    ucstodatpresenter_test.cpp
    dpscalculatorpresenter_test.cpp
    luainherittreepresenter_test.cpp
    rgdmacropresenter_test.cpp
    massextractpresenter_test.cpp
    refreshfilespresenter_test.cpp
    ${CDMS_SRC_DIR}/strconv.cpp
    ${CDMS_SRC_DIR}/strings.cpp
    ${CDMS_SRC_DIR}/Utility.cpp
    ${CDMS_SRC_DIR}/config.cpp
    ${CDMS_SRC_DIR}/services/ModuleService.cpp
    ${CDMS_SRC_DIR}/services/FileService.cpp
    ${CDMS_SRC_DIR}/services/FormatService.cpp
    ${CDMS_SRC_DIR}/services/HashService.cpp
    ${CDMS_SRC_DIR}/async/CProgressChannel.cpp
    ${CDMS_SRC_DIR}/async/CTaskRunner.cpp
    ${CDMS_SRC_DIR}/async/CWxTaskRunner.cpp
    ${CDMS_SRC_DIR}/presenters/CModuleLoadPresenter.cpp
    ${CDMS_SRC_DIR}/presenters/CSgaMakePresenter.cpp
    ${CDMS_SRC_DIR}/presenters/CUcsToDatPresenter.cpp
    ${CDMS_SRC_DIR}/presenters/CDpsCalculatorPresenter.cpp
    ${CDMS_SRC_DIR}/presenters/CRgdMacroPresenter.cpp
    ${CDMS_SRC_DIR}/presenters/CMassExtractPresenter.cpp
    ${CDMS_SRC_DIR}/presenters/CRefreshFilesPresenter.cpp
    ${CDMS_SRC_DIR}/Tool_AutoDPS.cpp
    ${CDMS_SRC_DIR}/Tool_AESetup.cpp
    ${CDMS_SRC_DIR}/CtrlStatusText.cpp
)

target_include_directories(cdms_tests PRIVATE
    ${CDMS_SRC_DIR}
    ${wxWidgets_INCLUDE_DIRS}
)

target_link_libraries(cdms_tests PRIVATE
    rainman
    GTest::gtest
    GTest::gtest_main
    ${wxWidgets_LIBRARIES}
)

target_compile_definitions(cdms_tests PRIVATE
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    UNICODE
    _UNICODE
    WIN32_LEAN_AND_MEAN
)

if(MSVC)
    target_compile_options(cdms_tests PRIVATE
        /W3
        /wd4996
        /wd4267
        /wd4244
    )
endif()

include(GoogleTest)
gtest_discover_tests(cdms_tests)
