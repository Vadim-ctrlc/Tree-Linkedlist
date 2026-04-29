#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <utility>

// структура узла дерева с указателями на предыдущего брата следующего брата и первого ребенка
struct TreeNode {
    int data;
    TreeNode* prev;
    TreeNode* next;
    TreeNode* child;

    explicit TreeNode(int val) : data(val), prev(nullptr), next(nullptr), child(nullptr) {}
};

// структура для хранения экранных координат узла при отрисовке
struct NodeView {
    TreeNode* node;
    float x, y;
};

// рекурсивный поиск узла по значению в дереве
TreeNode* findNode(TreeNode* root, int val) {
    if (!root) return nullptr;
    if (root->data == val) return root;
    TreeNode* res = findNode(root->child, val);
    return res ? res : findNode(root->next, val);
}

// поиск родительского узла для заданного целевого узла
TreeNode* findParent(TreeNode* root, TreeNode* target) {
    if (!root || root == target) return nullptr;
    if (root->child == target) return root;
    TreeNode* p = findParent(root->child, target);
    return p ? p : findParent(root->next, target);
}

// проверка наличия одинакового значения среди прямых детей родителя
bool isDuplicateChild(TreeNode* parent, int val) {
    if (!parent->child) return false;
    TreeNode* curr = parent->child;
    while (curr) {
        if (curr->data == val) return true;
        curr = curr->next;
    }
    return false;
}

// добавление узла с возвратом статуса для интерфейса
std::string addNode(TreeNode*& root, TreeNode*& selected, int val) {
    if (!root) {
        root = new TreeNode(val);
        selected = root;
        return "root created";
    }
    if (!selected) return "choose node";
    if (isDuplicateChild(selected, val)) return "this number already exist";

    TreeNode* newNode = new TreeNode(val);
    newNode->next = selected->child;
    if (selected->child) selected->child->prev = newNode;
    selected->child = newNode;
    selected = newNode;
    return "added";
}

// удаление выбранного узла с сохранением структуры дерева
void deleteSelected(TreeNode*& root, TreeNode*& target) {
    if (!target) return;
    TreeNode* parent = findParent(root, target);

    if (target->prev) target->prev->next = target->next;
    if (target->next) target->next->prev = target->prev;
    if (parent && parent->child == target) parent->child = target->next;

    if (target->child) {
        TreeNode* firstC = target->child;
        TreeNode* lastC = firstC;
        while (lastC->next) lastC = lastC->next;

        if (target->prev) {
            target->prev->next = firstC;
            firstC->prev = target->prev;
        }
        else if (parent) {
            parent->child = firstC;
            firstC->prev = nullptr;
        }
        lastC->next = target->next;
        if (target->next) target->next->prev = lastC;
    }

    if (target == root) root = target->child;
    delete target;
    target = nullptr;
}

// сортировка детей каждого узла на всех уровнях дерева
void sortTree(TreeNode* root) {
    if (!root) return;
    bool swapped;
    do {
        swapped = false;
        TreeNode* curr = root;
        while (curr->next) {
            if (curr->data > curr->next->data) {
                std::swap(curr->data, curr->next->data);
                swapped = true;
            }
            curr = curr->next;
        }
    } while (swapped);

    TreeNode* curr = root;
    while (curr) {
        sortTree(curr->child);
        curr = curr->next;
    }
}

// обход дерева в выбранном режиме пре пост порядок или bfs
std::string traverseTree(TreeNode* root, int type) {
    if (!root) return "";
    std::string res;
    if (type == 0) {
        res += std::to_string(root->data) + " ";
        res += traverseTree(root->child, type);
        res += traverseTree(root->next, type);
    }
    else if (type == 1) {
        res += traverseTree(root->child, type);
        res += traverseTree(root->next, type);
        res += std::to_string(root->data) + " ";
    }
    else {
        TreeNode* buffer[1000];
        int head = 0, tail = 0;
        buffer[tail++] = root;
        while (head < tail) {
            TreeNode* curr = buffer[head++];
            res += std::to_string(curr->data) + " ";
            for (TreeNode* ch = curr->child; ch; ch = ch->next) buffer[tail++] = ch;
        }
    }
    return res;
}

