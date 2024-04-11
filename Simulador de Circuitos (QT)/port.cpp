#include <fstream>
#include "port.h"

//
// CLASSE PORT
//

/// ***********************
/// Inicializacao e finalizacao
/// ***********************

// Construtor (recebe como parametro o numero de entradas da porta)
// Dimensiona o array id_in e inicializa elementos com valor invalido (0),
// inicializa out_port com UNDEF
Port::Port(int NI):id_in(NI,0),out_port(bool3S::UNDEF)
{
  // Nao pode testar o parametro NI com validNumInputs pq o construtor de
  // Port eh chamado pelo construtor de Port_NOT, mas sem que ocorra
  // polimorfismo nesse momento.
  // Entao seria chamada a versao base de validNumInputs, que testaria se
  // estah entre 2 e 4 e geraria erro ao criar portas NOT
  // Esse teste (validNumInputs) tem que ser feito no construtor das outras portas
}

// Destrutor (nao faz nada)
Port::~Port() {}

/// ***********************
/// Funcoes de testagem
/// ***********************

// Retorna true se um numero de entradas eh possivel para a porta, ou seja, se NI >= 2
// (na funcao correspondente na porta NOT, substituir por NI==1)
bool Port::validNumInputs(int NI) const
{
  if (NI>=2) return true;

  /// Mensagem de erro para debug. Remova na versao final,
  // especialmente se estiver utilizando interface com programacao visual
  //std::cerr << "ERRO Port::validNumInputs - numero de entradas (" << NI
            //<< ") invalido para porta do tipo " << getName() << std::endl;

  return false;
}

// Retorna true se um indice (qual entrada da porta) eh valido (entre 0 e NumInputs-1)
bool Port::validIndex(int I) const
{
  if (I>=0 && I<getNumInputs()) return true;

  // Mensagem de erro para debug. Remova na versao final,
  // especialmente se estiver utilizando interface com programacao visual
  //std::cerr << "ERRO Port::validIndex - indice (" << I
           // << ") invalido para porta do tipo " << getName()
           // << " com " << getNumInputs() << " entradas" << std::endl;

  return false;
}

// Testa se a dimensao do vetor eh igual ao numero de entradas da porta.
// Se n�o for, retorna false; caso seja, retorna true.
// Deve ser utilizada antes de simular uma porta.
bool Port::testValidSizeInputs(const std::vector<bool3S>& in_port)
{
  if (int(in_port.size()) == getNumInputs()) return true;

  // Mensagem de erro para debug. Remova na versao final,
  // especialmente se estiver utilizando interface com programacao visual
 // std::cerr << "ERRO Port::testValidSizeInputs - vetor de entrada (tamanho " << in_port.size()
           // << ") invalido para porta do tipo " << getName()
           // << " com " << getNumInputs() << " entradas" << std::endl;

  return false;
}

/// ***********************
/// Funcoes de consulta
/// ***********************

// Retorna o numero de entradas da porta
int Port::getNumInputs() const
{
  return id_in.size();
}

// Retorna a saida logica (?, F ou T) da porta
bool3S Port::getOutput() const
{
  return out_port;
}

// Retorna a Id da I-esima entrada da porta
// Depois de testar o parametro (validIndex), retorna o valor de id_in.at(I)
// ou 0 se indice invalido
int Port::getId_in(int I) const
{
  if (!validIndex(I)) return 0;
  return id_in.at(I);
}

/// ***********************
/// Funcoes de modificacao
/// ***********************

// Fixa o numero de entradas da porta
// Depois de testar o parametro (validNumInputs), se o novo numero de entradas for igual ao
// anterior n�o faz nada; caso seja diferente, redimensiona e inicializa os elementos
// do array id_in com valor invalido (0)
void Port::setNumInputs(int NI)
{
  // Soh altera se for valido e diferente do valor atual
  if (validNumInputs(NI) && NI!=getNumInputs())
  {
    id_in.resize(NI);
    for (int i=0; i<getNumInputs(); i++) id_in.at(i) = 0;
  } //else std::cerr << "Erro, valor improprio de inputs" << std::endl;  //isso aqui alterei por conta do c�digo teste.1

}

// Fixa o valor logico da saida da porta (?, F, T)
void Port::setOutput(bool3S S)
{
  out_port=S;
}

// Fixa a origem do sinal da I-esima entrada da porta como sendo Id
// Depois de testar os parametros (validIndex, Id!=0), faz: id_in.at(I) = Id
void Port::setId_in(int I, int Id)
{
  if (validIndex(I) && Id!=0) id_in.at(I) = Id;
}

/// ***********************
/// E/S de dados
/// ***********************

