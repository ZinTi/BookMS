
#include "WidgetPublicMethod.h"

// 该函数接收一个 QTableWidget 指针和一个要设置为不可编辑的列索引
void setColumnNonEditable(QTableWidget* tableWidget, int column) {
    int rowCount = tableWidget->rowCount();
    for (int row = 0; row < rowCount; ++row) {
        QTableWidgetItem* item = tableWidget->item(row, column);
        if (item == nullptr) {
            item = new QTableWidgetItem();
            tableWidget->setItem(row, column, item);
        }
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    }
}