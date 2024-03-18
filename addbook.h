#ifndef ADDBOOK_H
#define ADDBOOK_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class AddBook;
}

class AddBook : public QDialog
{
    Q_OBJECT

public:
    explicit AddBook(QWidget *parent = nullptr);
    ~AddBook();
signals:
    void sendSignaltoAddBook(QString*);
    void sendClose();
private slots:
    void on_addButton_clicked();
    void closeEvent(QCloseEvent*);
private:
    Ui::AddBook *ui;
};
#endif // ADDBOOK_H
