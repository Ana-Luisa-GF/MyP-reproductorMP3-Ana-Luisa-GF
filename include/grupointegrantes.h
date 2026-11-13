#ifndef GRUPOINTEGRANTES_H
#define GRUPOINTEGRANTES_H
#include "Performer.h"
#include "Person.h"
#include <QDialog>

namespace Ui {
class GrupoIntegrantes;
}

class GrupoIntegrantes : public QDialog
{
    Q_OBJECT

public:
    explicit GrupoIntegrantes(std::vector<Person> &artistas, std::vector<Performer> &integrantes, QWidget *parent = nullptr);
    ~GrupoIntegrantes();
    void cargarListaIntegrantes();
    bool getAccion(){return accion;}
    QString getArtista(){return textoArtista;}
    bool getNuevoArtista(){return esNuevoArtista;}
    int getIdArtista(){return id_artista;}

private slots:
    void on_realizado_clicked();

    void on_rechazado_clicked();

private:
    Ui::GrupoIntegrantes *ui;
    std::vector<Person> artistas;
    std::vector<Performer> integrantes;
    bool accion;
    QString textoArtista;
    int id_artista;
    bool esNuevoArtista;

};

#endif // GRUPOINTEGRANTES_H
