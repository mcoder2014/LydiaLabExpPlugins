#include "ModeDebugPluginWidget.h"
#include "ui_ModeDebugPluginWidget.h"

ModeDebugPluginWidget::ModeDebugPluginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModeDebugPluginWidget)
{
    ui->setupUi(this);
}

ModeDebugPluginWidget::~ModeDebugPluginWidget()
{
    delete ui;
}

Ui::ModeDebugPluginWidget *ModeDebugPluginWidget::getUi()
{
    return ui;
}
