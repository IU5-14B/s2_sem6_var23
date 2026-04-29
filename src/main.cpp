#include "Exhibit.h"
#include "Guide.h"
#include "Museum.h"
#include "MuseumItem.h"
#include "PendingQueue.h"
#include "Visitor.h"
#include <algorithm>
#include <clocale>
#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>

namespace {

void printSection(const std::string& title) {
    std::cout << "\n=== " << title << " ===\n";
}

void printDeferredMessage(std::string message) {
    std::cout << "Отложенное сообщение: " << message << '\n';
}

void showItemThroughBaseInterface(const MuseumItem& item) {
    item.printInfo();
    std::cout << "Рекомендуемое время осмотра: "
              << item.getRecommendedVisitMinutes() << " мин.\n";
}

void showRuntimeType(const MuseumItem& item) {
    std::cout << "Результат typeid: ";

    if (typeid(item) == typeid(Painting)) {
        std::cout << "объект является картиной.\n";
    } else if (typeid(item) == typeid(Sculpture)) {
        std::cout << "объект является скульптурой.\n";
    } else if (typeid(item) == typeid(Artifact)) {
        std::cout << "объект является артефактом.\n";
    } else {
        std::cout << "тип объекта определить не удалось.\n";
    }
}

void showSpecialDetails(const MuseumItem* item) {
    if (const Painting* painting = dynamic_cast<const Painting*>(item)) {
        std::cout << "dynamic_cast дал доступ к специфичному полю картины: техника = "
                  << painting->getTechnique() << ".\n";
        return;
    }

    if (const Sculpture* sculpture = dynamic_cast<const Sculpture*>(item)) {
        std::cout << "dynamic_cast дал доступ к специфичному полю скульптуры: материал = "
                  << sculpture->getMaterial() << ".\n";
        return;
    }

    if (const Artifact* artifact = dynamic_cast<const Artifact*>(item)) {
        std::cout << "dynamic_cast дал доступ к специфичному полю артефакта: эпоха = "
                  << artifact->getEpoch() << ".\n";
        return;
    }

    std::cout << "dynamic_cast не выявил специальный тип объекта.\n";
}

void showRouteItems(const MyContainer<std::shared_ptr<MuseumItem>>& route) {
    if (route.empty()) {
        std::cout << "Подходящих экспонатов для маршрута не найдено.\n";
        return;
    }

    for (const auto& item : route) {
        item->printInfo();
    }
}

void showItemsWithManualIterator(const MyContainer<std::shared_ptr<MuseumItem>>& items) {
    if (items.empty()) {
        std::cout << "Коллекция пуста, итератор показать не на чем.\n";
        return;
    }

    for (auto it = items.begin(); it != items.end(); ++it) {
        std::cout << "Элемент через ручной итератор:\n";
        (*it)->printInfo();
    }
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

    printSection("Информация о музее");
    museum.showInfo();

    printSection("Залы музея");
    museum.showHalls();

    printSection("Коллекция экспонатов");
    museum.showCollection();

    Hall requestedHall(1, "Русская живопись");

    printSection("Полиморфный вывод");
    std::cout << "Ниже каждый объект обрабатывается через базовый интерфейс MuseumItem:\n";
    for (const auto& item : museum.getItems()) {
        showItemThroughBaseInterface(*item);
        std::cout << '\n';
    }

    printSection("Ручной обход коллекции итератором");
    std::cout << "Коллекция музея обходится через MyContainer::begin() / end()\n"
              << "и явный цикл for (auto it = begin(); it != end(); ++it).\n";
    showItemsWithManualIterator(museum.getItems());

    printSection("План осмотра");
    museum.showVisitPlan();

    printSection("Поиск зала через std::find");
    const Hall* foundHall = museum.findHall(requestedHall);
    if (foundHall != nullptr) {
        std::cout << "Зал найден в MyContainer<Hall>: ";
        foundHall->printInfo();
    } else {
        std::cout << "Запрошенный зал не найден.\n";
    }

    printSection("Прикладная задача семинара 5");
    std::cout << "Собираем короткий маршрут по залу \"" << requestedHall.getName()
              << "\" с ограничением до 15 минут на экспонат.\n"
              << "Поиск зала выполняется через std::find, а найденные экспонаты\n"
              << "сортируются в MyContainer по году через std::sort.\n";
    MyContainer<std::shared_ptr<MuseumItem>> shortRoute =
        museum.buildShortRouteForHall(requestedHall, 15);
    std::cout << "До сортировки:\n";
    showRouteItems(shortRoute);

    std::sort(shortRoute.begin(), shortRoute.end(), [](const auto& left, const auto& right) {
        if (left->getYear() != right->getYear()) {
            return left->getYear() < right->getYear();
        }

        return left->getTitle() < right->getTitle();
    });

    std::cout << "После сортировки по году и названию:\n";
    showRouteItems(shortRoute);

    if (!shortRoute.empty()) {
        std::cout << "Первая остановка маршрута через operator[]:\n";
        shortRoute[0]->printInfo();
    }

    MyContainer<std::shared_ptr<MuseumItem>> shortenedRoute = shortRoute;
    shortenedRoute.pop_back();
    std::cout << "После pop_back в сокращённом маршруте осталось остановок: "
              << shortenedRoute.size() << '\n';

    printSection("Маршрут экскурсии");
    std::cout << "Количество остановок в маршруте гида: " << guide.getRouteCount() << '\n';
    guide.showRoute();

    printSection("Работа гида и посетителя");
    visitor.askGuideName();

    printSection("Поиск через каталог");
    std::cout << "Музей делегирует поиск экспоната объекту Catalog.\n";
    const MuseumItem* foundItem = museum.findItem("Мыслитель");
    if (foundItem != nullptr) {
        std::cout << "Поиск завершён успешно. Найден объект:\n";
        foundItem->printInfo();
    } else {
        std::cout << "Экспонат не найден.\n";
    }

    printSection("RTTI и dynamic_cast");
    if (foundItem != nullptr) {
        std::cout << "Дополнительная проверка типа нужна только для доступа\n"
                  << "к данным, которых нет в базовом интерфейсе.\n";
        showRuntimeType(*foundItem);
        showSpecialDetails(foundItem);
    } else {
        std::cout << "RTTI не демонстрируется, потому что объект для проверки не найден.\n";
    }

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
    std::cout << "Программа показала композицию, агрегацию, ассоциацию,\n"
              << "делегирование, полиморфную работу с музейными объектами\n"
              << "и очередь отложенных вызовов на вариадических шаблонах.\n";

    return 0;
}
