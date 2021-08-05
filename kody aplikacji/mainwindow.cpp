#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myfield.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->widget_1, SIGNAL(updateComboBoxes()), this, SLOT(setComboBoxes1()));
    connect(ui->widget_2, SIGNAL(updateComboBoxes()), this, SLOT(setComboBoxes2()));
    ui->addVertexButton1_2->setEnabled(false);
    ui->addVertexButton2_2->setEnabled(false);

    ui->giComboBox->addItem("VF2Plus");
    ui->giComboBox->addItem("RI");
    ui->giComboBox->addItem("ir algorithm");

    QPalette palette;
    palette.setColor(backgroundRole(), QColor(49, 63, 179));
    setPalette(palette);
    setAutoFillBackground(true);
    this->setWindowTitle("Solve isomorphism problem");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setComboBoxes1(){
    ui->addEdgeComboBox1_1->clear();
    ui->addEdgeComboBox1_2->clear();
    size_t n = ui->widget_1->getNumberOfVertex();
    for (size_t i=0; i<n; i++){
        ui->addEdgeComboBox1_1->addItem(QString::number(i));
        ui->addEdgeComboBox1_2->addItem(QString::number(i));
    }
}

void MainWindow::setComboBoxes2(){
    ui->addEdgeComboBox2_1->clear();
    ui->addEdgeComboBox2_2->clear();
    size_t n = ui->widget_2->getNumberOfVertex();
    for (size_t i=0; i<n; i++){
        ui->addEdgeComboBox2_1->addItem(QString::number(i));
        ui->addEdgeComboBox2_2->addItem(QString::number(i));
    }
}


void MainWindow::on_addEdgeButton_1_clicked()
{
    size_t a = size_t(ui->addEdgeComboBox1_1->currentText().toInt());
    size_t b = size_t(ui->addEdgeComboBox1_2->currentText().toInt());
    ui->widget_1->addEdge(a,b);
}

void MainWindow::on_addEdgeButton_2_clicked()
{
    size_t a = size_t(ui->addEdgeComboBox2_1->currentText().toInt());
    size_t b = size_t(ui->addEdgeComboBox2_2->currentText().toInt());
    ui->widget_2->addEdge(a,b);
}

void MainWindow::on_addVertexButton1_1_clicked()
{
    ui->addVertexButton1_1->setEnabled(false);
    ui->addVertexButton1_2->setEnabled(true);
    ui->widget_1->setCanBeAdded(true);
}

void MainWindow::on_addVertexButton1_2_clicked()
{
    ui->addVertexButton1_2->setEnabled(false);
    ui->addVertexButton1_1->setEnabled(true);
    ui->widget_1->setCanBeAdded(false);
}

void MainWindow::on_addVertexButton2_1_clicked()
{
    ui->addVertexButton2_1->setEnabled(false);
    ui->addVertexButton2_2->setEnabled(true);
    ui->widget_2->setCanBeAdded(true);
}

void MainWindow::on_addVertexButton2_2_clicked()
{
    ui->addVertexButton2_2->setEnabled(false);
    ui->addVertexButton2_1->setEnabled(true);
    ui->widget_2->setCanBeAdded(false);
}

void MainWindow::on_openFileButton_1_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,"Open file",QDir::homePath());
    ui->widget_1->loadFromFile(filename);
    this->setComboBoxes1();
}

void MainWindow::on_openFileButton_2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,"Open file",QDir::homePath());
    ui->widget_2->loadFromFile(filename);
    this->setComboBoxes2();
}

void MainWindow::on_compIzoButton1_clicked()
{
    int n1 = ui->widget_1->getNumberOfVertex();
    int n2 = ui->widget_2->getNumberOfVertex();
    Graph * g1 = new Graph(n1);
    Graph * g2 = new Graph(n2);

    std::vector<std::pair<int,int>> edges1;
    std::vector<std::pair<int,int>> edges2;

    std::vector<std::vector<size_t>> ebv1 = ui->widget_1->getEdges();
    std::vector<std::vector<size_t>> ebv2 = ui->widget_2->getEdges();

    for (size_t i= 0; i<ebv1.size(); i++){
        for (auto j: ebv1[i]){
            if (i<j)
                edges1.push_back(std::pair<int,int>(i,j));
        }
    }
    for (size_t i= 0; i<ebv2.size(); i++){
        for (auto j: ebv2[i]){
            if (i<j)
                edges2.push_back(std::pair<int,int>(i,j));
        }
    }
    g1->addEdges(edges1);
    g2->addEdges(edges2);

    std::map<int,int> res;

    if (ui->giComboBox->currentIndex() == 0)
        res= vf2plus(g1,g2);
    if (ui->giComboBox->currentIndex() == 1)
        res= ri_gi(g1,g2);
    if (ui->giComboBox->currentIndex() == 2)
        res= irAlg(*g1,*g2);

    this->ui->textIso->clear();
    if (res.size()==0){
        qDebug() << "brak izomorfizmu";
        this->ui->textIso->append("Brak izomorfizmu");

    }
    else{
        qDebug() << "Izomorfizm:";
        this->ui->textIso->append("Znaleziono izomorfizm: ");

        std::vector<int> perm1, perm2(res.size(),0);
        for(int i=0; i<res.size();++i){
            perm1.push_back(i);
            perm2[res[i]]=i;
        }
        this->ui->widget_1->makeColorful(perm1);
        this->ui->widget_2->makeColorful(perm2);
    }
    for (auto el: res){
        qDebug() <<"el: "<<el.first<<" "<<el.second;
        this->ui->textIso->append(QString::number(el.first)+"->"+QString::number(el.second));
    }

    delete g1;
    delete g2;
}

void MainWindow::on_clearButton_1_clicked()
{
    this->ui->widget_1->clear();
    this->ui->addEdgeComboBox1_1->clear();
    this->ui->addEdgeComboBox1_2->clear();
}

void MainWindow::on_clearButton_2_clicked()
{
    this->ui->widget_2->clear();
    this->ui->addEdgeComboBox2_1->clear();
    this->ui->addEdgeComboBox2_2->clear();
}