// рекурсивное удаление всего дерева и освобождение памяти
void clearTree(TreeNode*& root) {
    if (!root) return;
    TreeNode* curr = root;
    while (curr) {
        clearTree(curr->child);
        TreeNode* next = curr->next;
        delete curr;
        curr = next;
    }
    root = nullptr;
}

// глобальный вектор для хранения позиций всех узлов на экране
std::vector<NodeView> layoutNodes;

// вычисление ширины поддерева для правильного расположения без наложений
float getSubtreeWidth(TreeNode* root, float radius, float gap) {
    if (!root) return 0;
    if (!root->child) return radius * 2.0f;
    float w = 0; int count = 0;
    for (TreeNode* c = root->child; c; c = c->next) {
        w += getSubtreeWidth(c, radius, gap);
        count++;
    }
    w += (count > 0 ? (count - 1) * gap : 0);
    return (w < radius * 2.0f) ? radius * 2.0f : w;
}

// рекурсивный расчет координат всех узлов дерева для отрисовки
void calculateLayout(TreeNode* root, float centerX, float y, float radius, float gap) {
    if (!root) return;
    layoutNodes.push_back({ root, centerX, y });

    if (root->child) {
        float totalW = 0; int count = 0;
        for (TreeNode* c = root->child; c; c = c->next) {
            totalW += getSubtreeWidth(c, radius, gap);
            count++;
        }
        totalW += (count > 0 ? (count - 1) * gap : 0);

        float startX = centerX - totalW / 2.0f;
        TreeNode* curr = root->child;
        while (curr) {
            float w = getSubtreeWidth(curr, radius, gap);
            calculateLayout(curr, startX + w / 2.0f, y + 80, radius, gap);
            startX += w + gap;
            curr = curr->next;
        }
    }
}

// структура поля ввода текста с фокусом и буфером
struct InputBox {
    sf::RectangleShape rect;
    sf::Text txt;
    std::string buf;
    bool focused;
};

// структура кнопки с идентификатором для обработки нажатий
struct Btn {
    sf::RectangleShape r;
    sf::Text t;
    int id;
};

