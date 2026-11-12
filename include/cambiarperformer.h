#ifndef CAMBIARPERFORMER_H
#define CAMBIARPERFORMER_H

#include <QDialog>

namespace Ui {
class cambiarPerformer;
}

class cambiarPerformer : public QDialog
{
    Q_OBJECT

public:
    explicit cambiarPerformer(QWidget *parent = nullptr);
    ~cambiarPerformer();
    QString getTipo(){return tipo;}

private slots:
    void on_realizado_clicked();

    void on_realizado_2_clicked();

private:
    Ui::cambiarPerformer *ui;
    QString tipo = "";
};

#endif // CAMBIARPERFORMER_H
