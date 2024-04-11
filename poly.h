#ifndef _POLY_H_
#define _POLY_H_

class Poly{

private:
    int grau; //grau polinomio
    double* a; // coeficiente
public:
    ///construtores

    Poly(); //default
    Poly(const Poly& P); //cópia
    Poly(Poly&& P); //movimento
    explicit Poly (int num); //específico

    ///destrutor

    ~Poly();

    ///sobrecarga de operadores

    Poly& operator = (const Poly& P); //por cópia
    Poly& operator = (Poly&& P); //por movimento

    double operator[] (int j) const; //[]
    bool operator == (const Poly& P) const; //==
    bool operator != (const Poly& P) const; // !=
    double operator()(double x) const; // ()
    Poly operator-() const; //unário
    Poly operator+(const Poly& P) const; //binários
    Poly operator-(const Poly& P) const;
    Poly operator*(const Poly& P) const; // multiplicação

    ///métodos de consulta e alteração
    int getGrau() const;
    double getCoef(int i) const;
    void setCoef(int i, double new_num);
    bool isZero() const;
    double getValor(double x) const;

    ///métodos gerais
    bool empty() const;
    void recriar(int grau_new);
    bool ler (const std::string& name_arq);
    bool salvar(const std::string& name_arq) const;

    /// >> <<
    friend std::ostream& operator<<(std::ostream& Z, const Poly& P);
    friend std::istream& operator>>(std::istream& W, Poly& P);

};

#endif // _POLY_H_
