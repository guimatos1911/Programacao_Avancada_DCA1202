#include "coord.h"
#include <cmath>

/// Funcoes de E/S de coordenadas
std::istream& operator>>(std::istream& I, Coord& C)
{
  I >> C.lin >> C.col;
  return I;
}

std::ostream& operator<<(std::ostream& O, const Coord& C)
{
  O << C.lin << ';' << C.col;
  return O;
}

double Coord::hDiagonal(Coord& C) const{
    const double pi = 3.14159265359;

    double Dcol(col - C.col);
    double Dlin(lin - C.lin);

    double h_rotacao;
    if(Dcol == 0 || Dlin == 0 || Dlin == Dcol) {
        h_rotacao = 0;
    }
    else {
        h_rotacao = (pi)/8;
    }
    return sqrt(2.0)*std::min(fabs(Dlin),fabs(Dcol)) + fabs(fabs(Dlin) - fabs(Dcol)) + h_rotacao;
}
