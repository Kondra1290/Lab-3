#include "addbook.h"
#include "ui_addbook.h"

AddBook::AddBook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddBook)
{
    ui->setupUi(this);
    this->setWindowTitle("Добавление новой книги");
    //ui->dateEdit->setDate(QDate::currentDate());
}

void AddBook::on_addButton_clicked()
{
    QString book[4];
    book[0] = ui->bookLine->text().simplified();
    book[1] = ui->autorLine->text().simplified();
    book[2] = ui->dateEdit->text().simplified();
    book[3] = ui->genreLine->text().simplified();

    if(!book[0].isEmpty() && !book[1].isEmpty() && !book[2].isEmpty() && !book[3].isEmpty()){
        emit sendSignaltoAddBook(book);
    }
    else{
        QMessageBox msgBox;
        msgBox.setFixedSize(500, 500);
        msgBox.setWindowTitle("Ошибка");
        msgBox.setText("Не все поля заполнены");
        msgBox.exec();
    }
}

void AddBook::closeEvent(QCloseEvent *event)
{
    emit sendClose();
    QDialog::closeEvent(event);
}

AddBook::~AddBook()
{
    delete ui;
}
