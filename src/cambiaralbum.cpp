#include "cambiaralbum.h"
#include "ui_cambiaralbum.h"

CambiarAlbum::CambiarAlbum(const int &anio,const QString &titulo_album,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CambiarAlbum)
{
    ui->setupUi(this);
    setWindowTitle("Album de la canción");
    ui->lineEdit_nombre->setText(titulo_album);
    ui->lineEdit_anio->setText(QString::number(anio));
}

CambiarAlbum::~CambiarAlbum()
{
    delete ui;
}

QString CambiarAlbum::getNuevoAlbum() const{return titulo_nuevo;}

int CambiarAlbum::getNuenoAnio() const{return anio_nuevo;}

bool CambiarAlbum::getConversionAnio(){return conversion_anio;}

void CambiarAlbum::on_realizado_clicked()
{
    titulo_nuevo = ui->lineEdit_nombre->text();
    anio_nuevo = ui->lineEdit_anio->text().toInt(&conversion_anio);
    accept();
}


void CambiarAlbum::on_realizado_2_clicked()
{
    reject();
}