// Leh uma porta do teclado. O usuario deve digitar:
// - o numero de entradas da porta (testa com validNumInputs); e
// - a id de origem de cada uma das entradas da porta (testa se != 0)
// Se o usuario digitar um dado invalido, o metodo deve pedir que ele digite novamente
// Este metodo para digitar uma porta com numero variavel de entradas (>= 2)
// vale para todas as ports, exceto NOT.
// O metodo virtual digitar tem que ser refeito para a NOT.
// Nao precisa ser reimplementado nas demais ports
// ATENCAO: o metodo NAO vai solicitar que o usuario digite o tipo de porta
// (NT,AN,NA,OR,NO,XO,NX). Esse valor jah deve ter sido digitado previamente e a porta
// criada dinamicamente (new) do tipo certo, para que seja chamado o metodo virtual
// digitar apropriado para o tipo de porta.
void Port::digitar()
{
  int Nin;
  // Leh o numero de entradas da porta
  do
  {
    std::cout << "  Numero de entradas da porta: ";
    std::cin >> Nin;
  }
  while (!validNumInputs(Nin));
  // Altera o numero de entradas da porta
  setNumInputs(Nin);
  // Leh as ids de origem das entradas da porta
  for (int i=0; i<getNumInputs(); i++)
  {
    do
    {
      std::cout << "  Id de origem da entrada indice " << i << " da porta: ";
      std::cin >> id_in.at(i);
    }
    while (id_in.at(i)==0);
  }
}

// Leh uma porta da stream ArqI. Deve fixar o valor:
// - do numero de entradas da porta; e
// - da id de origem de cada uma das entradas da porta
// Retorna true se tudo OK, false se houve erro
// Este metodo nao eh virtual, pois pode ser feito generico de forma a servir para
// todas as ports.
// Basta que o metodo teste o numero de entradas com a funcao virtual validNumInputs()
// ATENCAO: o metodo NAO vai ler do arquivo o tipo de porta
// (NT,AN,NA,OR,NO,XO,NX). Esse valor jah deve ter sido lido previamente e a porta
// criada dinamicamente do tipo certo, para que seja chamado o metodo virtual
// apropriado para o tipo de porta.
bool Port::ler(std::istream& ArqI)
{
  try
  {
    int Nin;
    char c;

    // Leh o numero de entradas da porta
    ArqI >> Nin;
    if (!ArqI.good() || !validNumInputs(Nin)) throw 1;
    // Altera o numero de entradas da porta
    setNumInputs(Nin);
    // Leh o "("
    ArqI >> c;
    if (!ArqI.good() || c != ':') throw 2;
    // Leh as ids de origem das entradas da porta
    for (int i=0; i<getNumInputs(); i++)
    {
      ArqI >> id_in.at(i);
      if (!ArqI.good() || id_in.at(i) == 0) throw 3;
    }
  }
  catch (int erro)
  {
    // Mensagem de erro para debug. Remova na versao final,
    // especialmente se estiver utilizando interface com programacao visual
   // std::cerr << "ERRO Port::ler - tipo " << erro << std::endl;

    id_in.clear();
    return false;
  }
  return true;
}

// Imprime a porta na ostream ArqO (cout ou uma stream de arquivo, tanto faz)
// Imprime:
// - a string com o nome da porta + ESPACO
// - o numero de entradas colado com ':'; e
// - ESPACO + as ids de cada uma das entradas
// Este metodo nao eh virtual, pois pode ser feito generico de forma a servir para
// todas as ports.
// Basta que o metodo imprima o resultado da chamada aa funcao virtual getName() para o nome
// Os outros dados a serem impressos sao iguais em todas as portas
// Retorna a propria ostream O recebida como parametro de entrada, para que possa
// ser encadeada
std::ostream& Port::imprimir(std::ostream& ArqO) const
{
  ArqO << this->getName() << ' ';
  ArqO << getNumInputs() << ':';
  for (int j=0; j<getNumInputs(); j++)
  {
    ArqO << ' ' << id_in.at(j);
  }
  return ArqO;
}

// Operador << com comportamento polimorfico
// Serve para todas as ports (NO, AND, NOR, etc.)
std::ostream& operator<<(std::ostream& O, const Port& X)
{
  return X.imprimir(O);
};

///
/// AS OUTRAS PORTS
///

/// Implementa��o das classes derivadas

//Construtors das classes derivadas

Port_NOT::Port_NOT() : Port(1) {
}

Port_AND::Port_AND(int NI) : Port(NI) {
}

Port_NAND::Port_NAND(int NI) : Port(NI) {
}

Port_OR::Port_OR(int NI) : Port(NI) {
}

