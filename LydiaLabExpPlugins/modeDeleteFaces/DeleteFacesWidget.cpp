#include "DeleteFacesWidget.h"
#include "ui_DeleteFacesWidget.h"

DeleteFacesWidget::DeleteFacesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeleteFacesWidget)
{
    ui->setupUi(this);
}

DeleteFacesWidget::~DeleteFacesWidget()
{
    delete ui;
}
