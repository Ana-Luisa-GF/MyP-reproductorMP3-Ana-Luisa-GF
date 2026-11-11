#ifndef CAMBIARALBUM_H
#define CAMBIARALBUM_H

#include <QDialog>

namespace Ui {
class CambiarAlbum;
}

class CambiarAlbum : public QDialog
{
    Q_OBJECT

public:
    explicit CambiarAlbum(const int &anio,const QString &titulo_album,QWidget *parent = nullptr);
    ~CambiarAlbum();
    QString getNuevoAlbum() const;
    int getNuenoAnio() const;
    bool getConversionAnio();

private slots:
    void on_realizado_clicked();

    void on_realizado_2_clicked();

private:
    Ui::CambiarAlbum *ui;
    QString titulo_nuevo;
    int anio_nuevo;
    bool conversion_anio=false;
};

#endif // CAMBIARALBUM_H
