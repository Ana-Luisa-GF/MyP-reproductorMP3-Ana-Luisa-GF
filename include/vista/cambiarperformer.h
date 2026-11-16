#ifndef CAMBIARPERFORMER_H
#define CAMBIARPERFORMER_H

#include <QDialog>

namespace Ui {
class cambiarPerformer;
}

/**
 * @class cambiarPerformer
 * @brief Ventana de diálogo para seleccionar el tipo de intérprete.
 */
class cambiarPerformer : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief Constructor del diálogo.
     * @param parent Widget padre del diálogo.
     */
    explicit cambiarPerformer(QWidget *parent = nullptr);

    /**
     * @brief Destructor del diálogo.
     */
    ~cambiarPerformer();

    /**
     * @brief Obtiene el tipo de intérprete seleccionado.
     * @return QString con el tipo (persona o grupo).
     */
    QString getTipo(){return tipo;}

private slots:

    /**
     * @brief Slot ejecutado al hacer clic en el botón "Aceptar".
     */
    void on_realizado_clicked();

    /**
     * @brief Slot ejecutado al hacer clic en el botón "Cancelar".
     */
    void on_realizado_2_clicked();

private:
    Ui::cambiarPerformer *ui; ///< Puntero a los elementos visuales del archivo .ui.
    QString tipo = ""; ///< Cadena que almacena la selección del usuario (persona o grupo).
};

#endif // CAMBIARPERFORMER_H
