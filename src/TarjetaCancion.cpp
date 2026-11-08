#include "TarjetaCancion.h"
#include <QFile>

TarjetaCancion::TarjetaCancion(QWidget *parent) : QFrame(parent) {

    QVBoxLayout *layout = new QVBoxLayout(this);

    labelPortada = new QLabel(this);
    labelPortada->setFixedSize(178, 178);
    labelPortada->setScaledContents(true);

    labelTitulo = new QLabel(this);
    labelTitulo->setAlignment(Qt::AlignCenter);

    layout->addWidget(labelPortada);
    layout->addWidget(labelTitulo);
    this->setLayout(layout);

    this->setStyleSheet(
        "QFrame {"
        "   background-color: #2b2b2b;"
        "   border: 1px solid #3f3f3f;"
        "   border-radius: 8px;"
        "}"
        "QFrame:hover {"
        "   background-color: #3b3b3b;"
        "}"
        "QLabel { border: none; color: white; }"
        );
}

void TarjetaCancion::establecerDatos(const std::string& titulo, const std::filesystem::path& portada){
    this->tituloCancion =QString::fromStdString(titulo);
    labelTitulo->setText(tituloCancion);
    QString rutaQt = QString::fromStdString(portada.string());

    if (!rutaQt.isEmpty() && QFile::exists(rutaQt)) {
        QPixmap pixmap(rutaQt);
        labelPortada->setPixmap(pixmap.scaled(labelPortada->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        QPixmap pixmapDefault(":/iconos/letras-de-canciones.png");
        labelPortada->setPixmap(pixmapDefault.scaled(labelPortada->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    /*QString rutaQt = QString::fromStdString(portada.string());

    if(!rutaQt.isEmpty() && QFile::exists(rutaQt))
        labelPortada->setPixmap(QPixmap(rutaQt));
    else
        labelPortada->setPixmap(QPixmap(":/iconos/letras-de-canciones.png"));*/

}

void TarjetaCancion::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit click(this->tituloCancion.toStdString());
    }
    QFrame::mousePressEvent(event);
}