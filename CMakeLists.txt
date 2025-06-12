cmake_minimum_required(VERSION 3.16)

project(Fridge VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Core Qml Gui Widgets)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appFridge
    main.cpp
)

qt_add_qml_module(appFridge
    URI Fridge
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES fridge_model.h fridge_model.cpp
        SOURCES fridge_app.h fridge_app.cpp
)
qt_add_resources(appFridge "resources"
    PREFIX "/"
    FILES
        fridge.png
        delete_icon.png
        Main.qml
        background.png
)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appFridge PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appFridge
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appFridge
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Widgets
    PRIVATE Qt6::Core
    PRIVATE Qt6::Qml
    PRIVATE Qt6::Gui
)

include(GNUInstallDirs)
install(TARGETS appFridge
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
