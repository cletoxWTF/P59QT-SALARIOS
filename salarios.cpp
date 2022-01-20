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
        calculos();
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
                                                         "Guardar Archivo",
                                                         QDir::home().absolutePath(),
                                                         "Archivos de Texto(*.slr)");

    // CREAR UN OBJETO QFILE
    QFile archivo(nombreArchivo);

    //ABRIRLO PARA ESCRITURA
    if(archivo.open(QFile::WriteOnly|QFile::Truncate)){

        // CREAR UN 'STREAM' DE TEXTO
        QTextStream salida(&archivo);

        // ENVIAR LOS DATOS DEL RESULTADO A LA SALIDA
        salida <<ui->outResultado->toPlainText();

        // MOSTRAR 5 SEGUNDOS QUE TODO ESTA BIEN
        ui->statusbar->showMessage("Datos almacenados en " + nombreArchivo, 5000);
    }else{

        //MENSAJE DE ERROR SI NO SE PUEDE ABRIR EL ARCHIVO
        QMessageBox::warning(this,
                             "GUARDAR DATOS",
                             "No se pudo guardar el Archivo");
    }

    // CERRAR ARCHIVO
    archivo.close();
}

void Salarios::abrir()
{
    // ABRIR CUADRO DE DIALOGO PARA SELECCIONAR UBICACION Y NOMBRE DEL ARCHIVO
    QString nombreArchivo = QFileDialog::getOpenFileName(this,
                                                         "Abrir Archivo",
                                                         QDir::home().absolutePath(),
                                                         "Archivos de Texto(*.slr)");

    // CREAR UN OBJETO QFILE
    QFile archivo(nombreArchivo);

    //ABRIRLO PARA LECTURA
    if(archivo.open(QFile::ReadOnly)){

        // CREAR UN 'STREAM' DE TEXTO
        QTextStream entrada(&archivo);

        // LEER EL CONTENIDO DEL ARCHIVO
        QString datos = entrada.readAll();

        // CARGAR EL CONTENIDO AL ARE DE TEXTO
        ui->outResultado->clear();
        ui->outResultado->setPlainText(datos);

        // MOSTRAR 5 SEGUNDOS QUE TODO ESTA BIEN
        ui->statusbar->showMessage("Datos leidos desde " + nombreArchivo, 5000);
    }else{

        // MENSAJE DE ERROR SI NO SE PUEDE ABRIR EL ARCHIVO
        QMessageBox::warning(this,
                             "ABRIR DATOS",
                             "No se puede abrir el Archivo");
    }

    // CERRAR ARCHIVO
    archivo.close();
}

bool Salarios::calculos()
{
    // VALIDAR QUE EL OBRERO NO SEA NULO
    if(m_controlador->m_obrero == nullptr)
        return false;

    // DETERMINAR EL VALOR HORA
    double valorHora = 0;
    switch(m_controlador->m_obrero->jornada()){
    case TipoJornada::Vespertina:
        valorHora = VESPERTINO;
        break;
    case TipoJornada::Matutina:
        valorHora = MATUTINO;
        break;
    case TipoJornada::Nocturna:
        valorHora = NOCTURNO;
        break;
    default:
        return false;
    }

    // CALCULO DE LAS HORAS EXTRA
    int horas = m_controlador->m_obrero->horas();
    int horasExtra =0;
    if(m_controlador->m_obrero->horas()>40){
        horasExtra = m_controlador->m_obrero->horas() -40;
        horas = 40;
    }

    // CALCULO DEL SALARIO
    double salarioBruto = horas * valorHora + horasExtra * (HORA_EXTRA * valorHora/100);
    this->m_controlador->setTBruto(this->m_controlador->tBruto()+salarioBruto);
    ui->outSbruto->setText("$" + QString::number(this->m_controlador->tBruto(),'f',2));

    //CALCULO DEL DESCUENTO
    double descuento = salarioBruto * IESS/100;
    this->m_controlador->setTIEES(this->m_controlador->tIEES()+descuento);
    ui->outIESS->setText("$" + QString::number(this->m_controlador->tIEES(),'f',2));

    // CALCULO DEL SALARIO NETO
    double salarioNeto = salarioBruto - descuento;
    this->m_controlador->setTNeto(this->m_controlador->tNeto()+salarioNeto);
    ui->outSneto->setText("$" + QString::number(this->m_controlador->tNeto(),'f',2));

    // RETORNAR TRUE (TODO SE CALCULO CORRECTAMENTE)

    return true;

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


void Salarios::on_actionAbrir_triggered()
{
    abrir();
}


void Salarios::on_actionAcerca_de_Salarios_triggered()
{
    // CREAR UN OBJETO DE LA VENTANA QUE QUEREMOS INVOCAR
    Acerca *dialogo = new Acerca(this);

    // ENVIAR PARAMETRO A LA VENTANA
    dialogo->setVersion(VERSION);

    // MOSTRAR LA VENTANA (DIALOGO)
    dialogo->exec();
}


