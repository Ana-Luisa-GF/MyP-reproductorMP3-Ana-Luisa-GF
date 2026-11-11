#include "cambiartitulo.h"
#include "ui_cambiartitulo.h"
#include <QLabel>

CambiarTitulo::CambiarTitulo(const QString &titulo_ventana, const QString &titulo_cancion, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CambiarTitulo)
{
    ui->setupUi(this);
    setWindowTitle(titulo_ventana);
    this->titulo_cancion = titulo_cancion;
    ui->lineEdit->setText(titulo_cancion);

   if(titulo_ventana == "Titulo de la canción")
        ui->label->setText("<html><head/><body><p align='center'><span style='font-size:14pt; "
                          "font-weight:700; color:#1E90FF;'>EDITAR TITULO:</span></p></body></html>");

   if(titulo_ventana == "Género de la canción")
        ui->label->setText("<html><head/><body><p align='center'><span style='font-size:14pt; "
                          "font-weight:700; color:#1E90FF;'>EDITAR GÉNERO:</span></p></body></html>");

   if(titulo_ventana == "Track de la canción")
       ui->label->setText("<html><head/><body><p align='center'><span style='font-size:14pt; "
                          "font-weight:700; color:#1E90FF;'>EDITAR TRACK:</span></p></body></html>");

   if(titulo_ventana == "Año de la canción")
        ui->label->setText("<html><head/><body><p align='center'><span style='font-size:10pt; "
                          "font-weight:700; color:#1E90FF;'>EDITAR AÑO DE SALIDA:</span></p></body></html>");

}

CambiarTitulo::~CambiarTitulo()
{
    delete ui;
}

void CambiarTitulo::on_realizado_clicked()
{
    resultado = ui->lineEdit->text();
    accept();
}


void CambiarTitulo::on_realizado_2_clicked()
{
    reject();
}

QString CambiarTitulo::getResultado()const {return resultado;}
