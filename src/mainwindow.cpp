#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QInputDialog>
#include "TarjetaCancion.h"
#include "cambiartitulo.h"
#include "cambiaralbum.h"
#include <iostream>
#include <QMessageBox>
#include <QDate>

MainWindow::MainWindow(RepControlador *ctrl, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , controlador(ctrl)
{
    ui->setupUi(this);

    QLineEdit *barraBusqueda = new QLineEdit(this);
    barraBusqueda->setPlaceholderText("Buscar...");
    barraBusqueda->setFixedSize(750, 35);
    barraBusqueda->setStyleSheet(
        "QLineEdit {"
        "   font-size: 14px;"
        "   padding-left: 8px;"
        "   border-radius: 5px;"
        "   border: 1px solid #ccc;"
        "}"
        );
    ui->mainToolBar->addWidget(barraBusqueda);
    ui->mainToolBar->setIconSize(QSize(45, 45));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAgregar_Artista_triggered()
{

}


void MainWindow::on_actionMinar_triggered()
{
    QString ruta = QInputDialog::getText(this, "Directorio","Ingrese la ruta del directorio a minar. ");
    controlador->minar(ruta.toStdString());
    rolas = controlador->getCanciones();
    cargarListaCanciones(rolas);
}


void MainWindow::cargarListaCanciones(std::vector<Cancion> rolas){
    ui->listWidget->clear();
    ui->listWidget->setViewMode(QListWidget::IconMode);
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    ui->listWidget->setGridSize(QSize(180, 180));
    ui->listWidget->setWordWrap(true);

    for (const Cancion& cancion : rolas) {
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);

        TarjetaCancion* tarjeta = new TarjetaCancion(ui->listWidget);
        tarjeta->establecerDatos(cancion.titulo, cancion.portada);

        item->setSizeHint(tarjeta->sizeHint());
        ui->listWidget->setItemWidget(item, tarjeta);
        connect(tarjeta, &TarjetaCancion::click, this, [this, cancion](const std::string &titulo) {
            this->cancionClickeada(cancion);
        });
    }
}


void MainWindow::cancionClickeada(const Cancion& cancion)
{
    cancion_selec = cancion;
    qDebug() << "¡Se hizo clic en una canción!";
    QString rutaQt = QString::fromStdString(cancion.portada.string());

    if (!rutaQt.isEmpty() && QFile::exists(rutaQt)) {
        QPixmap pixmap(rutaQt);
        //ui->pantalla->setPixmap(pixmap.scaled(ui->pantalla->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        ui->pantalla->setPixmap(pixmap.scaled(ui->pantalla->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    } else {
        QPixmap pixmapDefault(":/iconos/letras-de-canciones.png");
        //ui->pantalla->setPixmap(pixmapDefault.scaled(ui->pantalla->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        ui->pantalla->setPixmap(pixmapDefault.scaled(ui->pantalla->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }

    int anchoDisponible = 261 - 75;

    QFontMetrics metrics1(ui->caja_titulo->font());
    QString titulo = QString::fromStdString(cancion.titulo);
    QString titulo_cortado = metrics1.elidedText(titulo, Qt::ElideRight, anchoDisponible);

    ui->caja_titulo->setText(
        "<span style='color: #b0b0b0; font-size: 10pt; font-weight: normal;'>Título: </span>"
        "<span style='color: #ffffff; font-size: 11pt; font-weight: bold;'>" + titulo_cortado + "</span>"
        );

    QFontMetrics metrics2(ui->caja_album->font());
    QString album = QString::fromStdString(cancion.album);
    QString album_cortado = metrics2.elidedText(album, Qt::ElideRight, anchoDisponible);

    ui->caja_album->setText(
        "<span style='color: #b0b0b0; font-size: 10pt; font-weight: normal;'>Album: </span>"
        "<span style='color: #ffffff; font-size: 11pt; font-weight: bold;'>" + album_cortado + "</span>"
        );

    QFontMetrics metrics3(ui->caja_genero->font());
    QString genero = QString::fromStdString(cancion.genero);
    QString genero_cortado = metrics3.elidedText(genero, Qt::ElideRight, anchoDisponible);
    ui->caja_genero->setText(
        "<span style='color: #b0b0b0; font-size: 10pt; font-weight: normal;'>Genero: </span>"
        "<span style='color: #ffffff; font-size: 11pt; font-weight: bold;'>" + genero_cortado+ "</span>"
        );

    std::string texto1 = "<span style='color: #b0b0b0; font-size: 10pt; font-weight: normal;'>Año: </span>  <span style='color: #ffffff; font-size: 11pt; font-weight: bold;'>";

    std::string texto2 = texto1+std::to_string(cancion.anio)+"</span>";
    ui->caja_anio->setText(texto2.c_str());



    QFontMetrics metrics4(ui->caja_artista->font());
    QString artista = QString::fromStdString(cancion.artista);
    QString artista_cortado = metrics4.elidedText(artista, Qt::ElideRight, anchoDisponible);
    ui->caja_artista->setText(
        "<span style='color: #b0b0b0; font-size: 10pt; font-weight: normal;'>Artista: </span>"
        "<span style='color: #ffffff; font-size: 11pt; font-weight: bold;'>" + artista_cortado+ "</span>"
        );

    std::string texto3 = "<span style='color: #b0b0b0; font-size: 10pt; font-weight: normal;'>Track: </span>  <span style='color: #ffffff; font-size: 11pt; font-weight: bold;'>";

    std::string texto4 = texto3+std::to_string(cancion.track)+"</span>";
    ui->caja_track->setText(texto4.c_str());

}

void MainWindow::refrescar(){
    rolas = controlador->getCanciones();
    cargarListaCanciones(rolas);
    Cancion c;
    for(const Cancion &rola : rolas)
        if(rola.id == cancion_selec.id)
            cancion_selec = rola;
    cancionClickeada(cancion_selec);
}

void MainWindow::on_editButton_titulo_clicked()
{
    QString mensaje="";
    if(!cancion_selec.ruta.empty())
        mensaje =  QString::fromStdString(cancion_selec.titulo);

    CambiarTitulo ventana_titulo("Titulo de la canción",mensaje);
    ventana_titulo.setModal(true);
    if(ventana_titulo.exec() == QDialog::Accepted){
        if(!controlador->modificarTitulo(cancion_selec.id,ventana_titulo.getResultado().toStdString()))
            QMessageBox::critical(this, "Error", "Algo salio mal al comunicarse con la base de datos.");
        refrescar();
    }
}


void MainWindow::on_editButton_artista_clicked()
{
    QString mensaje="";
    if(!cancion_selec.ruta.empty())
        mensaje =  QString::fromStdString(cancion_selec.album);

    CambiarTitulo ventana_titulo("Album de la canción",mensaje);
    ventana_titulo.setModal(true);
    ventana_titulo.exec();
}


void MainWindow::on_editButton_album_clicked()
{   
    QString nombre="";
    Album album;
    if(!cancion_selec.ruta.empty())
        album = controlador->getAlbum(cancion_selec.id_album);
    else
        album.anio = 0;

    nombre =  QString::fromStdString(album.nombre);
    CambiarAlbum ventana(album.anio,nombre);
    ventana.setModal(true);

    if(ventana.exec() == QDialog::Accepted && !cancion_selec.ruta.empty()){
        int anioActual = QDate::currentDate().year();
        if(!ventana.getConversionAnio() || ventana.getNuenoAnio() < 0 || ventana.getNuenoAnio() >anioActual ){
            QMessageBox::critical(this, "Error", "El año proporcionado no es valido.");
            return;
        }
        if(!controlador->mofificarAlbum(album.id,"name",ventana.getNuenoAnio(),ventana.getNuevoAlbum().toStdString()))
            QMessageBox::critical(this, "Error", "Algo salio mal al comunicarse con la base de datos.");

        if(!controlador->mofificarAlbum(album.id,"year",ventana.getNuenoAnio(),ventana.getNuevoAlbum().toStdString()))
            QMessageBox::critical(this, "Error", "Algo salio mal al comunicarse con la base de datos.");
        refrescar();
    }
}


void MainWindow::on_editButton_genero_clicked()
{
    QString mensaje="";
    if(!cancion_selec.ruta.empty())
        mensaje =  QString::fromStdString(cancion_selec.genero);

    CambiarTitulo ventana_titulo("Género de la canción",mensaje);
    ventana_titulo.setModal(true);

    if(ventana_titulo.exec() == QDialog::Accepted && !cancion_selec.ruta.empty()){
        if(!controlador->modificarGenero(cancion_selec.id,ventana_titulo.getResultado().toStdString()))
            QMessageBox::critical(this, "Error", "Algo salio mal al comunicarse con la base de datos.");
        refrescar();
    }
}




void MainWindow::on_editButton_anio_clicked()
{
    QString mensaje="";
    if(!cancion_selec.ruta.empty())
        mensaje =  QString::number(cancion_selec.anio);

    CambiarTitulo ventana_titulo("Año de la canción",mensaje);
    ventana_titulo.setModal(true);

    if(ventana_titulo.exec() == QDialog::Accepted && !cancion_selec.ruta.empty()){
        int anioActual = QDate::currentDate().year();
        bool conversion_anio;
        int anio = ventana_titulo.getResultado().toInt(&conversion_anio);

        if(!conversion_anio || anio < 0 || anio > anioActual ){
            QMessageBox::critical(this, "Error", "El año proporcionado no es valido.");
            return;
        }

        if(!controlador->modificarAnio(cancion_selec.id,ventana_titulo.getResultado().toStdString()))
            QMessageBox::critical(this, "Error", "Algo salio mal al comunicarse con la base de datos.");

        refrescar();
    }
}


void MainWindow::on_editButton_track_clicked()
{
    QString mensaje="";
    if(!cancion_selec.ruta.empty())
        mensaje =  QString::number(cancion_selec.track);

    CambiarTitulo ventana_titulo("Track de la canción",mensaje);
    ventana_titulo.setModal(true);

    if(ventana_titulo.exec() == QDialog::Accepted && !cancion_selec.ruta.empty()){

        bool conversion_entero;
        int num = ventana_titulo.getResultado().toInt(&conversion_entero);

        if(!conversion_entero || num < 0){
            QMessageBox::critical(this, "Error", "El track proporcionado no es valido.");
            return;
        }

        if(!controlador->modificarTrack(cancion_selec.id,ventana_titulo.getResultado().toStdString()))
            QMessageBox::critical(this, "Error", "Algo salio mal al comunicarse con la base de datos.");
        refrescar();
    }
}

