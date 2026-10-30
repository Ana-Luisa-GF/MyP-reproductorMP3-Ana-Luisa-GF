#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include "RepControlador.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(RepControlador *ctrl, QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_actionAgregar_Artista_triggered();

    void on_actionMinar_triggered();

private:
    Ui::MainWindow *ui;
    RepControlador *controlador;
};
#endif // MAINWINDOW_H
