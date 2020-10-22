#include "TransformRichParameterWidget.h"
#include "ui_TransformRichParameterWidget.h"

TransformRichParameterWidget::TransformRichParameterWidget(
        QWidget *parent, TransformRichParameter *transformRichParamter)
            :RichParameterWidget(parent, transformRichParamter),
            ui(new Ui::TransformRichParameterWidget)
{
    /// 构建 UI 样式
    QWidget* widget = new QWidget(nullptr);
    ui->setupUi(widget);
    this->gridLay->addWidget(widget);

    /// 建立信号链接


}

TransformRichParameterWidget::~TransformRichParameterWidget()
{
    delete ui;
}

void TransformRichParameterWidget::resetWidgetValue()
{

}

void TransformRichParameterWidget::collectWidgetValue()
{

}

void TransformRichParameterWidget::setWidgetValue(const RichParameter::Value &nv)
{

}
