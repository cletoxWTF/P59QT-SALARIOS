#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include <QObject>
#include "obrero.h"


#define MATUTINO 2.65
#define VESPERTINO 2.73
#define NOCTURNO 2.87

#define HORA_EXTRA 0.5
#define IESS 9.5

class Controlador : public QObject
{
    Q_OBJECT
public:
    Obrero *m_obrero;
    explicit Controlador(QObject *parent = nullptr); 
    bool agregarObrero(QString nombre, int horas,TipoJornada jornada);
    bool calcularSalario();

    Obrero *obrero() const;

    void setTBruto(float newTBruto);
    void setTIEES(float newTIEES);
    void setTNeto(float newTNeto);

    float tBruto() const;
    float tIEES() const;
    float tNeto() const;

signals:

private:
    float m_tBruto = 0;
    float m_tIEES = 0;
    float m_tNeto = 0;
    // Obrero *m_obrero;
};

#endif // CONTROLADOR_H
