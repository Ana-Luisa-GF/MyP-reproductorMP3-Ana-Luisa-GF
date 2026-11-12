#include "TarjetaCancion.h"
#include <QFile>
#include <QDebug>

TarjetaCancion::TarjetaCancion(const int &ancho, const int &alto, QWidget *parent) : QFrame(parent) {

    QVBoxLayout *layout = new QVBoxLayout(this);

    labelPortada = new QLabel(this);
    labelPortada->setFixedSize(ancho, alto);
    labelPortada->setScaledContents(true);

    labelTitulo = new QLabel(this);
    labelTitulo->setAlignment(Qt::AlignCenter);
    labelTitulo->setWordWrap(true);

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

void TarjetaCancion::establecerDatos(const QString &imagen_base,const std::string& titulo, const std::filesystem::path& portada){
    this->tituloCancion =QString::fromStdString(titulo);
    labelTitulo->setText(tituloCancion);
    QString rutaQt = QString::fromStdString(portada.string());

    if (!rutaQt.isEmpty() && QFile::exists(rutaQt)) {
        QPixmap pixmap(rutaQt);
        labelPortada->setPixmap(pixmap.scaled(labelPortada->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    } else {
        QPixmap pixmapDefault(imagen_base);
        labelPortada->setPixmap(pixmapDefault.scaled(labelPortada->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    }

}

void TarjetaCancion::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit click(this->tituloCancion.toStdString());
    }
    QFrame::mousePressEvent(event);
}