cmake_minimum_required(VERSION 3.16)

project(Fridge VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Подключение необходимых компонентов Qt6
find_package(Qt6 REQUIRED COMPONENTS Quick Core Qml Gui Widgets Test)

qt_standard_project_setup(REQUIRES 6.5)

# Основное приложение
qt_add_executable(appFridge
    main.cpp
)

qt_add_qml_module(appFridge
    URI Fridge
    VERSION 1.0
    QML_FILES
        Main.qml
    SOURCES
        fridge_model.h fridge_model.cpp
        fridge_app.h fridge_app.cpp
)

qt_add_resources(appFridge "resources"
    PREFIX "/"
    FILES
        fridge.png
        delete_icon.png
        Main.qml
        background.png
)

set_target_properties(appFridge PROPERTIES
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

# Тесты
enable_testing()

qt_add_executable(testFridge
    tst_fridgeapp.cpp
    fridge_app.cpp
    fridge_model.cpp
)

# Линковка библиотек для тестов
target_link_libraries(testFridge
    PRIVATE Qt6::Test
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Widgets # Добавлен для QSystemTrayIcon
    PRIVATE Qt6::Core
    PRIVATE Qt6::Qml
    PRIVATE Qt6::Gui
)

# Добавление теста
add_test(NAME FridgeAppTest COMMAND testFridge)

include(GNUInstallDirs)
install(TARGETS appFridge
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

