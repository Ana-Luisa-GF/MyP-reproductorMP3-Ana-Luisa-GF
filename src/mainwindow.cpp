#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAgregar_Artista_triggered()
{

}


void MainWindow::on_actionMinar_triggered()
{
    QString ruta = QInputDialog::getText(this, "Directorio","Ingrese la ruta del directorio a minar. ");
}

