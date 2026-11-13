#ifndef CAMBIARGROUP_H
#define CAMBIARGROUP_H

#include <QDialog>

namespace Ui {
class CambiarGroup;
}

class CambiarGroup : public QDialog
{
    Q_OBJECT

public:
    explicit CambiarGroup(const QString &nombre, const QString &fechaCrea,const QString &fechaDis, QWidget *parent = nullptr);
    ~CambiarGroup();
    QString getNombre(){return nombre;}
    QString getFechaCreacion(){return fecha_creacion;}
    QString getFechaDisolucion(){return fecha_disolucion;}
    bool existFechaDisolucion(){return hayFechaDisolucion;}

private slots:
    void on_realizado_clicked();

    void on_rechazado_clicked();

private:
    Ui::CambiarGroup *ui;
    QString nombre;
    QString fecha_creacion;
    QString fecha_disolucion;
    bool hayFechaDisolucion;
};

#endif // CAMBIARGROUP_H
