#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "RepControlador.h"
#include "Cancion.h"


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
    void cargarListaCanciones(std::vector<Cancion>);


private slots:
    void on_actionAgregar_Artista_triggered();

    void on_actionMinar_triggered();

    void cancionClickeada(const Cancion& cancion);

private:
    Ui::MainWindow *ui;
    RepControlador *controlador;

};
#endif // MAINWINDOW_H
