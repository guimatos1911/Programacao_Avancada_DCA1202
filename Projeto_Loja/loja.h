#ifndef _LOJA_H_
#define _LOJA_H_

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Produto
{
private:
  string nome;
  double preco;
public:
  //construtor default
  Produto(const string& N="", double P=0);

  //funcoes de consulta
  string getNome() const;
  double getPreco() const;

  //metodos de entrada/saida
  void digitar();
  void imprimir() const;

  //metodos de abordagem de arquivos
  bool ler(istream& I);
  void salvar(ostream& O) const;
};
//>> e <<
istream& operator>>(istream& I, Produto& P);
ostream& operator<<(ostream& O, const Produto& P);

class Livro: public Produto
{
private:
  string autor;
public:
  //construtor default
  Livro(const string& N="", double P=0, const string& A="");

  //funcoes de consulta
  string getAutor() const;

  //funcoes de entrada/saida
  void digitar();
  void imprimir() const;

  //metodos de abordagem de arquivos
  bool ler(istream& I);
  void salvar(ostream& O) const;
};
//>> e <<
istream& operator>>(istream& I, Livro& L);
ostream& operator<<(ostream& O, const Livro& L);

class CD: public Produto
{
private:
  int nfaixas;
public:
  CD(const string& N="", double P=0, int NF=0);
  int getNumFaixas() const;

  void digitar();
  void imprimir() const;

  bool ler(istream& I);
  void salvar(ostream& O) const;
};
istream& operator>>(istream& I, CD& C);
ostream& operator<<(ostream& O, const CD& C);

class DVD: public Produto
{
private:
  int duracao;
public:
  DVD(const string& N="", double P=0, int D=0);
  int getDuracao() const;

  void digitar();
  void imprimir() const;

  bool ler(istream& I);
  void salvar(ostream& O) const;
};
istream& operator>>(istream& I, DVD& D);
ostream& operator<<(ostream& O, const DVD& D);

class Loja
{
private:
  vector<Livro> LL;
  vector<CD>    LC;
  vector<DVD>   LD;
public:
  Loja();
  int getNumLivro() const;
  int getNumCD() const;
  int getNumDVD() const;
  Livro getLivro(int id) const;
  CD getCD(int id) const;
  DVD getDVD(int id) const;

  //funcoes de manipulacao de itens
  void incluirLivro(const Livro& X);
  bool excluirLivro(int id);
  void incluirCD(const CD& X);
  bool excluirCD(int id);
  void incluirDVD(const DVD& X);
  bool excluirDVD(int id);

  //funcoes de entrada/saida
  bool ler(const string& arq);
  bool salvar(const string& arq) const;
  void imprimir() const;
};
#endif // _LOJA_H_
