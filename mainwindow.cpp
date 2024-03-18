#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QStandardItemModel model;
    model.setColumnCount(5);
    model.setHeaderData(0, Qt::Horizontal, "№");
    model.setHeaderData(1, Qt::Horizontal, "Название");
    model.setHeaderData(2, Qt::Horizontal, "Автор");
    model.setHeaderData(3, Qt::Horizontal, "Год издания");
    model.setHeaderData(4, Qt::Horizontal, "Жанр");
    //ui->booktable->setModel(&model);
    selfModel.setSourceModel(&model);
    //proxyModel.setSourceModel(&model);
    ui->booktable->setModel(&selfModel);
    //ui->booktable->setModel(&proxyModel);
    ui->booktable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->booktable->setSortingEnabled(true);
    ui->booktable->setItemDelegateForColumn(0, &delegat);
    ui->booktable->verticalHeader()->setVisible(false);

    ui->booktable->setColumnWidth(0, 15);

    QAction *importAction = new QAction("Открыть проводник");
    QAction *exportAction = new QAction("Открыть проводник");
    QMenu *imp = menuBar()->addMenu("Импорт книг");
    imp->addAction(importAction);
    QMenu *exp = menuBar()->addMenu("Экспорт книг");
    exp->addAction(exportAction);

    connect(importAction, &QAction::triggered, this, &MainWindow::importBooks);
    connect(exportAction, &QAction::triggered, this, &MainWindow::exportBooks);

    connect(&addBook, &AddBook::sendSignaltoAddBook,this, &MainWindow::reciveSignaltoAddBook);
    connect(&addBook, &AddBook::sendClose,this, &MainWindow::endAdd);

    connect(this, &MainWindow::sendData, &changeBook, &ChangeBook::resiveData);
    connect(&changeBook, &ChangeBook::sendChanges, this, &MainWindow::reciveChanges);

    /*
    connect(ui->searchline, &QLineEdit::textChanged, this, [proxyModel](const QString &text){
        QRegExp regExp(text, Qt::CaseInsensitive, QRegExp::RegExp);
        proxyModel->setFilterRegExp(regExp);
    });
    */
}

void MainWindow::on_addbook_clicked()
{
    addBook.show();
    ui->addbook->setEnabled(false);
    addBook.exec();
}

void MainWindow::on_bookdetails_clicked()
{
    QItemSelectionModel *selectionModel = ui->booktable->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedIndexes();
    if(selectedRows.count() != 1) return;
    QString data[4];
    Row = selectedRows.at(0).row();
    data[0] = model.data(model.index(selectedRows.at(0).row(), 1)).toString();
    data[1] = model.data(model.index(selectedRows.at(0).row(), 2)).toString();
    data[2] = model.data(model.index(selectedRows.at(0).row(), 3)).toString();
    data[3] = model.data(model.index(selectedRows.at(0).row(), 4)).toString();
    emit sendData(data);
    changeBook.show();
    changeBook.exec();

    //ui->bookdetails->setEnabled(false);
    //books.append(QVector<QString>{data[0], data[1], data[2], data[3]});
}


void MainWindow::on_removebook_clicked()
{
    QItemSelectionModel *selectionModel = ui->booktable->selectionModel();
    QModelIndexList selectedRows = selectionModel->selectedIndexes();

    for (const QModelIndex &index : selectedRows) {
        model.removeRow(index.row());
    }
    model.submit();
    selfModel.setSourceModel(&model);
    ui->booktable->setModel(&selfModel);
}

void MainWindow::on_search_clicked() // разобраться с QSortFilterProxyModel и QStandardItemModel
{
    QString text = ui->searchline->text().simplified().toLower();
    selfModel.setFilterRegularExpression(text);
    ui->booktable->update();
}

