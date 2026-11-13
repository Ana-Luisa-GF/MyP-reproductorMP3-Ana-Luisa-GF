#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QInputDialog>
#include "TarjetaCancion.h"
#include "cambiartitulo.h"
#include "cambiaralbum.h"
#include "cambiarperformer.h"
#include "cambiarperson.h"
#include "cambiargroup.h"
#include "grupointegrantes.h"
#include <iostream>
#include <QMessageBox>
#include <QDate>

MainWindow::MainWindow(RepControlador *ctrl, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , controlador(ctrl)
{
    ui->setupUi(this);

    barraBusqueda = new QLineEdit(this);
    barraBusqueda->setPlaceholderText("Buscar...");
    barraBusqueda->setFixedSize(675, 35);
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

    connect(barraBusqueda, &QLineEdit::returnPressed, this, [this]() {
    this->ejecutarBusqueda(barraBusqueda->text());
    });


    std::vector<Album> albums = controlador->getAlbums();
    cargarListaAlbums(albums);
    std::vector<Performer> performers = controlador->getPerformers();
    cargarListaPerformers(performers);
    setListas(controlador->getCanciones(), albums, performers);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionMinar_triggered()
{
    QString ruta = QInputDialog::getText(this, "Directorio","Ingrese la ruta del directorio a minar. ");
    controlador->minar(ruta.toStdString());
    refrescar();

}

void MainWindow::cargarListaCanciones(std::vector<Cancion> rolas){
    ui->listWidget->clear();
    ui->listWidget->setViewMode(QListWidget::IconMode);
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    ui->listWidget->setGridSize(QSize(180, 180));
    ui->listWidget->setWordWrap(true);

    for (const Cancion& cancion : rolas) {
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);

        TarjetaCancion* tarjeta = new TarjetaCancion(168,168, ui->listWidget);
        tarjeta->establecerDatos(":/iconos/letras-de-canciones.png",cancion.titulo, cancion.portada);

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
        ui->pantalla->setPixmap(pixmap.scaled(ui->pantalla->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    } else {
        QPixmap pixmapDefault(":/iconos/letras-de-canciones.png");
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

void MainWindow::setListas(std::vector<Cancion> r, std::vector<Album> a, std::vector<Performer> p){
    rolas = r;
    albums = a;
    performers = p;
}

void MainWindow::refrescar(){
    cargarListaCanciones(controlador->getCanciones());

    for(const Cancion &rola : rolas)
        if(rola.id == cancion_selec.id)
            cancionClickeada(rola);

    cargarListaAlbums(controlador->getAlbums());

    if(album_selec.album_valido)
        album_selec = controlador->getAlbum(album_selec.id);
    albumClickeado(album_selec);

    cargarListaPerformers(controlador->getPerformers());

    if(performer_selec.performer_valido)
        performer_selec = controlador->getPerformer(performer_selec.id);
    performerClickeado(performer_selec);
}

void MainWindow::refrescar_busqueda(){
    cargarListaCanciones(rolas);

    for(const Cancion &rola : rolas)
        if(rola.id == cancion_selec.id)
    cancionClickeada(rola);

    cargarListaAlbums(albums);

    if(album_selec.album_valido)
    album_selec = controlador->getAlbum(album_selec.id);
    albumClickeado(album_selec);

    cargarListaPerformers(performers);

    if(performer_selec.performer_valido)
    performer_selec = controlador->getPerformer(performer_selec.id);
    performerClickeado(performer_selec);
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
        if(busqueda_valida)
            ejecutarBusqueda(busqueda);
        else
            refrescar();
    }
}


void MainWindow::on_editButton_artista_clicked()
{
    performer_selec = controlador->getPerformer(cancion_selec.id_performer);
    if(performer_selec.id_type == 0){
        on_editButton_person_clicked();
        return;
    }

    if(performer_selec.id_type == 2){
        on_editButton_performer_clicked();
        return;
    }

    on_editButton_grupo_clicked();
}

void MainWindow::modificarAlbum(const int &boton){
    QString nombre="";
    Album album;

    if(boton == 1){
        if(!cancion_selec.ruta.empty())
            album = controlador->getAlbum(cancion_selec.id_album);
        else
            album.anio = 0;
    }else
        album = album_selec;


    nombre =  QString::fromStdString(album.nombre);
    CambiarAlbum ventana(album.anio,nombre);
    ventana.setModal(true);

    if(ventana.exec() == QDialog::Accepted && ((boton ==1 && !cancion_selec.ruta.empty()) || (boton==2 && album_selec.album_valido))){
        int anioActual = QDate::currentDate().year();
        if(!ventana.getConversionAnio() || ventana.getNuenoAnio() < 0 || ventana.getNuenoAnio() >anioActual ){
            QMessageBox::critical(this, "Error", "El año proporcionado no es valido.");
            return;
        }
        if(!controlador->mofificarAlbum(album.id,"name",ventana.getNuenoAnio(),ventana.getNuevoAlbum().toStdString()))
            QMessageBox::critical(this, "Error", "Algo salio mal al comunicarse con la base de datos.");

        if(!controlador->mofificarAlbum(album.id,"year",ventana.getNuenoAnio(),ventana.getNuevoAlbum().toStdString()))
            QMessageBox::critical(this, "Error", "Algo salio mal al comunicarse con la base de datos.");

        if(busqueda_valida)
            ejecutarBusqueda(busqueda);
        else
            refrescar();
    }
}

void MainWindow::on_editButton_album_clicked()
{
    modificarAlbum(1);
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
        if(busqueda_valida)
            ejecutarBusqueda(busqueda);
        else
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

        if(busqueda_valida)
            ejecutarBusqueda(busqueda);
        else
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
        if(busqueda_valida)
            ejecutarBusqueda(busqueda);
        else
            refrescar();
    }
}


void MainWindow::on_actionAlbums_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
}



void MainWindow::on_actionMusica_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
}



void MainWindow::on_editButton_albumDatos_clicked()
{
   modificarAlbum(2);
}

void MainWindow::cargarListaAlbums(std::vector<Album> albums){

    ui->listWidget_albums->clear();
    ui->listWidget_albums->setViewMode(QListWidget::IconMode);
    ui->listWidget_albums->setResizeMode(QListWidget::Adjust);
    ui->listWidget_albums->setGridSize(QSize(223, 223));
    ui->listWidget_albums->setWordWrap(true);

    for (const Album& alb : albums) {
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget_albums);

        TarjetaCancion* tarjeta = new TarjetaCancion(221,221, ui->listWidget_albums);
        tarjeta->establecerDatos(":/iconos/album-base.png", alb.nombre, alb.portada);

        item->setSizeHint(tarjeta->sizeHint());
        ui->listWidget_albums->setItemWidget(item, tarjeta);
        connect(tarjeta, &TarjetaCancion::click, this, [this, alb](const std::string &titulo) {
            this->albumClickeado(alb);
        });
    }
}

