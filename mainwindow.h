#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFile>
#include <QProcess>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QItemDelegate>
#include <QApplication>
#include "addbook.h"
#include "changebook.h"
#include "selfsortfilterproxymodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class RowNumberDelegate : public QStyledItemDelegate {
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
        if (index.column() == 0) {
            painter->drawText(option.rect, Qt::AlignCenter, QString::number(index.row() + 1));
        } else {
            QStyledItemDelegate::paint(painter, option, index);
        }
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sendData(QString*);

private slots:
    void importBooks();
    void exportBooks();

    void on_addbook_clicked();
    void on_bookdetails_clicked();
    void on_removebook_clicked();

    void reciveSignaltoAddBook(QString*);
    void reciveChanges(QString*);
    //void reciveChanges();

    bool checkDubplicat(QString);
    void clearData();
    void endAdd();
    void on_search_clicked();
private:
    SelfSortFilterProxyModel selfModel;
    //QSortFilterProxyModel proxyModel;
    Ui::MainWindow *ui;
    QStandardItemModel model;
    //QVector<QVector<QString>> books;
    AddBook addBook;
    ChangeBook changeBook;
    RowNumberDelegate delegat;
    int Row;
};
#endif // MAINWINDOW_H
