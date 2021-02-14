#ifndef IWIDGET_H
#define IWIDGET_H

#include <QWidget>

class IWidget : public QWidget
{
    Q_OBJECT
public:
    explicit IWidget(QWidget *parent = nullptr): QWidget(parent) {};
    virtual ~IWidget() {};

public slots:
    virtual void addRecord() = 0;
    virtual void removeRecord() = 0;

signals:

};

#endif // IWIDGET_H
