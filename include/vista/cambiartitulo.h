#ifndef CAMBIARTITULO_H
#define CAMBIARTITULO_H

#include <QDialog>

namespace Ui {
class CambiarTitulo;
}

/**
 * @class CambiarTitulo
 * @brief Ventana de diálogo para modificar o visualizar el título de una canción.
 */
class CambiarTitulo : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief Constructor del diálogo.
     * @param titulo_ventana Título de la barra superior de la ventana.
     * @param titulo_cancion Título de la canción.
     * @param parent Widget padre del diálogo.
     */
    explicit CambiarTitulo(const QString &titulo_ventana, const QString &titulo_cancion, QWidget *parent = nullptr);
    
    /**
     * @brief Destructor del diálogo.
     */
    ~CambiarTitulo();

    /**
     * @brief Obtiene el nuevo título dado por el ususario.
     * @return QString con el título.
     */
    QString getResultado() const;


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
    Ui::CambiarTitulo *ui; ///< Puntero a los elementos visuales del archivo .ui.
    QString titulo_cancion=""; ///< Título original o previo de la canción.

    QString resultado; ///< Nuevo título dado por el usuario.
};

#endif // CAMBIARTITULO_H
