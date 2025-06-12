#ifndef FRIDGE_MODEL_H
#define FRIDGE_MODEL_H

#include <QAbstractListModel>
#include <QDate>

struct Product {
    QString name;
    QString quantity;
    QDate expiry;
    QString category;
};

class FridgeModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        QuantityRole,
        ExpiryRole,
        CategoryRole
    };

    FridgeModel(QObject *parent = nullptr);

    Q_INVOKABLE QVariantMap get(int row) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setProducts(const QList<Product> &newProducts);
    const QList<Product> &getProducts() const;

private:
    QList<Product> products;
};

#endif // FRIDGE_MODEL_H
