#ifndef CAMBIARTITULO_H
#define CAMBIARTITULO_H

#include <QDialog>

namespace Ui {
class CambiarTitulo;
}

class CambiarTitulo : public QDialog
{
    Q_OBJECT

public:
    explicit CambiarTitulo(const QString &titulo_ventana, const QString &titulo_cancion, QWidget *parent = nullptr);
    ~CambiarTitulo();
    QString getResultado() const;


private slots:
    void on_realizado_clicked();

    void on_realizado_2_clicked();

private:
    Ui::CambiarTitulo *ui;
    QString titulo_cancion="";

    QString resultado;
    bool modificado = false;
};

#endif // CAMBIARTITULO_H
