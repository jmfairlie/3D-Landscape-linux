#ifndef MENUMANGER_H
#define MENUMANGER_H
#include <QMouseEvent>
#include <QPainter>
#include "MenuItem.h"
#include <QImage>
#include "Utils3D.h"


class MenuItem;

class MenuManager : public QObject
{
Q_OBJECT

public:
    MenuManager();
    ~MenuManager();
    void renderMenu(QPainter* painter);
    void updateMenu();
    void createMenu();
    void processMouseEvent(QMouseEvent *e);
    void showHideMenu();
    void resize(int width, int height);
    void setCurrent(MenuItem* item);
    MenuItem* getCurrent();
    void resizeItems(QPointF newItemSize);
    void renderBg(QPainter* painter);
    void emitClickEvent(QString name);
    void notifyEndOfShrink(MenuItem* item);
    void setRoot();
    void hideMenuImmediate();
    void showMenuImmediate();

signals:
    void clicked(QString menuItemName);

private:
    int numItems;
    int numIcons;
    MenuItem * currentItem;
    MenuItem ** menuItems;
    QImage ** iconList;
    QPointF center;
    QPointF size;
    float bgoffset;
    bool bglocked;
    bool bgdescending;
    float moveratio;
};
#endif // MENUMANGER_H
