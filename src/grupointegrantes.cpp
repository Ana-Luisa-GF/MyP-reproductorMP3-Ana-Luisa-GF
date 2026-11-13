#include "grupointegrantes.h"
#include "ui_grupointegrantes.h"
#include "TarjetaCancion.h"
#include <QCompleter>
#include <QMessageBox>

GrupoIntegrantes::GrupoIntegrantes(std::vector<Person> &artistas, std::vector<Performer> &integrantes, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GrupoIntegrantes)
{
    ui->setupUi(this);
    this->artistas = artistas;
    this->integrantes = integrantes;
    cargarListaIntegrantes();


    QStringList listaQt;
    for (const Person &g : artistas) {
        listaQt.append(QString::fromStdString(g.stage_name));
        ui->comboBox->addItem(QString::fromStdString(g.stage_name), QVariant(g.id));
    }

    ui->comboBox->setEditable(true);
    ui->comboBox->setInsertPolicy(QComboBox::InsertPolicy::InsertAtBottom);

    QCompleter *completer = ui->comboBox->completer();
    if (completer) {
        completer->setCompletionMode(QCompleter::PopupCompletion);

        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setFilterMode(Qt::MatchContains);
    }
}

GrupoIntegrantes::~GrupoIntegrantes()
{
    delete ui;
}

void GrupoIntegrantes::cargarListaIntegrantes(){

    ui->listWidget->clear();
    ui->listWidget->setViewMode(QListWidget::IconMode);
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    ui->listWidget->setGridSize(QSize(223, 223));
    ui->listWidget->setWordWrap(true);

    for (const Performer& perf : integrantes) {
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);

        TarjetaCancion* tarjeta = new TarjetaCancion(221,221, ui->listWidget);

        tarjeta->establecerDatos(":/iconos/artista_base.png", perf.nombre, perf.ruta_imagen);

        item->setSizeHint(tarjeta->sizeHint());
        ui->listWidget->setItemWidget(item, tarjeta);
    }
}


void GrupoIntegrantes::on_realizado_clicked()
{

    if(ui->radioButton_agregar->isChecked()){
        accion = true;
    }else
        if(ui->radioButton_eliminar->isChecked()){
        accion = false;
    }else{
        QMessageBox::critical(this, "Error", "No se selecciono ninguna opción.");
        reject();
        return;
    }

    textoArtista = ui->comboBox->currentText().trimmed();

    if (textoArtista.isEmpty()) {
        QMessageBox::critical(this, "Error", "El campo del artista no puede estar vacío.");
        reject();
        return;
    }

    int indiceEncontrado = ui->comboBox->findText(textoArtista);

    if (indiceEncontrado != -1){
        esNuevoArtista = false;
        QVariant datoOculto = ui->comboBox->currentData();
        id_artista = datoOculto.toInt();
    }
    else{
        esNuevoArtista = true;
    }

    accept();
}


void GrupoIntegrantes::on_rechazado_clicked()
{
    reject();
}
