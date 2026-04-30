#include "Exhibit.h"
#include "Guide.h"
#include "Museum.h"
#include "MuseumItem.h"
#include "PendingQueue.h"
#include "Visitor.h"
#include <clocale>
#include <iostream>
#include <memory>
#include <string>

namespace {

void printSection(const std::string& title) {
    std::cout << "\n=== " << title << " ===\n";
}

void printDeferredMessage(std::string message) {
    std::cout << "Отложенное сообщение: " << message << '\n';
}

void printShortState(const std::string& title, const Museum& museum, const Guide& guide) {
    std::cout << title << '\n';
    std::cout << "  Залов: " << museum.getHallCount()
              << ", экспонатов: " << museum.getItemCount()
              << ", остановок в маршруте: " << guide.getRouteCount() << '\n';
}

}  // namespace

int main() {
    setlocale(LC_ALL, "");

    Museum museum("Городской историко-художественный музей");

    museum.addHall(1, "Русская живопись");
    museum.addHall(2, "Скульптура");
    museum.addHall(3, "Исторические реликвии");

    std::shared_ptr<MuseumItem> painting = std::make_shared<Painting>(
        "Девочка с персиками", 1887, "Валентин Серов", 1, "масло"
    );
    std::shared_ptr<MuseumItem> sculpture = std::make_shared<Sculpture>(
        "Мыслитель", 1904, "Огюст Роден", 2, "бронза"
    );
    std::shared_ptr<MuseumItem> artifact = std::make_shared<Artifact>(
        "Шлем княжеского дружинника", 1200, "Древняя Русь", 3, "XIII век"
    );
    std::shared_ptr<MuseumItem> secondPainting = std::make_shared<Painting>(
        "Над вечным покоем", 1894, "Исаак Левитан", 1, "масло"
    );

    museum.addItem(painting);
    museum.addItem(sculpture);
    museum.addItem(artifact);
    museum.addItem(secondPainting);

    Guide guide("Анна");
    guide.addToRoute(painting.get());
    guide.addToRoute(artifact.get());
    guide.addToRoute(sculpture.get());

    Visitor visitor("Иван");
    visitor.setGuide(&guide);

    printSection("КРАТКАЯ ПРОВЕРКА ПРЕДМЕТНОЙ ОБЛАСТИ");
    std::cout << "- Область: музей, вариант 23.\n";
    std::cout << "- Базовый музей: " << museum.getHallCount()
              << " зала, " << museum.getItemCount() << " экспоната.\n";
    std::cout << "- Гид и посетитель созданы.\n";

    printSection("СЕМИНАР 6. ОЧЕРЕДЬ ОТЛОЖЕННЫХ ВЫЗОВОВ");
    Museum seminar6Museum("Учебный музей отложенных вызовов");
    Guide seminar6Guide("Мария");
    Visitor seminar6Visitor("Ольга");

    std::shared_ptr<MuseumItem> deferredPainting = std::make_shared<Painting>(
        "Утро в сосновом лесу", 1889, "Иван Шишкин", 4, "масло"
    );
    std::shared_ptr<MuseumItem> deferredArtifact = std::make_shared<Artifact>(
        "Античная амфора", -350, "Неизвестный мастер", 4, "IV век до н. э."
    );

    printShortState("[1] До выполнения очереди:", seminar6Museum, seminar6Guide);
    std::cout << "  Посетитель: гид ещё не назначен.\n";

    PendingQueue queue;
    std::cout << "\n[2] Создана PendingQueue\n";
    std::cout << "  empty(): " << (queue.empty() ? "да" : "нет")
              << ", size(): " << queue.size() << '\n';
    std::cout << "  Используются вариадические шаблоны: enqueue принимает\n"
              << "  разные callable и разное число аргументов.\n";

    std::cout << "\n[3] Добавляем отложенные вызовы через enqueue\n";
    std::cout << "  - свободная функция: printDeferredMessage(std::string)\n";
    std::cout << "  - методы Museum: addHall, addItem, showInfo\n";
    std::cout << "  - методы Guide: addToRoute\n";
    std::cout << "  - методы Visitor: setGuide, askGuideName\n";
    queue.enqueue(printDeferredMessage, std::string("свободная функция выполнена"));
    queue.enqueue(&Museum::addHall, &seminar6Museum, 4, std::string("Зал отложенных вызовов"));
    queue.enqueue(&Museum::addItem, &seminar6Museum, deferredPainting);
    queue.enqueue(&Museum::addItem, &seminar6Museum, deferredArtifact);
    queue.enqueue(&Guide::addToRoute, &seminar6Guide, deferredPainting.get());
    queue.enqueue(&Guide::addToRoute, &seminar6Guide, deferredArtifact.get());
    queue.enqueue(&Visitor::setGuide, &seminar6Visitor, &seminar6Guide);
    queue.enqueue(&Visitor::askGuideName, &seminar6Visitor);
    queue.enqueue(&Museum::showInfo, &seminar6Museum);

    std::cout << "  После enqueue: empty(): "
              << (queue.empty() ? "да" : "нет")
              << ", size(): " << queue.size() << '\n';
    printShortState("  Предметная область ещё не изменилась:", seminar6Museum, seminar6Guide);

    std::cout << "\n[4] Выполняем первые вызовы через queue.run()\n";
    queue.run();
    std::cout << "  После run() 1: size() = " << queue.size() << '\n';
    queue.run();
    std::cout << "  После run() 2: size() = " << queue.size() << '\n';
    queue.run();
    std::cout << "  После run() 3: size() = " << queue.size() << '\n';
    printShortState("  Промежуточное состояние:", seminar6Museum, seminar6Guide);

    std::cout << "\n[5] Выполняем оставшиеся вызовы через queue.runAll()\n";
    queue.runAll();
    std::cout << "  После runAll(): empty() = " << (queue.empty() ? "да" : "нет")
              << ", size() = " << queue.size() << '\n';

    printShortState("\n[6] Итоговое состояние:", seminar6Museum, seminar6Guide);
    std::cout << "  Проверка Visitor::askGuideName():\n";
    seminar6Visitor.askGuideName();

    printSection("Итог");
    std::cout << "Программа кратко проверила предметную область музея\n"
              << "и показала очередь отложенных вызовов функций и методов\n"
              << "на вариадических шаблонах.\n";

    return 0;
}