void MainWindow::reciveSignaltoAddBook(QString *data)
{
    QStringList strList;
    int rowCount = model.rowCount();

    for (int row = 0; row < rowCount; ++row)
    {
        QModelIndex index = model.index(row, 1);
        QVariant d = model.data(index);
        strList.append(d.toString());
    }

    if(!strList.contains(data[0]))
    {
        //int newRow = ui->bookTable->model()->rowCount();
        //ui->bookTable->model()->insertRow(newRow);
        QList<QStandardItem*> row;
        model.appendRow(row);
        model.setData(model.index(model.rowCount()-1, 1), data[0]);
        model.setData(model.index(model.rowCount()-1, 2), data[1]);
        model.setData(model.index(model.rowCount()-1, 3), data[2]);
        model.setData(model.index(model.rowCount()-1, 4), data[3]);
        model.submit();
    }
    else{
        QMessageBox msgBox;
        msgBox.setFixedSize(500, 500);
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Книга уже занесена в список");
        msgBox.exec();
    }

}

void MainWindow::endAdd(){ ui->addbook->setEnabled(true); }

void MainWindow::reciveChanges(QString *data)
{
    int colCount = model.columnCount();
    for(int cel = 1; cel < colCount; cel++){
        QModelIndex data_index = model.index(Row, cel);
        model.setData(data_index, data[cel-1]);
    }
    model.submit();
}

void MainWindow::importBooks(){

    QString sourceFilePath = QFileDialog::getOpenFileName(nullptr, "Выберите файл для импорта", "", "(*.json);; (*.csv)");
    if (sourceFilePath.isEmpty()) {
        return; // Пользователь отменил выбор файла
    }

    QFile file(sourceFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open file for reading";
        return;
    }

    if(model.rowCount() > 0) {
        model.removeRows(0, model.rowCount());
        //model.removeColumns(0, model.columnCount());
        //clearData();
    }

    QByteArray data = file.readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

    QJsonObject jsonObject = jsonDoc.object();
    QJsonArray dataArray = jsonObject["data"].toArray();

    for (const QJsonValue &value : dataArray)
    {
        QJsonObject item = value.toObject();
        QList<QStandardItem*> row;
        model.appendRow(row);
        model.setData(model.index(model.rowCount()-1, 1), item["Название"].toString());
        model.setData(model.index(model.rowCount()-1, 2), item["Автор"].toString());
        model.setData(model.index(model.rowCount()-1, 3), item["Дата"].toString());
        model.setData(model.index(model.rowCount()-1, 4), item["Жанр"].toString());

    }
    model.submit();
    file.close();
}

void MainWindow::exportBooks(){
    QString exportFilePath = QFileDialog::getSaveFileName(nullptr, "Выберите файл для экспорта", "", "(*.json);;(*.csv)");
    if (exportFilePath.isEmpty()) {
        return; // Пользователь отменил выбор файла
    }

    QJsonObject json;
    QJsonArray jsonArray;
    for (int row = 0; row < ui->booktable->model()->rowCount(); ++row)
    {
       QJsonObject jsonObj;

       QModelIndex index = ui->booktable->model()->index(row, 1);
       QVariant data = ui->booktable->model()->data(index, Qt::DisplayRole);
       jsonObj["Название"] = data.toString();
       index = ui->booktable->model()->index(row, 2);
       data = ui->booktable->model()->data(index, Qt::DisplayRole);
       jsonObj["Автор"] = data.toString();
       index = ui->booktable->model()->index(row, 3);
       data = ui->booktable->model()->data(index, Qt::DisplayRole);
       jsonObj["Дата"] = data.toString();
       index = ui->booktable->model()->index(row, 4);
       data = ui->booktable->model()->data(index, Qt::DisplayRole);
       jsonObj["Жанр"] = data.toString();

       jsonArray.append(jsonObj);
    }

    json["data"] = jsonArray;

    QFile file(exportFilePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QJsonDocument jsonDoc(json);
        file.write(jsonDoc.toJson());
        qDebug() << "Данные успешно экспортированы в файл: " << exportFilePath;
    } else {
        qDebug() << "Не удалось экспортировать данные в файл.";
    }
    file.close();
}

void MainWindow::clearData(){
    QAbstractItemModel *model = ui->booktable->model();

    int rowCount = model->rowCount();
    int columnCount = model->columnCount();

    for (int row = 0; row < rowCount; ++row) {
        for (int column = 0; column < columnCount; ++column) {
            QModelIndex index = model->index(row, column);
            model->setData(index, QVariant());
        }
    }
    model->submit();
}

bool MainWindow::checkDubplicat(QString str)
{

}

MainWindow::~MainWindow()
{
    delete ui;
}

