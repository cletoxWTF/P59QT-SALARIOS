#include "controlador.h"

Controlador::Controlador(QObject *parent) : QObject(parent)
{
    this->m_obrero = nullptr;
}

bool Controlador::agregarObrero(QString nombre, int horas, TipoJornada jornada)
{
    this->m_obrero = new Obrero(nombre,horas,jornada);
    return true;
}

bool Controlador::calcularSalario()
{
    // VALIDAR QUE EL OBRERO NO SEA NULO
    if(m_obrero == nullptr)
        return false;

    // DETERMINAR EL VALOR HORA
    double valorHora = 0;
    switch(m_obrero->jornada()){
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
    int horas = m_obrero->horas();
    int horasExtra =0;
    if(m_obrero->horas()>40){
        horasExtra = m_obrero->horas() -40;
        horas = 40;
    }

    // CALCULO DEL SALARIO
    double salarioBruto = horas * valorHora + horasExtra * (HORA_EXTRA * valorHora/100);

    //CALCULO DEL DESCUENTO
    double descuento = salarioBruto * IESS/100;

    // CALCULO DEL SALARIO NETO
    double salarioNeto = salarioBruto - descuento;

    // ESTABLECER VALORES AL OBJETO
    m_obrero->setSalarioBruto(salarioBruto);
    m_obrero->setDescuento(descuento);
    m_obrero->setSalarioNeto(salarioNeto);

    // RETORNAR TRUE (TODO SE CALCULO CORRECTAMENTE)
    return true;
}

Obrero *Controlador::obrero() const
{
    return m_obrero;
}

void Controlador::setTBruto(float newTBruto)
{
    m_tBruto = newTBruto;
}

void Controlador::setTIEES(float newTIEES)
{
    m_tIEES = newTIEES;
}

void Controlador::setTNeto(float newTNeto)
{
    m_tNeto = newTNeto;
}

float Controlador::tBruto() const
{
    return m_tBruto;
}

float Controlador::tIEES() const
{
    return m_tIEES;
}

float Controlador::tNeto() const
{
    return m_tNeto;
}
