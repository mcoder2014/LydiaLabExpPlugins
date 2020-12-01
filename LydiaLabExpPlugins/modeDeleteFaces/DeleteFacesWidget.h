#ifndef DELETEFACESWIDGET_H
#define DELETEFACESWIDGET_H

#include <QWidget>

namespace Ui {
class DeleteFacesWidget;
}

class DeleteFacesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeleteFacesWidget(QWidget *parent = nullptr);
    ~DeleteFacesWidget();
    Ui::DeleteFacesWidget *ui;
};

#endif // DELETEFACESWIDGET_H
