#ifndef CAMBIARALBUM_H
#define CAMBIARALBUM_H

#include <QDialog>

namespace Ui {
class CambiarAlbum;
}

/**
 * @class CambiarAlbum
 * @brief Ventana de diálogo emergente para modificar o visualizar los datos de un álbum (título y año). 
 */
class CambiarAlbum : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor del diálogo.
     * @param anio Año actual del álbum a mostrar en la interfaz.
     * @param titulo_album Título inicial del álbum a mostrar en la interfaz.
     * @param parent Widget padre del diálogo.
     */
    explicit CambiarAlbum(const int &anio,const QString &titulo_album,QWidget *parent = nullptr);

    /**
     * @brief Destructor del diálogo.
     */
    ~CambiarAlbum();

    /**
     * @brief Obtiene el nuevo título introducido para el álbum.
     * @return QString del nuevo título.
     */
    QString getNuevoAlbum() const;

    /**
     * @brief Obtiene el nuevo año introducido para el álbum.
     * @return int del nuevo año.
     */
    int getNuenoAnio() const;

    /**
     * @brief Consulta el estado de la bandera de conversión del año.
     * @return true si el año fue convertido con éxito, false en otro caso.
     */
    bool getConversionAnio();

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
    Ui::CambiarAlbum *ui; ///< Puntero a los elementos visuales del archivo .ui.
    QString titulo_nuevo; ///< Almacena el título nuevo ingresado por el usuario.
    int anio_nuevo; ///< Almacena el nuevo año ingresado por el usuario.
    bool conversion_anio=false; ///< Bandera que indica si la conversion numericad el año fue exitosa.
};

#endif // CAMBIARALBUM_H
