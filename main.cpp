#include "salarios.h"
#include "obrero.h"
#include "controlador.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Salarios w;
    w.show();

    // Obrero *objObrero1 = new Obrero("Juan",47,TipoJornada::Vespertina);

    Controlador *obj = new Controlador();

    obj->agregarObrero("Juan",40,TipoJornada::Matutina);
    if(obj->calcularSalario()){
        qDebug() << obj->obrero()->toString();
    }else{
        qDebug() << "NO SE PUDO REALIZAR EL CALCULO" << endl;;
    }

    return a.exec();
}
