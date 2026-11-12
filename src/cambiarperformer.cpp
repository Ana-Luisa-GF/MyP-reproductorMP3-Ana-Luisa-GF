#include "cambiarperformer.h"
#include "ui_cambiarperformer.h"

#include <QMessageBox>

cambiarPerformer::cambiarPerformer(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::cambiarPerformer)
{
    ui->setupUi(this);
    setWindowTitle("Tipo de performer");
}

cambiarPerformer::~cambiarPerformer()
{
    delete ui;
}

void cambiarPerformer::on_realizado_clicked()
{
    if(ui->radioButton_grupo->isChecked()){
        tipo = ui->radioButton_grupo->text();
        accept();
        return;
    }

    if(ui->radioButton_artista->isChecked()){
        tipo = ui->radioButton_artista->text();
        accept();
        return;
    }

    QMessageBox::critical(this, "Error", "No se selecciono ninguna opción.");
    reject();

}


void cambiarPerformer::on_realizado_2_clicked()
{
    reject();
}