Port_NOR::Port_NOR(int NI) : Port(NI) {
}

Port_XOR::Port_XOR(int NI) : Port(NI) {
}

Port_NXOR::Port_NXOR(int NI) : Port(NI) {
}

//Ponteiros das classes derivadas

ptr_Port Port_NOT::clone() const {
    return new Port_NOT(*this);
}

ptr_Port Port_AND::clone() const {
    return new Port_AND(*this);
}

ptr_Port Port_NAND::clone() const {
    return new Port_NAND(*this);
}

ptr_Port Port_OR::clone() const {
    return new Port_OR(*this);
}

ptr_Port Port_NOR::clone() const {
    return new Port_NOR(*this);
}

ptr_Port Port_XOR::clone() const {
    return new Port_XOR(*this);
}

ptr_Port Port_NXOR::clone() const {
    return new Port_NXOR(*this);
}

//getName das classes derivadas

std::string Port_NOT::getName() const {
    return "NT";
}

std::string Port_AND::getName() const {
    return "AN";
}

std::string Port_NAND::getName() const {
    return "NA";
}

std::string Port_OR::getName() const {
    return "OR";
}

std::string Port_NOR::getName() const {
    return "NO";
}

std::string Port_XOR::getName() const {
    return "XO";
}

std::string Port_NXOR::getName() const {
    return "NX";
}

//Outros m�todos da classe Port_NOT
bool Port_NOT::validNumInputs(int NI) const {
   return NI == 1;
}
void Port_NOT::digitar() {
    std::cout << "Digite a id de entrada: ";
    std::cin >> id_in.at(0);
    while(id_in.at(0) == 0){
        std::cout <<"ID invalida, digite novamente: ";
        std::cin >> id_in.at(0);
    }
}

//M�todos simular das classes derivadas Port

void Port_NOT::simular(const std::vector<bool3S>& in_port) {
    // Implementa��o para simular a porta NOT
    // Testa se a dimensao do vetor in_port eh igual ao numero de entradas da porta
    if (!testValidSizeInputs(in_port)) {
        out_port = bool3S::UNDEF;
        return;
    }
    // Realiza a simula��o da porta NOT
    // L�gica da porta NOT: inverte a entrada
    out_port = ~in_port[0];
}

void Port_AND::simular(const std::vector<bool3S>& in_port) {
    // Implementa��o para simular a porta AND
    if (!testValidSizeInputs(in_port)) {
        out_port = bool3S::UNDEF;
        return;
    }
    out_port = in_port[0];
    for (size_t i = 1; i < in_port.size(); ++i){
        out_port = (out_port & in_port[i]);
    }
}

void Port_NAND::simular(const std::vector<bool3S>& in_port) {
    // Implementa��o para simular a porta NAND
    if (!testValidSizeInputs(in_port)) {
        out_port = bool3S::UNDEF;
        return;
    }
    out_port = in_port[0];
    for(size_t i = 1; i < in_port.size(); ++i){
        out_port = out_port & in_port[i];
    }
    //not de AND
    out_port = ~out_port;
}

void Port_OR::simular(const std::vector<bool3S>& in_port) {
    // Implementa��o para simular a porta OR
    if (!testValidSizeInputs(in_port)) {
        out_port = bool3S::UNDEF;
        return;
    }
    out_port = in_port[0];
    for(size_t i = 1; i < in_port.size(); ++i){
        out_port = (out_port | in_port[i]);
    }
}

void Port_NOR::simular(const std::vector<bool3S>& in_port) {
    // Implementa��o para simular a porta NOR
    if (!testValidSizeInputs(in_port)) {
        out_port = bool3S::UNDEF;
        return;
    }
    out_port = in_port[0];
    for (size_t i = 1; i < in_port.size(); ++i){ //in_port.size()  - > getNumInputs
        out_port = (out_port | in_port[i]);
    }
    //not de OR
    out_port = ~out_port;
}

void Port_XOR::simular(const std::vector<bool3S>& in_port) {
    // Implementa��o para simular a porta XOR
    if (!testValidSizeInputs(in_port)) {
        out_port = bool3S::UNDEF;
        return;
    }
    out_port = in_port[0];
    for(size_t i = 1; i < in_port.size(); ++i){
        out_port ^= in_port[i];
    }
}

void Port_NXOR::simular(const std::vector<bool3S>& in_port) {
    // Implementa��o para simular a porta NXOR
    if (!testValidSizeInputs(in_port)) {
        out_port = bool3S::UNDEF;
        return;
    }
    out_port = in_port[0];
    for (size_t i = 1; i < in_port.size(); ++i){
        out_port ^= in_port[i];
    }
    //not de XOR
    out_port = ~out_port;
}
