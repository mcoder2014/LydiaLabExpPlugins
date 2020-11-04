#include "ModeFixWidget.h"
#include "ui_ModeFixWidget.h"

ModeFixWidget::ModeFixWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModeFixWidget)
{
    ui->setupUi(this);
}

ModeFixWidget::~ModeFixWidget()
{
    delete ui;
}
