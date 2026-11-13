#ifndef CAMBIARPERSON_H
#define CAMBIARPERSON_H

#include <QDialog>

namespace Ui {
class cambiarPerson;
}

class cambiarPerson : public QDialog
{
    Q_OBJECT

public:
    explicit cambiarPerson(const QString &nombreArt, const QString &nombreReal,const QString &fechaCumple,
                           const QString &fechaMuerte,QWidget *parent = nullptr);
    ~cambiarPerson();
    QString getNombreArt(){return nombre_art;}
    QString getNombreReal(){return nombre_real;}
    QString getFechaCumple(){return fecha_cumple;}
    QString getFechaMuerte(){return fecha_muerte;}
    bool existFechaMuerte(){return hayFechaMuerte;}

private slots:
    void on_realizado_clicked();

    void on_rechazado_clicked();

private:
    Ui::cambiarPerson *ui;
    QString nombre_art;
    QString nombre_real;
    QString fecha_cumple;
    QString fecha_muerte;
    bool hayFechaMuerte;
};

#endif // CAMBIARPERSON_H
