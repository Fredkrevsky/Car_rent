#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("coursach");
    db.setUserName("Fedar");
    db.setPassword("1");

    if (db.open()){
        qDebug() << "open";
    }
    else {
        qDebug() << "no open";
    }

    auto tables = db.tables();
    ui->comboBox->addItems(tables);

    query = new QSqlQuery(db);

    model = new QSqlTableModel(this, db);
    model->setTable("book");
    model->select();

    ui->tableView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    model->insertRow(model->rowCount());
}


void MainWindow::on_pushButton_2_clicked()
{
    model->removeRow(row);
}


void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    row = index.row();
}


void MainWindow::on_pushButton_3_clicked()
{
    auto comboBox = ui->comboBox;
    QString previous_name = comboBox->currentText();

    comboBox->clear();
    QStringList tables = db.tables();

    if (!tables.isEmpty()) {
        comboBox->addItems(tables);

        int found_index = tables.indexOf(previous_name);
        if (found_index >= 0) {
            comboBox->setCurrentIndex(found_index);
        } else {
            comboBox->setCurrentIndex(0);
        }

        on_comboBox_activated(comboBox->currentIndex());
    } else {
        comboBox->clear();
        model->clear();
    }
}


void MainWindow::on_comboBox_activated(int index)
{
    model->clear();

    QString current_table = ui->comboBox->currentText();
    model->setTable(current_table);

    model->select();
}
