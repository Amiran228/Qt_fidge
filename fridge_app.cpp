#include "fridge_app.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDate>
#include <QDebug>
#include <QResource>

FridgeApp::FridgeApp(QObject *parent) : QObject(parent) {
    model = new FridgeModel(this);
    QIcon icon(":/fridge.png");
    if (icon.isNull()) {
        qWarning() << "Не удалось загрузить иконку: :/fridge.png. Проверьте resources.qrc и наличие файла fridge.png";
        trayIcon = new QSystemTrayIcon(this);
    } else {
        trayIcon = new QSystemTrayIcon(icon, this);
    }
    trayIcon->setVisible(true);
    loadProducts();
    checkExpiringProducts();
}

QStringList FridgeApp::categories() const {
    return {"Молочные", "Овощи", "Фрукты", "Мясо", "Другое"};
}

QString FridgeApp::filterText() const { return filterText_; }
void FridgeApp::setFilterText(const QString &text) {
    if (filterText_ != text) {
        filterText_ = text;
        filterProducts();
        emit filterTextChanged();
    }
}

QString FridgeApp::filterCategory() const { return filterCategory_; }
void FridgeApp::setFilterCategory(const QString &category) {
    if (filterCategory_ != category) {
        filterCategory_ = category;
        filterProducts();
        emit filterCategoryChanged();
    }
}

void FridgeApp::addProduct(const QString &name, const QString &quantity, const QDate &expiry, const QString &category) {
    if (name.isEmpty() || quantity.isEmpty() || !expiry.isValid()) return;
    Product product{name, quantity, expiry, category};
    allProducts.append(product);
    model->setProducts(allProducts);
    saveProducts();
    checkExpiringProducts();
    filterProducts();
}

void FridgeApp::removeProduct(int index) {
    if (index >= 0 && index < allProducts.size()) {
        qDebug() << "Removing product at index:" << index << "Name:" << allProducts[index].name;
        allProducts.removeAt(index);
        model->setProducts(allProducts);
        saveProducts();
        checkExpiringProducts();
        filterProducts();
    } else {
        qWarning() << "Invalid index for removal:" << index;
    }
}

void FridgeApp::editProduct(int index, const QString &name, const QString &quantity, const QDate &expiry, const QString &category) {
    if (index >= 0 && index < allProducts.size() && !name.isEmpty() && !quantity.isEmpty() && expiry.isValid()) {
        allProducts[index] = {name, quantity, expiry, category};
        model->setProducts(allProducts);
        saveProducts();
        checkExpiringProducts();
        filterProducts();
    }
}

void FridgeApp::sortProducts() {
    std::sort(allProducts.begin(), allProducts.end(), [this](const Product &a, const Product &b) {
        return sortAscending ? a.expiry < b.expiry : a.expiry > b.expiry;
    });
    sortAscending = !sortAscending;
    model->setProducts(allProducts);
    filterProducts();
}

void FridgeApp::saveProducts() {
    QJsonArray jsonArray;
    for (const auto &product : allProducts) {
        QJsonObject obj;
        obj["name"] = product.name;
        obj["quantity"] = product.quantity;
        obj["expiry"] = product.expiry.toString("yyyy-MM-dd");
        obj["category"] = product.category;
        jsonArray.append(obj);
    }
    QFile file("products.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(jsonArray).toJson());
        file.close();
    }
}

void FridgeApp::loadProducts() {
    QFile file("products.json");
    if (!file.open(QIODevice::ReadOnly)) return;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    allProducts.clear();
    for (const auto &value : doc.array()) {
        auto obj = value.toObject();
        Product product{
            obj["name"].toString(),
            obj["quantity"].toString(),
            QDate::fromString(obj["expiry"].toString(), "yyyy-MM-dd"),
            obj["category"].toString()
        };
        allProducts.append(product);
    }
    model->setProducts(allProducts);
    filterProducts();
}

void FridgeApp::checkExpiringProducts() {
    QDate today = QDate::currentDate();
    for (const auto &product : allProducts) {
        int daysToExpiry = today.daysTo(product.expiry);
        if (daysToExpiry < 0) {
            QString message = QString("Продукт %1 просрочен (%2)!").arg(product.name).arg(product.expiry.toString("yyyy-MM-dd"));
            trayIcon->showMessage("Продукт просрочен", message, QSystemTrayIcon::Critical);
        } else if (daysToExpiry <= 1 && daysToExpiry >= 0) {
            QString message = QString("%1 (%2) скоро истекает!").arg(product.name).arg(product.expiry.toString("yyyy-MM-dd"));
            trayIcon->showMessage("Предупреждение о сроке годности", message, QSystemTrayIcon::Warning);
        }
    }
}

void FridgeApp::filterProducts() {
    QList<Product> filtered;
    QString searchText = filterText_.toLower();
    for (const auto &product : allProducts) {
        bool matchesName = product.name.toLower().contains(searchText);
        bool matchesCategory = filterCategory_.isEmpty() || filterCategory_ == "Все категории" || product.category == filterCategory_;
        if (matchesName && matchesCategory) {
            filtered.append(product);
        }
    }
    model->setProducts(filtered);
}
