#include <iostream>
#include <cmath>
#include <fstream>

#include "poly.h"

using namespace std;

///construtores

Poly::Poly(): grau(-1), a(nullptr) {}  //default

Poly::Poly(const Poly& P): grau(P.grau), a(nullptr) {  //copia
    if (grau >= 0) a = new double [grau + 1];
    for (int k = 0; k <= grau; k++) a[k] = P.a[k];
}

Poly::Poly(Poly&& P): grau(P.grau), a(P.a){ //movimento
    P.grau = -1;
    P.a = nullptr;
}

Poly::Poly(int num): grau(num), a(nullptr){ //específico
    if (grau < 0) grau = -1;
    else if (grau == 0) a = new double[1], a[0] = 0.0;
    else {
        a = new double [grau + 1];
        for (int i=0 ; i <= grau; i++) a[i] = 0.0, a[grau] = 1.0;
    }
}

Poly::~Poly(){
  delete[] a;
}

///sobrecarga de operadores

Poly& Poly::operator=(const Poly& P){ //por copia
    if (this!=&P){
        delete[] a;
        grau = P.grau;
        if (grau >= 0) a = new double[grau+1];
        else {a = nullptr;}
        for (int i=0; i < grau + 1; i++){a[i] = P.a[i];}
    }
    return *this;
}

Poly& Poly::operator=(Poly&& P){ //por movimento
    delete[] a;

    grau = P.grau;
    a = P.a;
    P.grau = -1;
    P.a = nullptr;

    return *this;
}

double Poly::operator[](int i) const{ //operador []
    return getCoef(i);
}

bool Poly::operator==(const Poly& P) const{ //operador ==
    if (grau!= P.grau) return false;
    for (int j=0; j<=grau; j++){
        if (a[j] != P.a[j]) return false;
    }
    return true;
}

bool Poly::operator!=(const Poly& P) const{ //operador !=
    return !(*this == P);
}

double Poly::operator()(double x) const{//operador()
    return getValor(x);
}

///métodos de consulta e interacao

int Poly::getGrau() const{
    return grau;
}

double Poly::getCoef(int i) const{
    if (i < 0 || i > grau) return 0.0;
    else return a[i];
}

void Poly::setCoef(int i, double new_num){
    if (i < 0 || i > grau) {cerr << "Erro: valor invalido (indice)" << endl; return;}
    if(new_num == 0.0 && i == grau && grau > 0){
        cerr << "Erro: valor invalido (valor)" << endl;
        return;
    }
    a[i] = new_num;
}

void Poly::recriar(int new_grau){//recriar
    *this = Poly(new_grau);
}

bool Poly::empty() const{return grau < 0? true: false;} //método empty
bool Poly::isZero() const{return (grau == 0 && a[0] == 0.0)? true : false;} //método isZero

double Poly::getValor(double x) const{ //método getValor
    if(empty()) return 0.0; //checando se eh vazio de inicio
    double R = 0.0;
    for (int i = grau; i>= 0; i--) R = R*x+a[i]; // fiz pelo meio "inverso"
    return R;
}

ostream& operator << (ostream& Z, const Poly& P){ //operador <<
    bool termo = true;
    for (int i = P.getGrau(); i >= 0; i--) {
        double valor = P.getCoef(i);
        if (valor != 0.0) {
            if (!termo) {
                Z << (valor > 0 ? "+" : "-");
                valor = abs(valor);
            } else {
                termo = false;
                if (valor == -1) {
                    if (i > 0) Z << "-";
                    valor = 1;
                }
            }
            if (i > 0) {
                if (valor != 1.0) Z << valor << "*";
                Z << "x";
            } else Z << valor;
            if (i > 1) Z << "^" << i;
        }
    }
    if (!P.empty() && termo) Z << "0";
    return Z;
}

istream& operator >> (istream& W, Poly& P){ //operador >>
    if (P.empty()){
        cerr << "Erro: o polinomio estah vazio" << endl;
        return W;
    }
    for (int j = P.getGrau(); j >= 0; --j) {
        cout << "x^" << j << ": ";
        double coef;
        while (true) {
            W >> coef;
            if (j == P.getGrau() && coef == 0.0 && P.getGrau() != 0) {
                cerr << "Erro: O coeficiente de maior grau nao pode ser nulo(com excessao em polinomios de grau 0)" << endl;
                cout << "x^" << j << ": ";
            } else {
                P.setCoef(j, coef);
                break;
            }
        }
    }
    return W;
}

bool Poly::salvar(const string& name_arq) const{ // método salvar
    ofstream ark(name_arq);
    if (!ark.is_open()){
        return false;
    }
    ark << "POLY " << getGrau() << endl;
    for (int k = 0; k<= getGrau(); k++){
        ark << getCoef(k) << " ";
    }
    ark << endl;
    return true;
}

bool Poly::ler(const string& name_arq){ // método ler
    ifstream ark(name_arq);
    if (!ark.is_open()) return false;
    string titulo;
    ark >> titulo;
    if (!(ark.good()) || titulo != "POLY"){
        ark.close();
        return false;
    }
    int grau;
    if (!(ark >> grau) || !(ark.good())){
        ark.close();
        return false;
    }
    Poly prov(grau);
    if(grau >=0 || !(ark.good())){
        for (int i = 0; i<= grau; i++){
            double coeficiente;
            if(ark >> coeficiente || ark.good()){
                prov.a[i] = coeficiente;
            }
            else{
                ark.close();
                return false;
            }
        }
    }
    if (!(ark.good()) || (grau > 0 && prov.getCoef(grau) == 0.0)){
        ark.close();
        return false;
    }
    *this = prov;
    ark.close();
    return true;
}

Poly Poly::operator-() const { //unário
    if (!empty() && !isZero()) {
        Poly neg(getGrau());
        for (int k = 0; k <= getGrau(); k++) {
            neg.setCoef(k, -getCoef(k));
    }
        return neg;
}
    return *this;
}

Poly Poly::operator+(const Poly& P) const{ //binário +
    if (empty()) return P;
    else if (P.empty()) return *this;
    else if (isZero()) return P;
    else if (P.isZero()) return *this;

    int new_grau = max(getGrau(), P.getGrau());
    while (new_grau > 0 && (getCoef(new_grau) + P.getCoef(new_grau) == 0.0)) new_grau--;

    Poly R(new_grau);

    for(int i = 0; i <= new_grau; i++) R.setCoef(i, getCoef(i) + P.getCoef(i));
    return R;

}

Poly Poly::operator-(const Poly& P) const{ //binário -
    return *this + (-P);
}

Poly Poly::operator*(const Poly& P) const{
    if(empty() || P.empty()) return Poly();
    if(isZero() || P.isZero()) return Poly(0);
    int new_grau = getGrau() + P.getGrau();
    Poly R(new_grau);

    for(int i = 0; i <= new_grau; i++){
        double coeficiente = 0.0;
        for (int j = 0; j<= i; j++){
            if(j<= getGrau() && i - j <=P.getGrau()) coeficiente += a[j] * P.a[i - j];
        }
        R.setCoef(i, coeficiente);
    }
    return R;
}
