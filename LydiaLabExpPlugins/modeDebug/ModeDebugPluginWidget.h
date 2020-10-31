#ifndef MODEDEBUGPLUGINWIDGET_H
#define MODEDEBUGPLUGINWIDGET_H

#include <QWidget>

namespace Ui {
class ModeDebugPluginWidget;
}

class ModeDebugPluginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ModeDebugPluginWidget(QWidget *parent = nullptr);
    ~ModeDebugPluginWidget();

    Ui::ModeDebugPluginWidget *getUi();

private:
    Ui::ModeDebugPluginWidget *ui;
};

#endif // MODEDEBUGPLUGINWIDGET_H
