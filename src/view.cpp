#include "view.h"
#include "ui_view.h"

View::View(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::View)
{
    ui->setupUi(this);
}

View::~View()
{
    delete ui;
}

void View::handleSubmitClick()
{
    emit textSubmitted(ui->textEdit->toPlainText());
}
