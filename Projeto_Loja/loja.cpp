#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cmath>
#include <limits>
#include "loja.h"

using namespace std;
//default
Produto::Produto(const string& N, double P): nome(N), preco(P) {}

//consulta
string Produto::getNome() const{return nome;}
double Produto::getPreco() const{return preco;}

//recepcao de dados
void Produto::digitar(){
  do
  {
    cout << "Nome: ";
    getline(cin,nome);
  }
  while (nome=="");
  do
  {
    cout << "Preco (##.##): ";
    cin >> preco;
  }
  while (preco <= 0.0);
  cin.ignore();
}

//define a funcao imprimir como sendo salvar em cout
void Produto::imprimir() const{salvar(cout);}

bool Produto::ler(istream& I){
  try{
    I.ignore(numeric_limits<streamsize>::max(),'"');
    getline(I,nome,'"');
    if (!I.good() || nome=="") throw 1;
    I.ignore(numeric_limits<streamsize>::max(),'$');
    I >> preco;
    if (!I.good() || preco<=0.0) throw 2;
  }
  catch (int i){
    nome = "";
    preco = 0;
    return false;
  }
  return true;
}

void Produto::salvar(ostream& O) const{O << '"' << nome << '"' << ";$" << fixed << setprecision(2) << preco;}

istream& operator>>(istream& I, Produto& P){
  P.digitar();
  return I;
}

ostream& operator<<(ostream& O, const Produto& P){
  P.salvar(O);
  return O;
}

/// CLASSE LIVRO

//default
Livro::Livro(const string& N, double P, const string& A): Produto(N,P), autor(A) {}

//consulta
string Livro::getAutor() const{return autor;}

//recepcao de dados
void Livro::digitar()
{
  Produto::digitar();
  do
  {
    cout << "Autor: ";
    getline(cin,autor);
  }
  while (autor=="");
}

void Livro::imprimir() const{salvar(cout);}
bool Livro::ler(istream& I){

  try{
    char opcao;
    I >> opcao;
    if (!I.good() || opcao!='L') throw 1;
    I.ignore(numeric_limits<streamsize>::max(),':');
    if (!Produto::ler(I)) throw 2;
    I.ignore(numeric_limits<streamsize>::max(),'"');
    getline(I,autor,'"');
    if (!I.good() || autor == "") throw 3;
    I.ignore();
  }
  catch (int i){
    *this = Livro();
    return false;
  }
  return true;
}

void Livro::salvar(ostream& O) const{
  O << "L: ";
  Produto::salvar(O);
  O << ';' << '"' << autor << '"';
}

istream& operator>>(istream& I, Livro& L){
  L.digitar();
  return I;
}

ostream& operator<<(ostream& O, const Livro& L){
  L.salvar(O);
  return O;
}

/// CLASSE CD

//default
CD::CD(const string& N, double P, int NF): Produto(N,P), nfaixas(NF) {}

//consulta
int CD::getNumFaixas() const{return nfaixas;}

//recepcao de dados
void CD::digitar(){
  Produto::digitar();
  do
  {
    cout << "Numero de faixas: ";
    cin >> nfaixas;
  }
  while (nfaixas <= 0);
  cin.ignore();
}

void CD::imprimir() const{salvar(cout);}
bool CD::ler(istream& I)
{
  try{
    char opcao;
    I >> opcao;
    if (!I.good() || opcao!='C') throw 1;
    I.ignore(numeric_limits<streamsize>::max(),':');
    if (!Produto::ler(I)) throw 2;
    I.ignore(numeric_limits<streamsize>::max(),';');
    I >> nfaixas;
    if (!I.good() || nfaixas <= 0) throw 3;
    I.ignore();
  }
  catch (int i){
    *this = CD();
    return false;
  }
  return true;
}
void CD::salvar(ostream& O) const{ 
  O << "C: ";
  Produto::salvar(O);
  O << ';' << nfaixas;
}

istream& operator>>(istream& I, CD& C){
  C.digitar();
  return I;
}

ostream& operator<<(ostream& O, const CD& C){
  C.salvar(O);
  return O;
}

/// CLASSE DVD

//default
DVD::DVD(const string& N, double P, int D): Produto(N,P), duracao(D) {}

//consulta
int DVD::getDuracao() const{return duracao;}

//recepcao de dados
void DVD::digitar(){
  Produto::digitar();
  do
  {
    cout << "Duracao: ";
    cin >> duracao;
  }
  while (duracao <= 0);
  cin.ignore();
}

