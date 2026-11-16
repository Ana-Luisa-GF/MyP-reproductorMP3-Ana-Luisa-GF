#ifndef CAMBIARGROUP_H
#define CAMBIARGROUP_H

#include <QDialog>

namespace Ui {
class CambiarGroup;
}

/**
 * @class CambiarGroup
 * @brief Ventana de diálogo para modificar o visualizar los datos de un grupo musical.
 */
class CambiarGroup : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief Constructor del diálogo.
     * @param nombre Nombre actual del grupo.
     * @param fechaCrea Fecha inicial de creación del grupo.
     * @param fechaDis Fecha inicial de disolución del grupo (si hay).
     * @param parent Widget padre del diálogo.
     */
    explicit CambiarGroup(const QString &nombre, const QString &fechaCrea,const QString &fechaDis, QWidget *parent = nullptr);
    
    /**
     * @brief Destructor del diálogo.
     */
    ~CambiarGroup();

    /**
     * @brief Obtiene el nombre del grupo ingresado o modificado.
     * @return QString con el nombre del grupo.
     */
    QString getNombre(){return nombre;}

    /**
     * @brief Obtiene la fecha de creación del grupo.
     * @return QString con la fecha de creación.
     */
    QString getFechaCreacion(){return fecha_creacion;}

    /**
     * @brief Obtiene la fecha de disolución del grupo.
     * @return QString con la fecha de disolución.
     */
    QString getFechaDisolucion(){return fecha_disolucion;}

    /**
     * @brief Verifica si el grupo cuenta con una fecha de disolución activa.
     * @return true si existe fecha de disolución, false en caso contrario.
     */
    bool existFechaDisolucion(){return hayFechaDisolucion;}

private slots:

    /**
     * @brief Slot ejecutado al hacer clic en el botón "Aceptar".
     */
    void on_realizado_clicked();

    /**
     * @brief Slot ejecutado al hacer clic en el botón "Cancelar".
     */
    void on_rechazado_clicked();

private:
    Ui::CambiarGroup *ui; ///< Puntero a los elementos visuales del archivo .ui.
    QString nombre; ///< Almacena el nombre del grupo.
    QString fecha_creacion; ///< Almacena la fecha de creación del grupo.
    QString fecha_disolucion; ///< Almacena la fecha de disolución del grupo.
    bool hayFechaDisolucion; ///< Booleano que indica si el grupo tiene fecha de disolución.

};
#endif // CAMBIARGROUP_H
