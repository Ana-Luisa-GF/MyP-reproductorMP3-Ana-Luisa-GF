#include "cambiargroup.h"
#include "ui_cambiargroup.h"
#include <QDate>
#include <QMessageBox>

CambiarGroup::CambiarGroup(const QString &nombre, const QString &fechaCrea,const QString &fechaDis, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CambiarGroup)
{
    ui->setupUi(this);
    ui->lineEdit_nombre->setText(nombre);
    ui->lineEdit_fecha_creacion->setText(fechaCrea);
    ui->lineEdit_fecha_disolucion->setText(fechaDis);
}

CambiarGroup::~CambiarGroup()
{
    delete ui;
}

void CambiarGroup::on_realizado_clicked()
{
    nombre = ui->lineEdit_nombre->text();
    fecha_creacion = ui->lineEdit_fecha_creacion->text();
    fecha_disolucion = ui->lineEdit_fecha_disolucion->text();

    QDate hoy = QDate::currentDate();
    QDate fechaCreacion = QDate::fromString(fecha_creacion, "dd/MM/yyyy");


    if (!fechaCreacion.isValid()) {
        QMessageBox::critical(this, "Error", "Formato incorrecto o fecha de creación inexistente.");
        reject();
        return;
    }

    if(fechaCreacion > hoy){
        QMessageBox::critical(this, "Error", "La fecha de creación no puede ser futura (mayor a hoy).");
        reject();
        return;
    }

    if (!fecha_disolucion.trimmed().isEmpty()){
        hayFechaDisolucion = true;
        QDate fechaDisolucion = QDate::fromString(fecha_disolucion, "dd/MM/yyyy");

        if (!fechaDisolucion.isValid()) {
            QMessageBox::critical(this, "Error", "Formato incorrecto o fecha de disolución inexistente.");
            reject();
            return;
        }
        if(fechaDisolucion > hoy){
            QMessageBox::critical(this, "Error", "La fecha de disolución no puede ser futura (mayor a hoy).");
            reject();
            return;
        }

        if (fechaDisolucion < fechaCreacion) {
            QMessageBox::critical(this, "Error", "La fecha de disolución no puede ser anterior a la fecha de creación.");
            reject();
            return;
        }
    }else{
        hayFechaDisolucion = false;
    }

    accept();
}


void CambiarGroup::on_rechazado_clicked()
{
    reject();
}