void MainWindow::albumClickeado(const Album& album){
    album_selec = album;
    QString rutaQt = QString::fromStdString(album.portada);

    if (!rutaQt.isEmpty() && QFile::exists(rutaQt)) {
        QPixmap pixmap(rutaQt);
        ui->pantalla_album->setPixmap(pixmap.scaled(ui->pantalla_album->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    } else {
        QPixmap pixmapDefault(":/iconos/album-base.png");
        ui->pantalla_album->setPixmap(pixmapDefault.scaled(ui->pantalla_album->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }

    int anchoDisponible = 261 - 75;

    QFontMetrics metrics1(ui->caja_titulo_Album->font());
    QString titulo = QString::fromStdString(album.nombre);
    QString titulo_cortado = metrics1.elidedText(titulo, Qt::ElideRight, anchoDisponible);

    ui->caja_titulo_Album->setText(
        "<span style='color: #b0b0b0; font-size: 11pt; font-weight: normal;'>Título: </span>"
        "<span style='color: #ffffff; font-size: 12pt; font-weight: bold;'>" + titulo_cortado + "</span>"
        );

    std::string texto1 = "<span style='color: #b0b0b0; font-size: 11pt; font-weight: normal;'>Año: </span>  <span style='color: #ffffff; font-size: 12pt; font-weight: bold;'>";
    std::string texto2 = texto1+std::to_string(album.anio)+"</span>";
    ui->caja_anio_Album->setText(texto2.c_str());

}

void MainWindow::on_actionPerformers_triggered()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::cargarListaPerformers(std::vector<Performer> performers){

    ui->listWidget_performers->clear();
    ui->listWidget_performers->setViewMode(QListWidget::IconMode);
    ui->listWidget_performers->setResizeMode(QListWidget::Adjust);
    ui->listWidget_performers->setGridSize(QSize(223, 223));
    ui->listWidget_performers->setWordWrap(true);

    for (const Performer& perf : performers) {
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget_performers);

        TarjetaCancion* tarjeta = new TarjetaCancion(221,221, ui->listWidget_performers);

        if(perf.id_type == 1)
            tarjeta->establecerDatos(":/iconos/grupo.png", perf.nombre, perf.ruta_imagen);
        else
            if(perf.id_type == 0)
              tarjeta->establecerDatos(":/iconos/artista_base.png", perf.nombre, perf.ruta_imagen);
            else
            tarjeta->establecerDatos(":/iconos/performer.png", perf.nombre, perf.ruta_imagen);

        item->setSizeHint(tarjeta->sizeHint());
        ui->listWidget_performers->setItemWidget(item, tarjeta);
        connect(tarjeta, &TarjetaCancion::click, this, [this, perf](const std::string &titulo) {
            this->performerClickeado(perf);
        });
    }
}


void MainWindow::performerClickeado(const Performer& performer){
    performer_selec = performer;
    QString rutaQt = QString::fromStdString(performer.ruta_imagen);

    if (!rutaQt.isEmpty() && QFile::exists(rutaQt)) {
        QPixmap pixmap(rutaQt);
        ui->pantalla_performer->setPixmap(pixmap.scaled(ui->pantalla_performer->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    } else {
        QPixmap pixmapDefault;
        if(performer.id_type == 0)
            pixmapDefault.load(":/iconos/artista_base.png");
        else
            if(performer.id_type == 1)
                pixmapDefault.load(":/iconos/grupo.png");
            else
                pixmapDefault.load(":/iconos/performer.png");
        ui->pantalla_performer->setPixmap(pixmapDefault.scaled(ui->pantalla_performer->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }

    diseñoVentanaPerformer(performer_selec.id_type);
}


void MainWindow::diseñoVentanaPerformer(const int &tipo_performer){
    if(tipo_performer == 2){

        ui->stackedWidget_performer->setCurrentIndex(0);
        int anchoDisponible = 261 - 75;

        QFontMetrics metrics1(ui->caja_performer->font());
        QString titulo = QString::fromStdString(performer_selec.nombre);
        QString titulo_cortado = metrics1.elidedText(titulo, Qt::ElideRight, anchoDisponible);

        ui->caja_performer->setText(
            "<span style='color: #b0b0b0; font-size: 11pt; font-weight: normal;'>Nombre: </span>"
            "<span style='color: #ffffff; font-size: 12pt; font-weight: bold;'>" + titulo_cortado + "</span>"
            );

        return;
    }else{
        if(tipo_performer == 0){

            artista_selec = controlador->getPerson(performer_selec.nombre);

            qDebug() << "Artista encontrado ID:" << artista_selec.id << "Nombre art:" << artista_selec.stage_name.c_str();

            ui->stackedWidget_performer->setCurrentIndex(1);
            int anchoDisponible = 261 - 75;

            QFontMetrics metrics1(ui->caja_nombreArt_person->font());
            QString nombreArt = QString::fromStdString(artista_selec.stage_name);
            QString nombreArt_cortado = metrics1.elidedText(nombreArt, Qt::ElideRight, anchoDisponible);

            ui->caja_nombreArt_person->setText(
                "<span style='color: #b0b0b0; font-size: 11pt; font-weight: normal;'>Nombre art: </span>"
                "<span style='color: #ffffff; font-size: 12pt; font-weight: bold;'>" + nombreArt_cortado + "</span>"
                );

            QFontMetrics metrics2(ui->caja_nombreReal_person->font());
            QString nombreReal = QString::fromStdString(artista_selec.real_name);
            QString nombreReal_cortado = metrics2.elidedText(nombreReal, Qt::ElideRight, anchoDisponible);

            ui->caja_nombreReal_person->setText(
                "<span style='color: #b0b0b0; font-size: 10pt; font-weight: normal;'>Nombre real: </span>"
                "<span style='color: #ffffff; font-size: 11pt; font-weight: bold;'>" + nombreReal_cortado + "</span>"
                );

            QFontMetrics metrics3(ui->caja_fechaCumple_person->font());
            QString fechaCumple = QString::fromStdString(artista_selec.birth_date);
            QString fechaCumple_cortado = metrics3.elidedText(fechaCumple, Qt::ElideRight, anchoDisponible);

            ui->caja_fechaCumple_person->setText(
                "<span style='color: #b0b0b0; font-size: 10pt; font-weight: normal;'>Fecha cumpleaños: </span>"
                "<span style='color: #ffffff; font-size: 11pt; font-weight: bold;'>" + fechaCumple_cortado + "</span>"
                );

            QFontMetrics metrics4(ui->caja_fechaMuerte_person->font());
            QString fechaMuerte = QString::fromStdString(artista_selec.death_date);
            QString fechaMuerte_cortado = metrics4.elidedText(fechaMuerte, Qt::ElideRight, anchoDisponible);

            ui->caja_fechaMuerte_person->setText(
                "<span style='color: #b0b0b0; font-size: 10pt; font-weight: normal;'>Fecha muerte: </span>"
                "<span style='color: #ffffff; font-size: 11pt; font-weight: bold;'>" + fechaMuerte_cortado + "</span>"
                );

            return;

        }else{
            if(tipo_performer == 1){
                grupo_selec = controlador->getGroup(performer_selec.nombre);

                if(!grupo_selec.grupo_valido)
                    QMessageBox::critical(this, "Error", "No obtuvimos un grupo valido.");

                ui->stackedWidget_performer->setCurrentIndex(2);
                int anchoDisponible = 261 - 75;

                QFontMetrics metrics1(ui->caja_nombre_grupo->font());
                QString nombre = QString::fromStdString(grupo_selec.name);
                QString nombre_cortado = metrics1.elidedText(nombre, Qt::ElideRight, anchoDisponible);

                ui->caja_nombre_grupo->setText(
                    "<span style='color: #b0b0b0; font-size: 11pt; font-weight: normal;'>Nombre: </span>"
                    "<span style='color: #ffffff; font-size: 12pt; font-weight: bold;'>" + nombre_cortado + "</span>"
                    );

                QFontMetrics metrics2(ui->caja_fechaCrea_grupo->font());
                QString fechaCrea = QString::fromStdString(grupo_selec.start_date);
                QString fechaCrea_cortado = metrics2.elidedText(fechaCrea, Qt::ElideRight, anchoDisponible);

                ui->caja_fechaCrea_grupo->setText(
                    "<span style='color: #b0b0b0; font-size: 10pt; font-weight: normal;'>Fecha creación: </span>"
                    "<span style='color: #ffffff; font-size: 11pt; font-weight: bold;'>" + fechaCrea_cortado + "</span>"
                    );

                QFontMetrics metrics3(ui->caja_fechaDis_grupo->font());
                QString fechaDis = QString::fromStdString(grupo_selec.end_date);
                QString fechaDis_cortado = metrics3.elidedText(fechaDis, Qt::ElideRight, anchoDisponible);

                ui->caja_fechaDis_grupo->setText(
                    "<span style='color: #b0b0b0; font-size: 10pt; font-weight: normal;'>Fecha disolución: </span>"
                    "<span style='color: #ffffff; font-size: 11pt; font-weight: bold;'>" + fechaDis_cortado + "</span>"
                    );
            }
        }
    }

}

void MainWindow::on_editButton_performer_clicked()
{
    cambiarPerformer ventana;
    ventana.setModal(true);
    if(ventana.exec() == QDialog::Accepted && performer_selec.performer_valido){
        int tipo = ventana.getTipo() == "Artista"? 0 : 1;
        if(!controlador->definirTipoPerformer(performer_selec.id,tipo))
            QMessageBox::critical(this, "Error", "Algo salio mal al comunicarse con la base de datos.");

        if(tipo == 0){
            controlador->agregarPerson(performer_selec.nombre);
        }else{
            controlador->agregarGrupo(performer_selec.nombre);
        }
        if(busqueda_valida)
            ejecutarBusqueda(busqueda);
        else
            refrescar();
    }
}


void MainWindow::on_editButton_person_clicked()
{
    cambiarPerson ventana(QString::fromStdString(artista_selec.stage_name),QString::fromStdString(artista_selec.real_name),
                          QString::fromStdString(artista_selec.birth_date),QString::fromStdString(artista_selec.death_date));
    ventana.setModal(true);
    if(ventana.exec() == QDialog::Accepted){
        if(!controlador->modificarColumna_person(artista_selec.id,"stage_name",ventana.getNombreArt().toStdString()))
            QMessageBox::critical(this, "Error", "Algo salio mal al comunicarse con la base de datos.");
        if(!controlador->modificarColumna_performer(performer_selec.id,"name",0,ventana.getNombreArt().toStdString()))
            QMessageBox::critical(this, "Error", "Algo salio mal al comunicarse con la base de datos.");
        if(!controlador->modificarColumna_person(artista_selec.id,"real_name",ventana.getNombreReal().toStdString()))
            QMessageBox::critical(this, "Error", "Algo salio mal al comunicarse con la base de datos.");
        if(!controlador->modificarColumna_person(artista_selec.id,"birth_date",ventana.getFechaCumple().toStdString()))
            QMessageBox::critical(this, "Error", "Algo salio mal al comunicarse con la base de datos.");
        if(ventana.existFechaMuerte())
            if(!controlador->modificarColumna_person(artista_selec.id,"death_date",ventana.getFechaMuerte().toStdString()))
                QMessageBox::critical(this, "Error", "Algo salio mal al comunicarse con la base de datos.");
        performer_selec = controlador->getPerformer(performer_selec.id);
        performerClickeado(performer_selec);
        if(busqueda_valida)
            ejecutarBusqueda(busqueda);
        else
            refrescar();
    }
}



void MainWindow::on_editButton_grupo_clicked()
{
    CambiarGroup ventana(QString::fromStdString(grupo_selec.name),QString::fromStdString(grupo_selec.start_date),
                         QString::fromStdString(grupo_selec.end_date));
    ventana.setModal(true);
    if(ventana.exec() == QDialog::Accepted){
        if(!controlador->modificarColumna_goup(grupo_selec.id,"name",ventana.getNombre().toStdString()))
            QMessageBox::critical(this, "Error", "Algo salio mal al comunicarse con la base de datos.");
        if(!controlador->modificarColumna_performer(performer_selec.id,"name",0,ventana.getNombre().toStdString()))
            QMessageBox::critical(this, "Error", "Algo salio mal al comunicarse con la base de datos.");
        if(!controlador->modificarColumna_goup(grupo_selec.id,"start_date",ventana.getFechaCreacion().toStdString()))
            QMessageBox::critical(this, "Error", "Algo salio mal al comunicarse con la base de datos.");
        if(ventana.existFechaDisolucion())
            if(!controlador->modificarColumna_goup(grupo_selec.id,"end_date",ventana.getFechaDisolucion().toStdString()))
                QMessageBox::critical(this, "Error", "Algo salio mal al comunicarse con la base de datos.");
        performer_selec = controlador->getPerformer(performer_selec.id);
        performerClickeado(performer_selec);
        if(busqueda_valida)
            ejecutarBusqueda(busqueda);
        else
            refrescar();
    }
}




void MainWindow::on_editButton_integrantes_grupo_clicked()
{
    grupo_selec = controlador->getGroup(performer_selec.nombre);
    std::vector<Person> artistas = controlador->getPersons();
    std::vector<Performer> integrantes;

    for(const Person &artist : controlador->getPersonasGrupo(grupo_selec.id))
        for(const Performer &performer : controlador->getPerformers())
            if(artist.stage_name == performer.nombre && performer.id_type == 0)
                integrantes.push_back(performer);

    GrupoIntegrantes ventana(artistas,integrantes);
    ventana.setModal(true);
    if(ventana.exec() == QDialog::Accepted){
        if(!ventana.getAccion()){
            if(ventana.getNuevoArtista())
                return;
            else
                if(!controlador->eliminarPersonDeGrupo(ventana.getIdArtista(),grupo_selec.id))
                    QMessageBox::critical(this, "Error", "Algo salio mal al comunicarse con la base de datos.");
        }else{
            if(ventana.getNuevoArtista())
                if(busqueda_valida)
                    ejecutarBusqueda(busqueda);
                else
                    refrescar();
            else
                if(!controlador->agregarPersonAGrupo(ventana.getIdArtista(), grupo_selec.id))
                QMessageBox::critical(this,"Error","Algo salio mal al comunicarse con la base de datos.");
        }

        if(busqueda_valida){
            ejecutarBusqueda(busqueda);
        }else{
            refrescar();
        }
    }
}

void MainWindow::nuevoArtista(const QString &nombre){
    int id_perf = controlador->agregarPerformer(nombre.toStdString(),0);;
    controlador->modificarColumna_performer(id_perf,"id_type",0,"");
    controlador->agregarPerson(nombre.toStdString());
    if(busqueda_valida)
        ejecutarBusqueda(busqueda);
    else
        refrescar();
}

void MainWindow::nuevoGrupo(const QString &nombre){
    int id_perf = controlador->agregarPerformer(nombre.toStdString(),1);
    controlador->modificarColumna_performer(id_perf,"id_type",1,"");
    controlador->agregarGrupo(nombre.toStdString());
    if(busqueda_valida)
        ejecutarBusqueda(busqueda);
    else
        refrescar();
}

void MainWindow::on_actionAgregar_Artista_triggered()
{
    CambiarTitulo ventana("Nuevo artista","");
    ventana.setModal(true);
    if(ventana.exec() == QDialog::Accepted){
        if(!ventana.getResultado().isEmpty())
            nuevoArtista(ventana.getResultado());
    }
}


void MainWindow::on_actionAgregar_GRupo_triggered()
{
    CambiarTitulo ventana("Nuevo grupo","");
    ventana.setModal(true);
    if(ventana.exec() == QDialog::Accepted){
        if(!ventana.getResultado().isEmpty())
            nuevoGrupo(ventana.getResultado());
    }
}

void MainWindow::ejecutarBusqueda(const QString &texto) {
    busqueda = texto;
    controlador->busqueda(texto.toStdString());
}

void MainWindow::busquedaInvalida(){
    QMessageBox::critical(this,"Error de busqueda","Los argumentos de la busqueda no son correctos.");
}