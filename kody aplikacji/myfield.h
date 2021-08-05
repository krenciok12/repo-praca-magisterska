#ifndef MYFIELD_H
#define MYFIELD_H

#include <QWidget>
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QFile>
#include <math.h>


class MyField : public QWidget
{
    Q_OBJECT
public:
    explicit MyField(QWidget *parent = nullptr);

    size_t getNumberOfVertex();
    void addEdge(size_t a, size_t b);
    void setCanBeAdded(bool b);
    void loadFromFile(QString filename);
    std::vector<std::vector<size_t>> getEdges();
    void makeColorful(std::vector<int> perm);
    void clear();


signals:
    void updateComboBoxes();

protected:
    void paintEvent(QPaintEvent * event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent (QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;


public slots:

private:
    std::vector<std::pair<int,int>> vertices;
    std::vector<std::vector<size_t>> edgesByVertex;
    std::vector<std::vector<int>> colors;
    bool canBeAdded;
    size_t moving;
    bool isMoving;
};

#endif // MYFIELD_H
