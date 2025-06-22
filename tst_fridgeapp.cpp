#include <QtTest>
#include "fridge_app.h"
#include "fridge_model.h"

// Тестовый класс для FridgeApp
class TestFridgeApp : public QObject {
    Q_OBJECT

private:
    FridgeApp *app;

private slots:
    void initTestCase(); // Инициализация перед всеми тестами
    void cleanupTestCase(); // Очистка после всех тестов
    void init(); // Инициализация перед каждым тестом
    void cleanup(); // Очистка после каждого теста

    void testAddProduct(); // Тест добавления продукта
    void testRemoveProduct(); // Тест удаления продукта
    void testEditProduct(); // Тест редактирования продукта
    void testFilterProducts(); // Тест фильтрации
    void testSortProducts(); // Тест сортировки
};

void TestFridgeApp::initTestCase() {
    // Инициализация ресурсов, если нужно
}

void TestFridgeApp::cleanupTestCase() {
    // Очистка ресурсов, если нужно
}

void TestFridgeApp::init() {
    // Создаём новый экземпляр FridgeApp перед каждым тестом
    app = new FridgeApp();
}

void TestFridgeApp::cleanup() {
    // Удаляем экземпляр после каждого теста
    delete app;
    // Удаляем тестовый JSON-файл, если он был создан
    QFile::remove("products.json");
}

void TestFridgeApp::testAddProduct() {
    // Проверяем добавление продукта
    QDate expiry = QDate::currentDate().addDays(5);
    app->addProduct("Молоко", "1 литр", expiry, "Молочные");

    // Проверяем, что продукт добавлен в модель
    FridgeModel *model = app->getModel();
    QCOMPARE(model->rowCount(), 1);

    // Проверяем данные продукта
    QVariantMap product = model->get(0);
    QCOMPARE(product["name"].toString(), QString("Молоко"));
    QCOMPARE(product["quantity"].toString(), QString("1 литр"));
    QCOMPARE(product["expiry"].toString(), expiry.toString("yyyy-MM-dd"));
    QCOMPARE(product["category"].toString(), QString("Молочные"));

    // Проверяем, что пустой продукт не добавляется
    app->addProduct("", "1 кг", expiry, "Овощи");
    QCOMPARE(model->rowCount(), 1); // Количество не изменилось
}

void TestFridgeApp::testRemoveProduct() {
    // Добавляем продукт для удаления
    QDate expiry = QDate::currentDate().addDays(5);
    app->addProduct("Молоко", "1 литр", expiry, "Молочные");

    // Удаляем продукт
    app->removeProduct(0);
    QCOMPARE(app->getModel()->rowCount(), 0);

    // Проверяем удаление по неверному индексу
    app->removeProduct(999); // Ничего не должно сломаться
    QCOMPARE(app->getModel()->rowCount(), 0);
}

void TestFridgeApp::testEditProduct() {
    // Добавляем продукт
    QDate expiry = QDate::currentDate().addDays(5);
    app->addProduct("Молоко", "1 литр", expiry, "Молочные");

    // Редактируем продукт
    QDate newExpiry = QDate::currentDate().addDays(10);
    app->editProduct(0, "Сыр", "500 г", newExpiry, "Молочные");

    // Проверяем изменения
    QVariantMap product = app->getModel()->get(0);
    QCOMPARE(product["name"].toString(), QString("Сыр"));
    QCOMPARE(product["quantity"].toString(), QString("500 г"));
    QCOMPARE(product["expiry"].toString(), newExpiry.toString("yyyy-MM-dd"));
    QCOMPARE(product["category"].toString(), QString("Молочные"));

    // Проверяем, что редактирование с неверными данными не проходит
    app->editProduct(0, "", "1 кг", newExpiry, "Овощи");
    product = app->getModel()->get(0);
    QCOMPARE(product["name"].toString(), QString("Сыр")); // Имя не изменилось
}

void TestFridgeApp::testFilterProducts() {
    // Добавляем несколько продуктов
    QDate expiry = QDate::currentDate().addDays(5);
    app->addProduct("Молоко", "1 литр", expiry, "Молочные");
    app->addProduct("Яблоки", "2 кг", expiry, "Фрукты");
    app->addProduct("Сыр", "500 г", expiry, "Молочные");

    // Фильтрация по категории
    app->setFilterCategory("Молочные");
    QCOMPARE(app->getModel()->rowCount(), 2);

    // Фильтрация по тексту
    app->setFilterText("Мол");
    QCOMPARE(app->getModel()->rowCount(), 1);
    QVariantMap product = app->getModel()->get(0);
    QCOMPARE(product["name"].toString(), QString("Молоко"));

    // Сбрасываем фильтры
    app->setFilterCategory("Все категории");
    app->setFilterText("");
    QCOMPARE(app->getModel()->rowCount(), 3);
}

void TestFridgeApp::testSortProducts() {
    // Добавляем продукты с разными сроками годности
    QDate expiry1 = QDate::currentDate().addDays(5);
    QDate expiry2 = QDate::currentDate().addDays(2);
    QDate expiry3 = QDate::currentDate().addDays(10);
    app->addProduct("Молоко", "1 литр", expiry1, "Молочные");
    app->addProduct("Яблоки", "2 кг", expiry2, "Фрукты");
    app->addProduct("Сыр", "500 г", expiry3, "Молочные");

    // Сортировка по возрастанию
    app->sortProducts();
    QVariantMap product1 = app->getModel()->get(0);
    QCOMPARE(product1["name"].toString(), QString("Яблоки")); // Самый ранний срок

    // Сортировка по убыванию
    app->sortProducts();
    product1 = app->getModel()->get(0);
    QCOMPARE(product1["name"].toString(), QString("Сыр")); // Самый поздний срок
}

QTEST_MAIN(TestFridgeApp)
#include "tst_fridgeapp.moc"
