#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "graph.h"
#include "vf2plus.h"
#include "ri.h"
#include "my_nauty.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void setComboBoxes1();
    void setComboBoxes2();

private slots:
    void on_addEdgeButton_1_clicked();

    void on_addEdgeButton_2_clicked();

    void on_addVertexButton1_1_clicked();

    void on_addVertexButton1_2_clicked();

    void on_addVertexButton2_1_clicked();

    void on_addVertexButton2_2_clicked();

    void on_openFileButton_1_clicked();

    void on_openFileButton_2_clicked();

    void on_compIzoButton1_clicked();

    void on_clearButton_1_clicked();

    void on_clearButton_2_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
