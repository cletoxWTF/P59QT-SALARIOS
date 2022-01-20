#include "salarios.h"
#include "ui_salarios.h"

Salarios::Salarios(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Salarios)
{
    ui->setupUi(this);
    m_controlador = new Controlador();
}

Salarios::~Salarios()
{
    delete ui;
}


void Salarios::on_cmdCalcular_clicked()
{
    calcular();
}

void Salarios::limpiar()
{
    ui->inNombre->setText("");
    ui->inHoras->setValue(0);
    ui->inMatutino->setChecked(true);
    ui->inNombre->setFocus();
}

void Salarios::calcular()
{
    // OBTENER DATOS DE LA GUI
    QString nombre = ui->inNombre->text();
    int horas = ui->inHoras->value();
    TipoJornada jornada;
    if(ui->inMatutino->isChecked()){
        jornada = TipoJornada::Matutina;
    }else if(ui->inVespertina->isChecked()){
        jornada = TipoJornada::Vespertina;
    }else{
        jornada = TipoJornada::Nocturna;
    }

    // VALIDAR DATOS CORRECTOS
    if(nombre==""||horas==0){
        /*
        QMessageBox msgBox;
        msgBox.setText("El nombre o el numero de horas esta vacio");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        */
        QMessageBox::warning(this,"ADVERTENCIA","El nombre o el numero de horas esta vacio");
        return;
    }

    // AGREGAR OBRERO AL CONTROLADOR
    m_controlador->agregarObrero(nombre,horas,jornada);

    // CALCULAR
    if(m_controlador->calcularSalario()){
        // MUESTRA LOS RESULTADOS DE LOS CALCULOS DE LOS OBREROS
        ui->outResultado->appendPlainText(m_controlador->obrero()->toString());
        // LIMPIAR LA INTERFAZ
        limpiar();

        // MOSTRAR MENSAJE POR 5 SEGUNDOS EN LA BARRA DE ESTADOS
        ui->statusbar->showMessage("Calculos procesados para " + nombre,5000);
    }else{
        QMessageBox::critical(
                    this,
                    "ERROR",
                    "Error al calcular el salario.");
    }
}

void Salarios::guardar()
{
    // ABRIR CUADRO DE DIALOGO PARA SELECCIONAR UBICACION Y NOMBRE DEL ARCHIVO
    QString nombreArchivo = QFileDialog::getSaveFileName(this,
                                                         "Guardar Datos",
                                                         QDir::home().absolutePath(),
                                                         "Archivos de Texto(*.txt)");
    qDebug() << nombreArchivo;

    // CREAR UN OBJETO QFILE
    QFile archivo(nombreArchivo);

    //ABRIRLO PARA ESCRITURA
    if(archivo.open(QFile::WriteOnly|QFile::Truncate)){

        // CREAR UN 'STREAM' DE TEXTO
        QTextStream salida(&archivo);

        // ENVIAR LOS DATOS DEL RESULTADO A LA SALIDA
        salida <<ui->outResultado->toPlainText();

        // MOSTRAR 5 SEGUNDOS QUE TODO ESTA BIEN
        ui->statusbar->showMessage("Datos alamacenados en " + nombreArchivo, 5000);
    }else{

        //MENSAJE DE ERROR SI NO SE PUEDE ABRIR EL ARCHIVO
        QMessageBox::warning(this,
                             "GUARDAR DATOS",
                             "No se Pudo guardar los datos");
    }

    // CERRAR ARCHIVO
    archivo.close();
}


void Salarios::on_actionCalcular_triggered()
{
    calcular();
}


void Salarios::on_actionGuaradar_triggered()
{
    guardar();
}


void Salarios::on_actionNuevo_triggered()
{
    limpiar();
    ui->outResultado->clear();
}

