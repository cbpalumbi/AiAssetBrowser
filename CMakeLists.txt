cmake_minimum_required(VERSION 3.16)

project(AssetBrowser VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
    LinguistTools
    Core
    Gui
    Quick
    QuickControls2
    Quick3D
    QuickWidgets
    Widgets
)

set(TS_FILES AssetBrowser_en_US.ts)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ModelPreviewWidget.h
    ${TS_FILES}
)

qt6_add_qml_module(AssetBrowserQml
    URI AssetBrowser
    VERSION 1.0
    QML_FILES
        ModelViewer.qml
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(AssetBrowser
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ModelPreviewWidget.cpp
        AssetBrowserPanel.cpp
        AssetBrowserPanel.h
        TagEditorPanel.cpp
        TagEditorPanel.h
        MetadataUtils.h
        MetadataUtils.cpp
        HistoryUtils.h HistoryUtils.cpp
    )

    # ✅ Link the QML resources to the executable
    qt_add_resources(RESOURCES resources.qrc)
    target_sources(AssetBrowser PRIVATE ${RESOURCES})

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(AssetBrowser SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(AssetBrowser
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(AssetBrowser PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::QuickControls2
    Qt${QT_VERSION_MAJOR}::Quick3D
    Qt${QT_VERSION_MAJOR}::QuickWidgets
    Qt${QT_VERSION_MAJOR}::Widgets
)

# macOS/iOS app metadata
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.AssetBrowser)
endif()

set_target_properties(AssetBrowser PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS AssetBrowser
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

message(STATUS "QML path = ${CMAKE_CURRENT_SOURCE_DIR}/ModelViewer.qml")

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(AssetBrowser)
endif()
