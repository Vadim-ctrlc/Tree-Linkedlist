cmake_minimum_required(VERSION 3.21)
project(TreeLinkedlist LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Оптимизации для Release
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-O2)
    endif()
endif()

# Загрузка SFML 2.6.1
include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
    GIT_SHALLOW TRUE
)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SFML)

# Создаем приложение
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE 
    sfml-graphics 
    sfml-window 
    sfml-system
)

# Пути к файлам
set(OUTPUT_DIR $<TARGET_FILE_DIR:${PROJECT_NAME}>)
set(FONT_SRC "${CMAKE_SOURCE_DIR}/assets/arialmt.ttf")
set(FONT_SRC2 "${CMAKE_SOURCE_DIR}/assets/arial.ttf")

# Проверяем существование шрифта
if(NOT EXISTS "${FONT_SRC}")
    message(FATAL_ERROR "Font file not found: ${FONT_SRC}")
endif()
message(STATUS "Font found: ${FONT_SRC}")

# Копируем шрифт в папку сборки
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Copying font to: ${OUTPUT_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${FONT_SRC}"
        "${OUTPUT_DIR}/arialmt.ttf"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${FONT_SRC2}"
        "${OUTPUT_DIR}/arial.ttf"
    COMMENT "Copying fonts"
)

# Windows: копируем DLL
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Copying DLLs..."
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
            "${OUTPUT_DIR}"
        COMMAND_EXPAND_LISTS
        COMMENT "Copying runtime DLLs"
    )
endif()

# Linux: копируем .so и настраиваем RPATH
if(UNIX AND NOT APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        BUILD_RPATH "$ORIGIN"
        INSTALL_RPATH "$ORIGIN"
        SKIP_BUILD_RPATH FALSE
        BUILD_WITH_INSTALL_RPATH FALSE
    )
    
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Copying SFML libraries..."
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:sfml-graphics>
            $<TARGET_FILE:sfml-window>
            $<TARGET_FILE:sfml-system>
            "${OUTPUT_DIR}"
        COMMENT "Copying SFML .so files"
    )
endif()

message(STATUS "========================================")
message(STATUS "Output directory: ${OUTPUT_DIR}")
message(STATUS "========================================")
