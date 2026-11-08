#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QInputDialog>
#include "TarjetaCancion.h"

MainWindow::MainWindow(RepControlador *ctrl, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , controlador(ctrl)
{
    ui->setupUi(this);

    QLineEdit *barraBusqueda = new QLineEdit(this);
    barraBusqueda->setPlaceholderText("Buscar...");
    ui->mainToolBar->addWidget(barraBusqueda);
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
    cargarListaCanciones(controlador->getCanciones());
}


void MainWindow::cargarListaCanciones(std::vector<Cancion> rolas){

    ui->listWidget->setViewMode(QListWidget::IconMode);
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    ui->listWidget->setGridSize(QSize(180, 180));
    ui->listWidget->setWordWrap(true); //-----

    for (const Cancion& cancion : rolas) {
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);

        TarjetaCancion* tarjeta = new TarjetaCancion(ui->listWidget);
        tarjeta->establecerDatos(cancion.titulo, cancion.portada);

        item->setSizeHint(tarjeta->sizeHint());
        ui->listWidget->setItemWidget(item, tarjeta);
        connect(tarjeta, &TarjetaCancion::click, this, [this, cancion](const std::string &titulo) {
            this->cancionClickeada(cancion.portada);
        });
    }

  /*  QWidget* contenedor = ui->scrollArea->widget();
    QVBoxLayout* layoutScroll = qobject_cast<QVBoxLayout*>(contenedor->layout());
    if (!layoutScroll) {
        layoutScroll = new QVBoxLayout(contenedor);
        contenedor->setLayout(layoutScroll);
    }

    QLayoutItem* item;
    while ((item = layoutScroll->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    for (const Cancion& cancion : rolas) {
        TarjetaCancion* tarjeta = new TarjetaCancion(contenedor);

        tarjeta->establecerDatos(cancion.titulo, cancion.portada);


        connect(tarjeta, &TarjetaCancion::click, this, &MainWindow::cancionClickeada);

        layoutScroll->addWidget(tarjeta);
    }
    layoutScroll->addStretch();*/
}


void MainWindow::cancionClickeada(const std::filesystem::path& portada)
{
    qDebug() << "¡Se hizo clic en una canción!";
    QString rutaQt = QString::fromStdString(portada.string());

    if (!rutaQt.isEmpty() && QFile::exists(rutaQt)) {
        QPixmap pixmap(rutaQt);
        ui->pantalla->setPixmap(pixmap.scaled(ui->pantalla->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        QPixmap pixmapDefault(":/iconos/letras-de-canciones.png");
        ui->pantalla->setPixmap(pixmapDefault.scaled(ui->pantalla->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}
