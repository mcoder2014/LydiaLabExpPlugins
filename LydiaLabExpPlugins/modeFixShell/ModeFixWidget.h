#ifndef MODEFIXWIDEGET_H
#define MODEFIXWIDEGET_H

#include <QWidget>

namespace Ui {
class ModeFixWidget;
}

class ModeFixWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ModeFixWidget(QWidget *parent = nullptr);
    ~ModeFixWidget();
    Ui::ModeFixWidget *ui;
};

#endif // MODEFIXWIDEGET_H
