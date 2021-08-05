#include "myfield.h"

MyField::MyField(QWidget *parent) : QWidget(parent)
{
    this->canBeAdded = false;
    this->moving = 0;
    this->isMoving = false;
}

void MyField::paintEvent(QPaintEvent * event)
{

    QPainter painter(this);
    QWidget::paintEvent(event);

    QFont font;
    font.setPixelSize(15);
    painter.setFont(font);

    QPen pen;
    QBrush brush;
    QColor color;

    QPalette pal = palette();
    color.setRgb(106, 205, 218);
    pal.setColor(QPalette::Background, color);
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    painter.setBrush(QBrush(color));


    for (size_t i=0; i<this->edgesByVertex.size(); i++){
        for (size_t j=0; j<this->edgesByVertex[i].size(); j++){
            if (i < this->edgesByVertex[i][j]){
                QLine line(this->vertices[i].first,this->vertices[i].second,this->vertices[this->edgesByVertex[i][j]].first,this->vertices[this->edgesByVertex[i][j]].second);
                color.setRgb((this->colors[i][0]+this->colors[this->edgesByVertex[i][j]][0])/2,(this->colors[i][1]+this->colors[this->edgesByVertex[i][j]][1])/2,(this->colors[i][2]+this->colors[this->edgesByVertex[i][j]][2])/2);
                painter.setPen(QPen(color,3));
                painter.drawLine(line);
            }
        }
    }
    color.setRgb(0,0,0);
    painter.setPen(color);
    for (size_t i=0; i<this->vertices.size();i++){
        color.setRgb(this->colors[i][0],this->colors[i][1],this->colors[i][2]);
        painter.setBrush(QBrush(color));
        painter.drawEllipse(this->vertices[i].first-5,this->vertices[i].second-5,10,10);
        color.setRgb(0,0,0);
        painter.setPen(color);
        painter.drawText(this->vertices[i].first-5,this->vertices[i].second-7,QString::number(i));
    }
}

size_t MyField::getNumberOfVertex(){
    return this->vertices.size();
}

void MyField::addEdge(size_t a, size_t b){
    if (a == b)
        return;
    if (std::find(this->edgesByVertex[a].begin(), this->edgesByVertex[a].end(),b) != this->edgesByVertex[a].end()){
        return;
    }
    this->edgesByVertex[a].push_back(b);
    this->edgesByVertex[b].push_back(a);
    this->update();
}

void MyField::setCanBeAdded(bool b){
    this->canBeAdded=b;
}

void MyField::loadFromFile(QString filename){
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

    QTextStream in(&file);

    QString line = in.readLine();
    QStringList fields = line.split(' ');
    size_t n = size_t(fields[0].toInt());

    int x = this->size().width()/2;
    int y = this->size().height()/2;
    double r = 150;
    double theta = 2*M_PI/n;


    this->vertices.clear();
    this->edgesByVertex.clear();
    this->colors.clear();

    for (size_t i=0; i<n; i++){
        int st = int(floor(r*sin(theta*i)));
        int ct = int(floor(r*cos(theta*i)));
        this->vertices.push_back(std::pair<int,int>{x + st, y + ct});
        this->colors.push_back({255,255,255});
        this->edgesByVertex.push_back(std::vector<size_t>{});
    }

    while (!in.atEnd())
    {
        line = in.readLine();
        fields = line.split(' ');
        size_t a = size_t(fields[1].toInt());
        size_t b = size_t(fields[2].toInt());
        this->addEdge(a % n, b % n);
    }

}

std::vector<std::vector<size_t>> MyField::getEdges(){
    return this->edgesByVertex;
}

void MyField::mousePressEvent(QMouseEvent *event){
    int x = event->x();
    int y = event->y();

    if (canBeAdded){
        this->vertices.push_back(std::pair<int,int>{x,y});
        this->colors.push_back({255,255,255});
        this->edgesByVertex.push_back(std::vector<size_t>{});
    }else{
        for (size_t i=0; i<this->vertices.size();i++)
        {
            if (abs(this->vertices[i].first - x)<5 && abs(this->vertices[i].second - y)<5){
                this->isMoving= true;
                this->moving = i;
            }
        }
    }

    this->update();
    this->updateComboBoxes();
}

void MyField::mouseMoveEvent(QMouseEvent *event){
    int x = event->x();
    int y = event->y();

    if (isMoving){
        this->vertices[this->moving].first = x;
        this->vertices[this->moving].second = y;
        this->update();
    }


}

void MyField::mouseReleaseEvent(QMouseEvent *event){
    int x = event->x();
    int y = event->y();
    if (isMoving){
        this->vertices[this->moving].first = x;
        this->vertices[this->moving].second = y;
        this->isMoving = false;
        this->update();
    }
}

void MyField::makeColorful(std::vector<int> perm){

    std::vector<std::vector<int>> new_colors;

    new_colors.push_back({255,255,255});
    new_colors.push_back({255,0,0});
    new_colors.push_back({0,255,0});
    new_colors.push_back({0,0,255});
    new_colors.push_back({255,255,0});
    new_colors.push_back({255,0,255});
    new_colors.push_back({0,255,255});
    new_colors.push_back({128,0,0});
    new_colors.push_back({0,128,0});
    new_colors.push_back({0,0,128});

    new_colors.push_back({128,128,0});
    new_colors.push_back({128,0,128});
    new_colors.push_back({0,128,128});
    new_colors.push_back({153,153,255});
    new_colors.push_back({153,51,102});
    new_colors.push_back({255,255,204});
    new_colors.push_back({204,255,255});
    new_colors.push_back({255,128,128});
    new_colors.push_back({255,153,0});
    new_colors.push_back({0,0,0});

    int n = this->colors.size();
    this->colors.clear();

    for (int i=0; i<n; i++){
        this->colors.push_back(new_colors[perm[i]]);
    }
    this->update();
}

void MyField::clear(){
    this->vertices.clear();
    this->edgesByVertex.clear();
    this->colors.clear();
    this->update();
}
