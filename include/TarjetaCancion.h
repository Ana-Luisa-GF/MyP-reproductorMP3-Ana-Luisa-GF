#ifndef TARJETACANCION_H
#define TARJETACANCION_H
#include <filesystem>
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QFile>

class TarjetaCancion: public QFrame {
    Q_OBJECT

public:
    explicit TarjetaCancion(QWidget *parent=nullptr);
    void establecerDatos(const std::string& titulo, const std::filesystem::path& portada);

signals:
    void click(const std::string &titulo);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QString tituloCancion;
    QLabel *labelPortada;
    QLabel *labelTitulo;
};

#endif
