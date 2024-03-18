#include "changebook.h"
#include "ui_changebook.h"

ChangeBook::ChangeBook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeBook)
{
    ui->setupUi(this);
}

void ChangeBook::resiveData(QString *data)
{
    ui->line1->setText(data[0]);
    ui->line2->setText(data[1]);
    ui->date->setDate(QDate::fromString(data[2],"dd/MM/yyyy"));
    ui->line3->setText(data[3]);
}

void ChangeBook::on_savechanges_clicked()
{
    QString data[4];
    data[0] = ui->line1->text();
    data[1] = ui->line2->text();
    data[2] = ui->date->text();
    data[3] = ui->line3->text();
    emit sendChanges(data);
}

ChangeBook::~ChangeBook()
{
    delete ui;
}
