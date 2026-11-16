#ifndef CAMBIARPERSON_H
#define CAMBIARPERSON_H

#include <QDialog>

namespace Ui {
class cambiarPerson;
}

/**
 * @class cambiarPerson
 * @brief Ventana de diálogo para modificar o visualizar los datos de una persona.
 */
class cambiarPerson : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief Constructor del diálogo.
     * @param nombreArt Nombre artístico de la persona.
     * @param nombreReal Nombre real de la persona.
     * @param fechaCumple Fecha de nacimiento.
     * @param fechaMuerte Fecha de defunción (si hay).
     * @param parent Widget padre ael diálogo.
     */
    explicit cambiarPerson(const QString &nombreArt, const QString &nombreReal,const QString &fechaCumple,
                           const QString &fechaMuerte,QWidget *parent = nullptr);
    
    /**
     * @brief Destructor del diálogo. 
     */
    ~cambiarPerson();

    /**
     * @brief Obtiene el nombre artístico ingresado o modificado.
     * @return QString con el nombre artístico.
     */
    QString getNombreArt(){return nombre_art;}

    /**
     * @brief Obtiene el nombre real ingresado o modificado.
     * @return QString con el nombre real.
     */
    QString getNombreReal(){return nombre_real;}

    /**
     * @brief Obtiene la fecha de nacimiento ingresada o modificada.
     * @return QString con la fecha de nacimiento.
     */
    QString getFechaCumple(){return fecha_cumple;}

    /**
     * @brief Obtiene la fecha de defunción ingresada o modificada.
     * @return QString con la fecha de defunción.
     */
    QString getFechaMuerte(){return fecha_muerte;}

    /**
     * @brief Verifica si la persona tiene una fecha de defunción.
     * @return true si existe fecha de defunción, false en otro caso.
     */
    bool existFechaMuerte(){return hayFechaMuerte;}

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
    Ui::cambiarPerson *ui; ///< Puntero a los elementos visuales del archivo .ui.
   QString nombre_art; ///< Almacena el nombre artístico actual o editado.
    QString nombre_real; ///< Almacena el nombre real actual o editado.
    QString fecha_cumple; ///< Almacena la fecha de nacimiento actual o editada.
    QString fecha_muerte; ///< Almacena la fecha de defunción actual o editada.
    bool hayFechaMuerte; ///< Booleano que indica si la persona tiene fecha de defunción.
};

#endif // CAMBIARPERSON_H