// главная функция программы
int main() {
    try {
        sf::RenderWindow window(sf::VideoMode(900, 700), "Tree Manager with Zoom/Pan");
        window.setFramerateLimit(60);

        // загрузка шрифта для отрисовки текста
        sf::Font font;

        // Пробуем загрузить шрифт
        if (!font.loadFromFile("arial.ttf")) {
            // Если не получилось - пробуем из системной папки
            if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
                std::cerr << "CRITICAL ERROR: Cannot load font!\n";
                std::cerr << "Please copy arial.ttf to the program folder.\n";
                std::cin.get();
                return 1; // ВАЖНО: выходим из программы
            }
        }

        TreeNode* treeRoot = nullptr;
        TreeNode* selectedNode = nullptr;
        std::string status = "Ready. Drag background to pan, scroll to zoom.";

        // камера для просмотра с возможностью перемещения и зума
        sf::View view = window.getDefaultView();
        sf::Vector2f lastMousePos;
        bool isDragging = false;

        // поле ввода значения узла
        InputBox valBox;
        valBox.rect.setSize({ 140, 30 }); valBox.rect.setPosition(20, 20);
        valBox.rect.setFillColor(sf::Color(40, 40, 40)); valBox.rect.setOutlineThickness(2);
        valBox.rect.setOutlineColor(sf::Color::Cyan);
        valBox.txt.setFont(font); valBox.txt.setCharacterSize(18);
        valBox.txt.setFillColor(sf::Color::White);
        valBox.txt.setPosition(25, 23);
        valBox.focused = true;

        // создание кнопок интерфейса
        std::vector<Btn> btns;
        std::vector<std::string> labels = {
            "Add to Selected", "Delete Selected", "Search", "Sort Tree",
            "Traverse: Pre", "Traverse: Post", "Traverse: BFS", "Clear All","Center on Root"
        };
        float bx = 20, by = 60;
        for (size_t i = 0; i < labels.size(); ++i) {
            Btn b; b.id = i;
            b.r.setSize({ 150, 35 }); b.r.setPosition(bx, by);
            b.r.setFillColor(sf::Color(50, 50, 50));
            b.r.setOutlineColor(sf::Color::Red); b.r.setOutlineThickness(2);
            b.t.setFont(font); b.t.setString(labels[i]); b.t.setCharacterSize(12); b.t.setFillColor(sf::Color::White);
            sf::FloatRect tb = b.t.getLocalBounds();
            b.t.setPosition(bx + (150 - tb.width) / 2, by + (35 - tb.height) / 2 - 2);
            btns.push_back(b);
            bx += 160;
            if (bx + 160 > 880) { bx = 20; by += 40; }
        }

        sf::Clock blinkTimer;
        bool blink = true;
        bool isnodecreated = false;
        // главный цикл обработки событий и отрисовки
        while (window.isOpen()) {
            sf::Event ev;
            while (window.pollEvent(ev)) {
                if (ev.type == sf::Event::Closed) window.close();

                // обработка прокрутки колеса мыши для зума
                if (ev.type == sf::Event::MouseWheelScrolled) {
                    if (ev.mouseWheelScroll.delta > 0) view.zoom(0.95f);
                    else view.zoom(1.05f);
                }

                // начало перетаскивания камеры при нажатии на фон
                if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f m(ev.mouseButton.x, ev.mouseButton.y);
                    bool onUI = valBox.rect.getGlobalBounds().contains(m);
                    for (const auto& b : btns) if (b.r.getGlobalBounds().contains(m)) onUI = true;
                    if (!onUI) { isDragging = true; lastMousePos = m; }
                }
                // завершение перетаскивания
                if (ev.type == sf::Event::MouseButtonReleased && ev.mouseButton.button == sf::Mouse::Left) {
                    isDragging = false;
                }
                // перемещение камеры при движении мыши с зажатой кнопкой
                if (ev.type == sf::Event::MouseMoved && isDragging) {
                    sf::Vector2f m(ev.mouseMove.x, ev.mouseMove.y);
                    view.move(lastMousePos - m);
                    lastMousePos = m;
                }

                // обработка кликов мыши по узлам и кнопкам
                if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePixel(ev.mouseButton.x, ev.mouseButton.y);
                    // преобразование экранных координат в координаты мира с учетом зума
                    sf::Vector2f worldPos = window.mapPixelToCoords(mousePixel, view);

                    valBox.focused = valBox.rect.getGlobalBounds().contains(ev.mouseButton.x, ev.mouseButton.y);
                    valBox.rect.setOutlineColor(valBox.focused ? sf::Color::Cyan : sf::Color::Red);

                    // проверка клика по узлам дерева
                    bool clickedNode = false;
                    for (int i = layoutNodes.size() - 1; i >= 0; --i) {
                        float dx = worldPos.x - layoutNodes[i].x;
                        float dy = worldPos.y - layoutNodes[i].y;
                        if (dx * dx + dy * dy <= 400) {
                            selectedNode = layoutNodes[i].node;
                            status = "Selected: " + std::to_string(selectedNode->data);
                            clickedNode = true;
                            break;
                        }
                    }

                    // проверка клика по кнопкам интерфейса
                    for (auto& b : btns) {
                        if (b.r.getGlobalBounds().contains(ev.mouseButton.x, ev.mouseButton.y)) {
                            int v = 0; bool validV = false;
                            try { if (!valBox.buf.empty()) { v = std::stoi(valBox.buf); validV = true; } }
                            catch (...) {}

                            switch (b.id) {
                            case 0: if (validV) { status = addNode(treeRoot, selectedNode, v); valBox.buf = ""; }
                                  else status = "введите число"; isnodecreated = true; break;
                            case 1: if (selectedNode) { deleteSelected(treeRoot, selectedNode); status = "Deleted"; } break;
                            case 2: if (validV) { selectedNode = findNode(treeRoot, v); status = selectedNode ? "Found" : "Not found"; valBox.buf = ""; } break;
                            case 3: sortTree(treeRoot); status = "Sorted"; break;
                            case 4: case 5: case 6: status = "Result: " + traverseTree(treeRoot, b.id - 4); break;
                            case 7: clearTree(treeRoot); selectedNode = nullptr; valBox.buf = ""; status = "Cleared"; break;
                            case 8: view.setCenter(20, 40);
                            }
                            break;
                        }
                    }
                }

                // обработка ввода текста в поле
                if (ev.type == sf::Event::TextEntered && ev.text.unicode < 128) {
                    char c = static_cast<char>(ev.text.unicode);
                    if (std::isdigit(c) || (c == '-' && valBox.buf.empty() && valBox.focused)) {
                        if (valBox.buf.length() < 6) valBox.buf += c;
                    }
                }
                // обработка удаления символов
                if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Backspace) {
                    if (!valBox.buf.empty()) valBox.buf.pop_back();
                }
            }

            // очистка окна перед отрисовкой
            window.clear(sf::Color(30, 30, 30));

            // отрисовка дерева с применением камеры
            window.setView(view);
            layoutNodes.clear();
            if (treeRoot) calculateLayout(treeRoot, 0, 0, 20, 40);
            // центрирование камеры на выбранной ноде
            if (isnodecreated && selectedNode) {
                for (const auto& lv : layoutNodes) {
                    if (lv.node == selectedNode) {
                        view.setCenter(lv.x, lv.y);
                        break;
                    }
                }
                isnodecreated = false; // сбрасываем флаг
            }
            // отрисовка линий между родителями и детьми
            for (const auto& lv : layoutNodes) {
                if (lv.node->child) {
                    TreeNode* curr = lv.node->child;
                    while (curr) {
                        for (const auto& clv : layoutNodes) {
                            if (clv.node == curr) {
                                sf::Vertex line[] = {
                                    sf::Vertex(sf::Vector2f(lv.x, lv.y + 20), sf::Color(150, 150, 150)),
                                    sf::Vertex(sf::Vector2f(clv.x, clv.y - 20), sf::Color(150, 150, 150))
                                };
                                window.draw(line, 2, sf::Lines);
                                break;
                            }
                        }
                        curr = curr->next;
                    }
                }
            }
            // отрисовка узлов дерева кружками с текстом
            for (const auto& lv : layoutNodes) {
                sf::Color fillColor = (lv.node == selectedNode) ? sf::Color(50, 200, 50) : sf::Color(60, 120, 180);
                sf::CircleShape c(20);
                c.setFillColor(fillColor); c.setOutlineColor(sf::Color::White); c.setOutlineThickness(2);
                c.setPosition(lv.x - 20, lv.y - 20);
                window.draw(c);
                sf::Text txt; txt.setFont(font); txt.setString(std::to_string(lv.node->data)); txt.setCharacterSize(16); txt.setFillColor(sf::Color::White);
                sf::FloatRect b = txt.getLocalBounds();
                txt.setPosition(lv.x - b.width / 2, lv.y - 8);
                window.draw(txt);
                
            }

            // возврат к стандартной камере для отрисовки интерфейса
            window.setView(window.getDefaultView());

            // отрисовка подписи поля ввода
            sf::Text lbl; lbl.setFont(font); lbl.setCharacterSize(12); lbl.setFillColor(sf::Color(200, 200, 200));
            lbl.setString("Node Value"); lbl.setPosition(20, 5);
            window.draw(lbl); window.draw(valBox.rect);
            // мигание курсора в поле ввода
            if (blinkTimer.getElapsedTime().asSeconds() > 0.5f) { blink = !blink; blinkTimer.restart(); }
            valBox.txt.setString(valBox.buf + (valBox.focused && blink ? "_" : ""));
            window.draw(valBox.txt);

            // отрисовка статуса операций
            sf::Text out; out.setFont(font); out.setCharacterSize(14); out.setFillColor(sf::Color::Yellow);
            out.setPosition(250, 20); out.setString(status);
            window.draw(out);

            // отрисовка всех кнопок
            for (auto& b : btns) { window.draw(b.r); window.draw(b.t); }

            window.display();
        }

        // очистка памяти дерева при завершении программы
        clearTree(treeRoot);
    }
    catch (const std::exception& e) {
        std::cerr << "FATAL: " << e.what() << "\n"; std::cin.get(); return 1;
    }
    return 0;
}