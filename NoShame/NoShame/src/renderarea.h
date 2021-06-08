#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(QWidget* parent = nullptr);
protected:
    void paintEvent(QPaintEvent* event) override;
};

#endif // RENDERAREA_H
