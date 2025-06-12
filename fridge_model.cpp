#include "fridge_model.h"

FridgeModel::FridgeModel(QObject *parent) : QAbstractListModel(parent) {}

int FridgeModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : products.size();
}

QVariant FridgeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= products.size()) return QVariant();
    const Product &product = products[index.row()];
    switch (role) {
    case NameRole: return product.name;
    case QuantityRole: return product.quantity;
    case ExpiryRole: return product.expiry.toString("yyyy-MM-dd");
    case CategoryRole: return product.category;
    default: return QVariant();
    }
}

QVariantMap FridgeModel::get(int row) const {
    QVariantMap map;
    if (row >= 0 && row < products.size()) {
        const Product &p = products[row];
        map["name"] = p.name;
        map["quantity"] = p.quantity;
        map["expiry"] = p.expiry.toString("yyyy-MM-dd");
        map["category"] = p.category;
    }
    return map;
}


QHash<int, QByteArray> FridgeModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[QuantityRole] = "quantity";
    roles[ExpiryRole] = "expiry";
    roles[CategoryRole] = "category";
    return roles;
}

void FridgeModel::setProducts(const QList<Product> &newProducts) {
    beginResetModel();
    products = newProducts;
    endResetModel();
}

const QList<Product> &FridgeModel::getProducts() const {
    return products;
}
