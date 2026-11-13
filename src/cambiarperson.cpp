#include "cambiarperson.h"
#include "ui_cambiarperson.h"
#include <QDate>
#include <QMessageBox>

cambiarPerson::cambiarPerson(const QString &nombreArt, const QString &nombreReal,const QString &fechaCumple,
                             const QString &fechaMuerte,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::cambiarPerson)
{
    ui->setupUi(this);
    ui->lineEdit_nombre_artistico->setText(nombreArt);
    ui->lineEdit_nombre_real->setText(nombreReal);
    ui->lineEdit_fecha_nacimiento->setText(fechaCumple);
    ui->lineEdit_fecha_muerte->setText(fechaMuerte);
}

cambiarPerson::~cambiarPerson()
{
    delete ui;
}

void cambiarPerson::on_realizado_clicked()
{
    nombre_art = ui->lineEdit_nombre_artistico->text();
    nombre_real = ui->lineEdit_nombre_real->text();
    fecha_cumple = ui->lineEdit_fecha_nacimiento->text();
    fecha_muerte = ui->lineEdit_fecha_muerte->text();

    QDate hoy = QDate::currentDate();
    QDate fechaCumple = QDate::fromString(fecha_cumple, "dd/MM/yyyy");


    if (!fechaCumple.isValid()) {
        QMessageBox::critical(this, "Error", "Formato incorrecto o fecha de nacimiento inexistente.");
        reject();
        return;
    }

    if(fechaCumple > hoy){
        QMessageBox::critical(this, "Error", "La fecha de nacimiento no puede ser futura (mayor a hoy).");
        reject();
        return;
    }

    if (!fecha_muerte.trimmed().isEmpty()){
        hayFechaMuerte = true;
        QDate fechaMuerte = QDate::fromString(fecha_muerte, "dd/MM/yyyy");

        if (!fechaMuerte.isValid()) {
            QMessageBox::critical(this, "Error", "Formato incorrecto o fecha de muerte inexistente.");
            reject();
            return;
        }
        if(fechaMuerte > hoy){
            QMessageBox::critical(this, "Error", "La fecha de muerte no puede ser futura (mayor a hoy).");
            reject();
            return;
        }

        if (fechaMuerte < fechaCumple) {
            QMessageBox::critical(this, "Error", "La fecha de muerte no puede ser anterior a la fecha de nacimiento.");
            reject();
            return;
        }
    }else{
        hayFechaMuerte = false;
    }

    accept();
}


void cambiarPerson::on_rechazado_clicked()
{
    reject();
}

