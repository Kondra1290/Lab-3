#ifndef CHANGEBOOK_H
#define CHANGEBOOK_H

#include <QDialog>

namespace Ui {
class ChangeBook;
}

class ChangeBook : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeBook(QWidget *parent = nullptr);

    void resiveData(QString*);
    ~ChangeBook();
signals:
    void sendChanges(QString*);
    //void sendChanges();
private slots:
    void on_savechanges_clicked();

private:
    Ui::ChangeBook *ui;
};

#endif // CHANGEBOOK_H
