#ifndef SALARIOS_H
#define SALARIOS_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

#include "controlador.h"
#include "acerca.h"
#include "obrero.h"

#define VERSION "1.0"
QT_BEGIN_NAMESPACE
namespace Ui { class Salarios; }
QT_END_NAMESPACE

class Salarios : public QMainWindow
{
    Q_OBJECT

public:
    Salarios(QWidget *parent = nullptr);
    ~Salarios();

private slots:
    void on_cmdCalcular_clicked();

    void on_actionCalcular_triggered();

    void on_actionGuaradar_triggered();

    void on_actionNuevo_triggered();

    void on_actionAbrir_triggered();

    void on_actionAcerca_de_Salarios_triggered();

private:
    Ui::Salarios *ui;
    Controlador *m_controlador;
    Obrero *m_datos;

    float m_tBruto = 0;
    float m_tIEES = 0;
    float m_tNeto = 0;
    void limpiar();
    void calcular();
    void guardar();
    void abrir();
    bool calculos();
};
#endif // SALARIOS_H
