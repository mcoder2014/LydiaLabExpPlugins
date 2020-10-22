#ifndef TRANSFORMRICHPARAMETERWIDGET_H
#define TRANSFORMRICHPARAMETERWIDGET_H

#include <Eigen/Dense>

#include <parameters/RichParameter.h>
#include <parameters/RichParameterWidget.h>
#include <parameters/RichParameterSet.h>

using Eigen::Vector3d;

namespace Ui {
class TransformRichParameterWidget;
}
class TransformRichParameter;

/**
 * @brief The TransformRichParameterWidget class
 * UI 相关
 */
class TransformRichParameterWidget : public RichParameterWidget
{
    Q_OBJECT

public:
    explicit TransformRichParameterWidget(QWidget *parent, TransformRichParameter* transformRichParamter);
    virtual ~TransformRichParameterWidget() override;

private:
    Ui::TransformRichParameterWidget *ui;

    // RichParameterWidget interface
public:
    virtual void resetWidgetValue() override;
    virtual void collectWidgetValue() override;
    virtual void setWidgetValue(const RichParameter::Value &nv) override;
};

/**
 * @brief The TransformRichParameter class
 */
class TransformRichParameter:public RichParameter
{
public:
    TransformRichParameter():RichParameter("Transform","","")
    {}
    TransformRichParameter(QString name, QString description ="", QString tooltip = "")
        :RichParameter(name, description, tooltip)
    {
        this->defaultValue = new Value();
        this->val = new Value;
    }
    virtual ~TransformRichParameter()
    {

    }

    virtual RichParameterWidget* getWidget(QWidget* parent){
        return new TransformRichParameterWidget(parent,this);
    }

private:

public:
    class Value
            :public RichParameter::Value
    {
    public:
        Value(Vector3d pos, Vector3d rot, Vector3d scale)
            :position(pos), rotation(rot), scale(scale){}

        Value()
            :position(.0,.0,.0),
             rotation(.0,.0,.0),
             scale(.0,.0,.0){}

        Value(const Value& value)
            :position(value.position),
             rotation(value.rotation),
             scale(value.scale){}

        virtual ~Value() override {}

    private:
        // 位移信息
        Vector3d position;

        // 旋转信息
        Vector3d rotation;

        // 缩放信息
        Vector3d scale;
    };
};



#endif // TRANSFORMRICHPARAMETERWIDGET_H
