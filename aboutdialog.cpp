#include "aboutdialog.h"
#include <QtWidgets>



AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
{
    QLabel *label = new QLabel(this);
    QPushButton *okButton = new QPushButton("Ok");
    label->setOpenExternalLinks(true);
    label->setText("<p>Simple HTML code generator<br>"
                   "for <a href='https://QuakeEngines.github.io'>QuakeEngines.github.io</a><br><br>"
                   "App repository: <a href='https://github.com/QuakeEngines/Simple_QE_HTML_generator'>on github</a><br><br>"
                   "Made by Maxi, 2020</p>");


    connect(okButton, SIGNAL(clicked()), SLOT(accept()));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(okButton);
    setLayout(layout);

    setWindowTitle("About");

}

AboutDialog::~AboutDialog()
{
}
