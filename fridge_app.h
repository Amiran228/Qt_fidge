#ifndef FRIDGE_APP_H
#define FRIDGE_APP_H

#include <QObject>
#include <QSystemTrayIcon>
#include "fridge_model.h"

// Класс для управления логикой приложения "Учёт продуктов в холодильнике"
class FridgeApp : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringList categories READ categories CONSTANT) // Список категорий продуктов
    Q_PROPERTY(QString filterText READ filterText WRITE setFilterText NOTIFY filterTextChanged) // Текст для фильтрации по названию
    Q_PROPERTY(QString filterCategory READ filterCategory WRITE setFilterCategory NOTIFY filterCategoryChanged) // Категория для фильтрации

public:
    explicit FridgeApp(QObject *parent = nullptr);
    FridgeModel *getModel() { return model; }

    QStringList categories() const;
    QString filterText() const;
    void setFilterText(const QString &text);
    QString filterCategory() const;
    void setFilterCategory(const QString &category);

public slots:
    void addProduct(const QString &name, const QString &quantity, const QDate &expiry, const QString &category); // Добавить продукт
    void removeProduct(int index); // Удалить продукт
    void editProduct(int index, const QString &name, const QString &quantity, const QDate &expiry, const QString &category); // Редактировать продукт
    void sortProducts(); // Сортировать продукты по сроку годности

signals:
    void filterTextChanged();
    void filterCategoryChanged();

private:
    FridgeModel *model;
    QSystemTrayIcon *trayIcon;
    QString filterText_;
    QString filterCategory_;
    bool sortAscending = true;
    QList<Product> allProducts; // Полный список продуктов

    void saveProducts(); // Сохранить продукты в JSON
    void loadProducts(); // Загрузить продукты из JSON
    void checkExpiringProducts(); // Проверить продукты с истекающим сроком годности
    void filterProducts(); // Фильтровать продукты по названию и категории
};

#endif // FRIDGE_APP_H
