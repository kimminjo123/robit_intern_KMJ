#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>
#include <QTableWidgetItem>
#include <QString>

struct Product {
    QString name;
    int quantity;
};

static std::vector<Product> inventory;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(2);
    QStringList name;
    name << "상품명" << "수량";
    ui->tableWidget->setHorizontalHeaderLabels(name);
    ui->tableWidget->setColumnWidth(0, 369);
    ui->tableWidget->setColumnWidth(1, 100);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(false);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pushButton_clicked() {
    QString name = ui->lineEdit->text();
    int qty = ui->spinBox->value();
    if (name.isEmpty() || qty <= 0) {
        return;
    }
    Product p{name, qty};
    inventory.push_back(p);
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(name));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(qty)));
    ui->lineEdit->clear();
    ui->spinBox->setValue(1);
}


void MainWindow::on_pushButton_2_clicked() {
    if (inventory.empty()) {
        return;
    }
    inventory.erase(inventory.begin());
    ui->tableWidget->removeRow(0);
}


void MainWindow::on_lineEdit_cursorPositionChanged(int, int) {

}


void MainWindow::on_spinBox_textChanged(const QString &) {

}


void MainWindow::on_tableWidget_cellActivated(int, int) {

}
