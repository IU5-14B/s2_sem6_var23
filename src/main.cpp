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
    std::cout << "Предметная область варианта 23: музей.\n";
    std::cout << "Базовый музей создан успешно.\n";
    std::cout << "Количество залов: " << museum.getHallCount() << '\n';
    std::cout << "Количество экспонатов: " << museum.getItemCount() << '\n';
    std::cout << "Гид и посетитель созданы.\n";
    std::cout << "Старые классы предметной области используются в демонстрации семинара 6.\n";

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

    std::cout << "Начальное состояние до создания и выполнения очереди:\n";
    seminar6Museum.showInfo();
    seminar6Museum.showHalls();
    seminar6Museum.showCollection();
    seminar6Guide.showRoute();
    seminar6Visitor.askGuideName();

    PendingQueue queue;
    std::cout << "\nСоздана очередь PendingQueue. Пустая ли очередь: "
              << (queue.empty() ? "да" : "нет")
              << ", размер: " << queue.size() << '\n';
    std::cout << "PendingQueue использует вариадические шаблоны,\n"
              << "поэтому enqueue принимает разные callable и разное число аргументов.\n";

    std::cout << "Добавляем отложенные вызовы функции и методов классов музея.\n";
    queue.enqueue(printDeferredMessage, std::string("начинаем демонстрацию семинара 6"));
    queue.enqueue(&Museum::addHall, &seminar6Museum, 4, std::string("Зал отложенных открытий"));
    queue.enqueue(&Museum::addItem, &seminar6Museum, deferredPainting);
    queue.enqueue(&Museum::addItem, &seminar6Museum, deferredArtifact);
    queue.enqueue(&Guide::addToRoute, &seminar6Guide, deferredPainting.get());
    queue.enqueue(&Guide::addToRoute, &seminar6Guide, deferredArtifact.get());
    queue.enqueue(&Visitor::setGuide, &seminar6Visitor, &seminar6Guide);
    queue.enqueue(&Visitor::askGuideName, &seminar6Visitor);
    queue.enqueue(&Guide::showRoute, &seminar6Guide);
    queue.enqueue(&Museum::showInfo, &seminar6Museum);
    queue.enqueue(&Museum::showHalls, &seminar6Museum);
    queue.enqueue(&Museum::showCollection, &seminar6Museum);

    std::cout << "Все вызовы только добавлены. Размер очереди: " << queue.size() << '\n';
    std::cout << "Проверяем, что предметная область ещё не изменилась:\n";
    seminar6Museum.showInfo();
    seminar6Guide.showRoute();
    seminar6Visitor.askGuideName();

    std::cout << "\nВыполняем первые вызовы по одному через queue.run():\n";
    queue.run();
    std::cout << "Размер очереди после первого run(): " << queue.size() << '\n';
    queue.run();
    std::cout << "Размер очереди после второго run(): " << queue.size() << '\n';
    queue.run();
    std::cout << "Размер очереди после третьего run(): " << queue.size() << '\n';

    std::cout << "Промежуточное состояние после трёх запусков:\n";
    seminar6Museum.showInfo();
    seminar6Museum.showHalls();
    seminar6Museum.showCollection();
    seminar6Guide.showRoute();
    seminar6Visitor.askGuideName();

    std::cout << "\nВыполняем оставшиеся вызовы через queue.runAll().\n";
    queue.runAll();
    std::cout << "После runAll() очередь пуста: "
              << (queue.empty() ? "да" : "нет")
              << ", размер: " << queue.size() << '\n';

    std::cout << "\nИтоговое состояние после выполнения всей очереди:\n";
    seminar6Museum.showInfo();
    seminar6Museum.showHalls();
    seminar6Museum.showCollection();
    seminar6Guide.showRoute();
    seminar6Visitor.askGuideName();

    printSection("Итог");
    std::cout << "Программа кратко проверила предметную область музея\n"
              << "и показала очередь отложенных вызовов функций и методов\n"
              << "на вариадических шаблонах.\n";

    return 0;
}
