cmake_minimum_required(VERSION 3.16)
project(Tree-Linkedlist VERSION 1.0 LANGUAGES CXX)

# Стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Оптимизации для Release
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-O3 -march=native)
    endif()
endif()

# Поиск SFML
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} main.cpp)

# Подключаем SFML
target_link_libraries(${PROJECT_NAME} PRIVATE 
    sfml-graphics
    sfml-window
    sfml-system
)

# Linux специфичные настройки
if(UNIX AND NOT APPLE)
    # Устанавливаем RPATH для поиска библиотек
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INSTALL_RPATH "$ORIGIN/lib"
        BUILD_WITH_INSTALL_RPATH FALSE
    )
    
    # Копируем шрифт если существует
    if(EXISTS "${CMAKE_SOURCE_DIR}/arialmt.ttf")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CMAKE_SOURCE_DIR}/arialmt.ttf"
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    endif()
    
    # Создаем скрипт запуска
    configure_file(
        "${CMAKE_SOURCE_DIR}/run_linux.sh.in"
        "${CMAKE_BINARY_DIR}/run.sh"
        @ONLY
    )
    execute_process(COMMAND chmod +x "${CMAKE_BINARY_DIR}/run.sh")
endif()

# Windows специфичные настройки
if(WIN32)
    # Копируем шрифт
    if(EXISTS "${CMAKE_SOURCE_DIR}/arialmt.ttf")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CMAKE_SOURCE_DIR}/arialmt.ttf"
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    endif()
    
    # Копируем DLL
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
    )
endif()

# Вывод информации
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "System: ${CMAKE_SYSTEM_NAME}")
message(STATUS "SFML found: ${SFML_FOUND}")
message(STATUS "Executable: ${CMAKE_BINARY_DIR}/${PROJECT_NAME}")
