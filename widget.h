#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "aboutdialog.h"
#include <QtWidgets>



class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QLineEdit *lineEdit_1;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_6;
    QPlainTextEdit *resultText;

private slots:
    void generateHTML();
    void showAboutDialog();

private:

};
#endif // WIDGET_H