void DVD::imprimir() const{salvar(cout);}
bool DVD::ler(istream& I)
{
  try{
    char opcao;
    I >> opcao;
    if (!I.good() || opcao!='D') throw 1;
    I.ignore(numeric_limits<streamsize>::max(),':');
    if (!Produto::ler(I)) throw 2;
    I.ignore(numeric_limits<streamsize>::max(),';');
    I >> duracao;
    if (!I.good() || duracao <= 0) throw 3;
    I.ignore();
  }
  catch (int i){
    *this = DVD();
    return false;
  }
  return true;
}
// Formato da linha-> D: <Conteudo do Produto>;UNSIGNED_DURACAO
void DVD::salvar(ostream& O) const{
  O << "D: ";
  Produto::salvar(O);
  O << ';' << duracao;
}

istream& operator>>(istream& I, DVD& D){
  D.digitar();
  return I;
}

ostream& operator<<(ostream& O, const DVD& D){
  D.salvar(O);
  return O;
}

/// CLASSE LOJA

//default
Loja::Loja(): LL(), LC(), LD() {}

//consulta
int Loja::getNumLivro() const{return LL.size();}
int Loja::getNumCD() const{return LC.size();}
int Loja::getNumDVD() const{return LD.size();}

Livro Loja::getLivro(int id) const{
  if (LL.empty() || id<0 || id>=(int)LL.size()) return Livro();
  return LL[id];
}

CD Loja::getCD(int id) const{
  if (LC.empty() || id<0 || id>=(int)LC.size()) return CD();
  return LC[id];
}

DVD Loja::getDVD(int id) const{
  if (LD.empty() || id<0 || id>=(int)LD.size()) return DVD();
  return LD[id];
}

//funcoes de manipulacao de itens

void Loja::incluirLivro(const Livro& X){LL.push_back(X);}
bool Loja::excluirLivro(int id){
  if (LL.empty() || id<0 || id>=(int)LL.size()) return false;
  LL.erase(LL.begin()+id);
  return true;
}
void Loja::incluirCD(const CD& X){LC.push_back(X);}
bool Loja::excluirCD(int id){
  if (LC.empty() || id<0 || id>=(int)LC.size()) return false;
  LC.erase(LC.begin()+id);
  return true;
}

void Loja::incluirDVD(const DVD& X){LD.push_back(X);}
bool Loja::excluirDVD(int id){
  if (LD.empty() || id<0 || id>=(int)LD.size()) return false;
  LD.erase(LD.begin()+id);
  return true;
}

//recepcao de dados
void Loja::imprimir() const{
  cout << ">> LIVROS:" << endl;
  for (int i=0; i<(int)LL.size(); i++) cout << i << ") " << LL[i] << endl;

  cout << ">> CDS:" << endl;
  for (int i=0; i<(int)LC.size(); i++) cout << i << ") " << LC[i] << endl;

  cout << ">> DVDS:" << endl;
  for (int i=0; i<(int)LD.size(); i++) cout << i << ") " << LD[i] << endl;
}

bool Loja::ler(const string& arq){
  ifstream I(arq.c_str());
  bool resultado=true;
  try
  {
    if (!I.is_open()) throw 1;
    string pS;
    int NN;
    Livro L;
    CD C;
    DVD D;
    //Livros
    I >> pS >> NN;
    if (!I.good() || pS!="LISTALIVRO") throw 2;
    I >> ws;
    LL.clear();
    for (int i=0; i<NN; i++){
      if (!L.ler(I)) throw 3;
      LL.push_back(L);
    }

    //CDs
    I >> pS >> NN;
    if (!I.good() || pS!="LISTACD") throw 4;
    I.ignore();
    LC.clear();
    for (int i=0; i<NN; i++){
      if (!C.ler(I)) throw 5;
      LC.push_back(C);
    }

    //DVDs
    I >> pS >> NN;
    if (!I.good() || pS!="LISTADVD") throw 6;
    I.ignore();
    LD.clear();
    for (int i=0; i<NN; i++){
      if (!D.ler(I)) throw 7;
      LD.push_back(D);
    }
  }
  catch (int i)
  {
    LL.clear();
    LC.clear();
    LD.clear();
    resultado = false;
  }
  if (I.is_open()) I.close();
  return resultado;
}

bool Loja::salvar(const string& arq) const{
  ofstream O(arq.c_str());
  if (!O.is_open()) return false;

  O << "LISTALIVRO " << LL.size() << endl;
  for (int i=0; i<(int)LL.size(); i++) O << LL[i] << endl;

  O << "LISTACD " << LC.size() << endl;
  for (int i=0; i<(int)LC.size(); i++) O << LC[i] << endl;

  O << "LISTADVD " << LD.size() << endl;
  for (int i=0; i<(int)LD.size(); i++) O << LD[i] << endl;

  //O.close();
  return true;
}
