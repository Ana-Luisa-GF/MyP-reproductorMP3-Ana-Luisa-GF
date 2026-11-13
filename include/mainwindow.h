#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "RepControlador.h"
#include "Cancion.h"
#include "Performer.h"
#include "Person.h"
#include "Group.h"


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
    void cargarListaAlbums(std::vector<Album> albums);
    void cargarListaPerformers(std::vector<Performer> performers);
    void modificarAlbum(const int &boton);
    void performerClickeado(const Performer& performer);
    void diseñoVentanaPerformer(const int &tipo_performer);

    void refrescar();




private slots:
    void on_actionAgregar_Artista_triggered();

    void on_actionMinar_triggered();

    void albumClickeado(const Album& album);



    void cancionClickeada(const Cancion& cancion);

    void on_editButton_titulo_clicked();

    void on_editButton_artista_clicked();

    void on_editButton_album_clicked();

    void on_editButton_genero_clicked();

    void on_editButton_anio_clicked();

    void on_editButton_track_clicked();

    void on_actionAlbums_triggered();

    void on_actionMusica_triggered();


    void on_editButton_albumDatos_clicked();

    void on_actionPerformers_triggered();

    void on_editButton_performer_clicked();

    void on_editButton_person_clicked();

    void on_editButton_grupo_clicked();

    void on_editButton_integrantes_grupo_clicked();

private:
    Ui::MainWindow *ui;
    RepControlador *controlador;
    Cancion cancion_selec;
    Album album_selec;
    Performer performer_selec;
    Person artista_selec;
    Group grupo_selec;
    std::vector<Cancion> rolas;
};
#endif // MAINWINDOW_H
