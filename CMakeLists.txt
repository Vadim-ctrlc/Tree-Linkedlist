cmake_minimum_required(VERSION 3.10)
project(LabZ8sem Tree-Linkedlist)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Устанавливаем имя исполняемого файла
set(EXECUTABLE_NAME "Tree-Linkedlist")

# Создаем исполняемый файл из main.cpp
add_executable(${EXECUTABLE_NAME} main.cpp)

# Определяем путь к папке с библиотеками Windows
set(WINDOWS_LIBS_DIR "${CMAKE_SOURCE_DIR}/Windows")

# Добавляем пути поиска библиотек
link_directories(${WINDOWS_LIBS_DIR})

# Добавляем пути поиска заголовочных файлов (если есть в папке Windows)
# Если SFML установлен отдельно, раскомментируйте и укажите правильный путь
# include_directories("C:/SFML/include")

# Определяем библиотеки SFML (debug версии, судя по именам файлов)
set(SFML_LIBRARIES
    sfml-graphics-d-2
    sfml-window-d-2
    sfml-system-d-2
)

# Связываем библиотеки с проектом
target_link_libraries(${EXECUTABLE_NAME}
    ${SFML_LIBRARIES}
)

# Добавляем определения для использования debug версий SFML
target_compile_definitions(${EXECUTABLE_NAME} PRIVATE SFML_STATIC)

# Копируем все необходимые DLL в папку с исполняемым файлом
# Создаем список DLL файлов для копирования
set(DLL_FILES
    ${WINDOWS_LIBS_DIR}/sfml-graphics-d-2.dll
    ${WINDOWS_LIBS_DIR}/sfml-window-d-2.dll
    ${WINDOWS_LIBS_DIR}/sfml-system-d-2.dll
    ${WINDOWS_LIBS_DIR}/msvcp140.dll
    ${WINDOWS_LIBS_DIR}/msvcp140d.dll
    ${WINDOWS_LIBS_DIR}/vcruntime140.dll
    ${WINDOWS_LIBS_DIR}/vcruntime140d.dll
    ${WINDOWS_LIBS_DIR}/vcruntime140_1d.dll
    ${WINDOWS_LIBS_DIR}/ucrtbased.dll
    ${WINDOWS_LIBS_DIR}/arialmt
)

# Копируем DLL после сборки
add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>
    COMMAND ${CMAKE_COMMAND} -E echo "Copying DLL files..."
)

# Добавляем команды копирования для каждого файла
foreach(DLL_FILE ${DLL_FILES})
    if(EXISTS ${DLL_FILE})
        add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${DLL_FILE}
                $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>
        )
    endif()
endforeach()

# Выводим информацию о сборке
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Executable will be created in: ${CMAKE_BINARY_DIR}")