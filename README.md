# Tree-Linkedlist
Курсовая работа

Весь код в файле main.cpp собирается при наличии библиотеки SFML x64 для Visual studio 2022. Для работы необходим шрифт arialmt

## Требования

Для сборки проекта понадобятся:
* **CMake** (версия 3.16 или выше)
* **Компилятор C++17** (Visual Studio на Windows или GCC на Linux)
* **Git**

---

## Инструкция по сборке


### Windows

1. Откройте командную строку (cmd) или PowerShell в папке проекта:
2. Создайте и перейдите в папку build:
```cmd
mkdir build
cd build
```
3. Настройте CMake:
```cmd
cmake .. -G "Visual Studio 17 2022" -A x64
```
4. Соберите приложение:
```cmd
cmake --build . --config Release
```
5. Запустите приложение:
```cmd
cd Release
.\TreeLinkedlist.exe
```


### Linux (Ubuntu/Debian)

1. Установите необходимые зависимости:
```cmd
sudo apt update & sudo apt install -y build-essential cmake git \
    libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev \
    libgl1-mesa-dev libfreetype6-dev libopenal-dev \
    libflac-dev libvorbis-dev libudev-dev
```
2. Перейдите в папку проекта:
```cmd
cd /путь/к/Tree-Linkedlist
```
3. Создайте папку build:
```cmd
mkdir build && cd build
```
4. Настройте CMake:
```cmd
cmake .. -DCMAKE_BUILD_TYPE=Release
```
5. Соберите приложение:
```cmd
cmake --build . -j$(nproc)
```
6. Запустите приложение:
```cmd
./TreeLinkedlist
```



---

## Инструкция пользователя:
поле подвижно, левой кнопкой мышки его можно перемещать. колесиком увеличивать и уменьшать
Кнопка вернуться к корню если поле отодвинуто слишком далеко. так же возврат при добавлении новых элементов.
Для добавления первого элемента введите будующее значение в поле ввода и нажмите кнопку добавления. В дальнейшем
для добавления нужно ввести значение, выбрать нужную ноду и нажать на кнопку.

 Обходы дерева доступны в трех вариантах: обход прямой (pre) обратный (post) и поуровневый (bfs)
