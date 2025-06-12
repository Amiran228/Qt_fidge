import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Controls.Material 2.15
import Fridge

ApplicationWindow {
    visible: true
    width: 800
    height: 500
    title: "Учёт продуктов в холодильнике"
    Material.accent: Material.LightBlue
    function isValidDate(dateStr) {
        var regex = /^\d{4}-\d{2}-\d{2}$/;
        if (!regex.test(dateStr)) return false;
        var date = Date.fromLocaleString(Qt.locale(), dateStr, "yyyy-MM-dd");
        return date instanceof Date && !isNaN(date);
    }
    Item {
        anchors.fill: parent
        z: -1 // делает фон позади всего остального

        Image {
            anchors.fill: parent
            source: "qrc:/background.png" // или путь к изображению
            fillMode: Image.PreserveAspectCrop
        }

        Rectangle {
            anchors.fill: parent
            color: "#60FFFFFF" // почти белый, с очень слабой прозрачностью
        }

    }
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10

        // Секция фильтрации
        RowLayout {
            TextField {
                id: searchField
                placeholderText: "Поиск по названию..."
                Layout.fillWidth: true
                onTextChanged: {
                    fridgeApp.filterText = text;
                    productList.currentIndex = -1; // Сброс выбора
                }
                focus: true
            }
            ComboBox {
                id: categoryFilter
                model: ["Все категории"].concat(fridgeApp.categories)
                onCurrentTextChanged: {
                    fridgeApp.filterCategory = currentText;
                    productList.currentIndex = -1; // Сброс выбора
                }
            }
            Button {
                text: "Сортировать по сроку годности"
                Material.foreground: "white"
                Material.background: Material.LightGreen
                font.bold: true
                onClicked: fridgeApp.sortProducts()
            }
            Button {
                text: "Сбросить фильтры"
                Material.foreground: "white"
                Material.background: Material.LightGreen
                font.bold: true
                onClicked: {
                    searchField.text = "";
                    categoryFilter.currentIndex = 0;
                    productList.currentIndex = -1;
                }
            }
        }

        // Список продуктов
        ListView {
            id: productList
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: fridgeModel
            clip: true
            spacing: 2
            focus: true
            activeFocusOnTab: true
            onCurrentIndexChanged: console.log("Current index changed to:", currentIndex)

            header: Rectangle {
                width: productList.width
                height: 44
                radius: 4
                color: "#a8d5ba" // Светло-синий цвет
                border.color: "#a0c8e0"
                border.width: 1

                Row {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 10

                    Text {
                        text: "Название"
                        width: parent.width * 0.3
                        font.bold: true
                        color: "#003366"
                        verticalAlignment: Text.AlignVCenter
                    }
                    Text {
                        text: "Количество"
                        width: parent.width * 0.2
                        font.bold: true
                        color: "#003366"
                        verticalAlignment: Text.AlignVCenter
                    }
                    Text {
                        text: "Срок годности"
                        width: parent.width * 0.25
                        font.bold: true
                        color: "#003366"
                        verticalAlignment: Text.AlignVCenter
                    }
                    Text {
                        text: "Категория"
                        width: parent.width * 0.25
                        font.bold: true
                        color: "#003366"
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }

            delegate: Rectangle {
                width: productList.width
                height: 44
                radius: 4
                border.width: 1
                border.color: "#dddddd"
                color: productList.currentIndex === index
                    ? "#b2f2bb" // Нежный голубой для выбранной строки
                    : mouseArea.containsMouse
                        ? "#f5f5f5"
                        : "white"

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    propagateComposedEvents: false
                    onClicked: {
                        productList.currentIndex = index
                        productList.forceActiveFocus()
                    }
                    onDoubleClicked: editDialog.open(index)
                }

                Row {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 10

                    Text {
                        text: model.name
                        width: parent.width * 0.3
                        color: "#202020"
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }
                    Text {
                        text: model.quantity
                        width: parent.width * 0.2
                        color: "#202020"
                        verticalAlignment: Text.AlignVCenter
                    }
                    Text {
                        text: model.expiry
                        width: parent.width * 0.25
                        color: "#202020"
                        verticalAlignment: Text.AlignVCenter
                    }
                    Text {
                        text: model.category
                        width: parent.width * 0.25
                        color: "#202020"
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }
                }
            }
}
        // Форма добавления продукта
        RowLayout {
            TextField {
                id: nameField
                placeholderText: "Название"
                Layout.fillWidth: true
                color: text ? "black" : "red"
            }
            TextField {
                id: quantityField
                placeholderText: "Количество"
                Layout.fillWidth: true
                color: text ? "black" : "red"
            }
            TextField {
                id: expiryField
                placeholderText: "Срок годности (гггг-мм-дд)"
                Layout.fillWidth: true
                text: Qt.formatDate(new Date(), "yyyy-MM-dd")
                color: isValidDate(text) ? "black" : "red"
                ToolTip.visible: !isValidDate(text) && text
                ToolTip.text: "Введите дату в формате гггг-мм-дд"
            }
            ComboBox {
                id: categoryCombo
                model: fridgeApp.categories
                Layout.fillWidth: true
            }
            Button {
                text: "Добавить"
                Material.foreground: "white"
                Material.background: Material.Green
                font.bold: true
                onClicked: {
                    if (!nameField.text || !quantityField.text || !expiryField.text) {
                        errorDialog.text = "Заполните все поля!";
                        errorDialog.visible = true;
                        return;
                    }
                    if (!isValidDate(expiryField.text)) {
                        errorDialog.text = "Неверный формат даты (гггг-мм-дд)!";
                        errorDialog.visible = true;
                        return;
                    }
                    var date = Date.fromLocaleString(Qt.locale(), expiryField.text, "yyyy-MM-dd");
                    fridgeApp.addProduct(nameField.text, quantityField.text, date, categoryCombo.currentText);
                    nameField.text = "";
                    quantityField.text = "";
                    expiryField.text = Qt.formatDate(new Date(), "yyyy-MM-dd");
                    categoryCombo.currentIndex = 0;
                    productList.currentIndex = -1;
                }
            }
            Button {
                text: "Редактировать"
                enabled: productList.currentIndex >= 0
                Material.foreground: "white"
                Material.background: Material.Grey
                font.bold: true
                onClicked: {
                    if (productList.currentIndex >= 0) {
                        editDialog.open(productList.currentIndex);
                    } else {
                        errorDialog.text = "Выберите продукт для редактирования!";
                        errorDialog.visible = true;
                    }
                }
            }

            Button {
                text: "Удалить"
                enabled: productList.currentIndex >= 0
                Material.foreground: "white"
                Material.background: Material.Red
                font.bold: true
                onClicked: {
                    if (productList.currentIndex >= 0) {
                        console.log("Delete button clicked for index:", productList.currentIndex);
                        confirmDeleteDialog.visible = true;
                    } else {
                        errorDialog.text = "Выберите продукт для удаления!";
                        errorDialog.visible = true;
                    }
                }
            }
        }
    }

    // Диалог редактирования
    Dialog {
        id: editDialog
        title: "Редактировать продукт"
        standardButtons: Dialog.Ok | Dialog.Cancel
        property int index: -1
        modal: true
        width: 400
        height: 500

        function open(idx) {
            if (idx < 0 || idx >= fridgeModel.count) {
                errorDialog.text = "Выберите продукт для редактирования!";
                errorDialog.visible = true;
                return;
            }

            index = idx;
            var product = fridgeModel.get(idx);
            editName.text = product.name;
            editQuantity.text = product.quantity;
            editExpiry.text = product.expiry;
            editCategory.currentIndex = fridgeApp.categories.indexOf(product.category);
            visible = true;
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 10

            TextField {
                id: editName
                placeholderText: "Название"
                Layout.fillWidth: true
            }
            TextField {
                id: editQuantity
                placeholderText: "Количество"
                Layout.fillWidth: true
            }
            TextField {
                id: editExpiry
                placeholderText: "Срок годности (гггг-мм-дд)"
                Layout.fillWidth: true
                ToolTip.visible: !isValidDate(text) && text
                ToolTip.text: "Введите дату в формате гггг-мм-дд"
            }
            ComboBox {
                id: editCategory
                model: fridgeApp.categories
                Layout.fillWidth: true
            }
        }

        onAccepted: {
            if (!editName.text || !editQuantity.text || !editExpiry.text) {
                errorDialog.text = "Заполните все поля!";
                errorDialog.visible = true;
                return;
            }
            if (!isValidDate(editExpiry.text)) {
                errorDialog.text = "Неверный формат даты (гггг-мм-дд)!";
                errorDialog.visible = true;
                return;
            }
            var date = Date.fromLocaleString(Qt.locale(), editExpiry.text, "yyyy-MM-dd");
            fridgeApp.editProduct(index, editName.text, editQuantity.text, date, editCategory.currentText);
            productList.currentIndex = -1;
        }
    }

    // Диалог ошибок
    MessageDialog {
        id: errorDialog
        title: "Ошибка"
        text: ""
        buttons: MessageDialog.Ok
    }

    // Диалог подтверждения удаления
    Dialog {
            id: confirmDeleteDialog
            title: "Подтверждение"
            modal: true
            standardButtons: Dialog.Yes | Dialog.No
            anchors.centerIn: parent

            contentItem: Row {
                spacing: 10
                Image {
                    id: dialogIcon
                    source: "qrc:/delete_icon.png"
                    width: 32
                    height: 32
                    anchors.verticalCenter: parent.verticalCenter
                }
                Text {
                    text: "Удалить выбранный продукт?"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        onAccepted: {
            if (productList.currentIndex >= 0) {
                console.log("Confirmed deletion for index:", productList.currentIndex);
                fridgeApp.removeProduct(productList.currentIndex);
                productList.currentIndex = -1;
            }
        }
    }
}
