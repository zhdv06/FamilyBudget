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
    virtual void init() = 0;
    virtual void reset() = 0;
    virtual void addRecord() = 0;
    virtual void removeRecord() = 0;
    virtual void confirmRecord() = 0;
    virtual void cancelRecord() = 0;

signals:

};

#endif // IWIDGET_H
