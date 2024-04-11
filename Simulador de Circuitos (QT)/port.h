#ifndef _PORT_H_
#define _PORT_H_

#include <iostream>
#include <string>
#include <vector>
#include "bool3S.h"

/// ###########################################################################
/// ATENCAO PARA A CONVENCAO DOS NOMES PARA OS PARAMETROS DAS FUNCOES:
/// int I: indice (de entrada de porta): de 0 a NInputs-1
/// ###########################################################################

//
// A CLASSE ABSTRATA PORT
//

class Port;
typedef Port* ptr_Port;

class Port {
protected:
  /// ***********************
  /// Dados
  /// ***********************

  // O numero de entradas da porta eh dado pelo tamanho (size) do vetor id_in,
  // que contem as ids das entradas da porta:
  // se id_in.at(i)>0: a i-esima entrada da porta vem da saida da porta cuja id eh o
  // valor desse elemento do array
  // se id_in.at(i)<0: a i-esima entrada da porta vem da entrada do circuito cuja id eh o
  // valor desse elemento do array
  // se id_in.at(i)==0: a i-esima entrada da porta estah indefinida
  std::vector<int> id_in;
  // O valor logico (bool3S) da saida da porta (?, F ou T)
  bool3S out_port;

public:
  /// ***********************
  /// Inicializacao e finalizacao
  /// ***********************

  // Deixa explicito que a classe abstrata Port nao tem construtor default.
  // Os construtores das outras classes que vao herdar de Port terao que chamar
  // explicitamente o construtor especifico de Port, indicando o numero de entradas
  Port() = delete;
  // Construtor especifico (recebe como parametro o numero de entradas da porta)
  // Dimensiona e inicializa os elementos do array id_in com valor invalido (0),
  // Inicializa out_port com UNDEF
  Port(int NI);
  // Destrutor virtual
  virtual ~Port();

  // Essa funcao virtual pura deve criar e retornar um ponteiro para Port que aponta para
  // uma copia de si mesma, do tipo correto.
  // Por exemplo, se for chamada com um objeto Port_AND, retorna um ponteiro que aponta para
  // uma area que contem uma Port_AND cujo valor eh uma copia de *this
  // Deve ser utilizada, por exemplo, no construtor por copia da classe Circuito
  virtual ptr_Port clone() const = 0;

  /// ***********************
  /// Funcoes de testagem
  /// ***********************

  // Retorna true se um numero de entradas eh possivel para a porta, ou seja, se NI>=2
  // (na funcao correspondente na porta NOT, substituir por NI==1)
  virtual bool validNumInputs(int NI) const;

  // Retorna true se um indice (qual entrada da porta) eh valido (entre 0 e NumInputs-1)
  bool validIndex(int I) const;

  // Testa se a dimensao do vetor eh igual ao numero de entradas da porta.
  // Se n�o for, retorna false; caso seja, retorna true.
  // Deve ser utilizada antes de simular uma porta.
  bool testValidSizeInputs(const std::vector<bool3S>& in_port);

  /// ***********************
  /// Funcoes de consulta
  /// ***********************

  // Funcao virtual pura que retorna a sigla correta da Port (AN, NT, OR, NX, etc.)
  // Deve ser utilizada para imprimir uma porta
  virtual std::string getName() const = 0;

  // Retorna o numero de entradas da porta
  int getNumInputs() const;

  // Retorna a saida logica (?, F ou T) da porta
  bool3S getOutput() const;

  // Retorna a Id da I-esima entrada da porta
  // Depois de testar o parametro (validIndex), retorna o valor de id_in.at(I)
  // ou 0 se indice invalido
  int getId_in(int I) const;

  /// ***********************
  /// Funcoes de modificacao
  /// ***********************

  // Fixa o numero de entradas da porta
  // Depois de testar o parametro (validNumInputs), se o novo numero de entradas for igual ao
  // anterior n�o faz nada; caso seja diferente, redimensiona e inicializa os elementos do
  // array id_in com valor invalido (0)
  void setNumInputs(int NI);

  // Fixa o valor logico da saida da porta (?, F, T)
  void setOutput(bool3S S);

  // Fixa a origem do sinal da I-esima entrada da porta como sendo Id
  // Depois de testar os parametros (validIndex, Id!=0), faz: id_in.at(I) = Id
  void setId_in(int I, int Id);

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
  virtual void digitar();

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
  bool ler(std::istream& ArqI);

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
  std::ostream& imprimir(std::ostream& ArqO) const;

  /// ***********************
  /// SIMULACAO (funcao principal da porta)
  /// ***********************

  // Simula uma porta logica
  // Recebe um vector de bool3S com os valores logicos atuais das entradas da porta
  // A implementacao desse metodo virtual nas classes derivadas (Port_AND, etc) deve:
  // 1) Testar se a dimensao do vetor eh igual ao numero de entradas da porta,
  //    utilizando testValidSizeInputs
  // 2) Simular a porta, baseando-se nos operadores AND, OR, etc da classe bool3S.
  // 3) Armazenar o valor bool3S com o resultado da simulacao (saida da porta)
  //    no dado "out_port" da porta
  virtual void simular(const std::vector<bool3S>& in_port)= 0;
};

// Operador << com comportamento polimorfico
// Serve para todas as ports (NO, AND, NOR, etc.)
std::ostream& operator<<(std::ostream& O, const Port& X);

///
/// As outras PORTS
///

class Port_NOT: public Port {
public:
  // Fixa o numero de entradas da porta como sendo 1
  Port_NOT();
  // Retorna new Port_NOT(*this)
  ptr_Port clone() const;
  // Retorna "NT"
  std::string getName() const;

  // Retorna true se um numero de entradas eh possivel para a porta, ou seja, se NI==1
  bool validNumInputs(int NI) const;

  // Leh uma porta NOT do teclado. O usuario deve digitar:
  // - a id da entrada da porta
  // (nao deve ser solicitado a digitar o numero de entradas, que eh sempre 1)
  // Se o usuario digitar um dado invalido, o metodo deve pedir que ele digite novamente
  void digitar();

  // Testa se a dimensao do vetor in_port eh igual ao numero de entradas da porta,
  // utilizando testValidSizeInputs.
  // Se nao for, faz out_port = UNDEF e retorna.
  // Armazena o valor bool3S com o resultado da simulacao (saida da porta)
  // no dado "out_port" da porta
  void simular(const std::vector<bool3S>& in_port);
};

class Port_AND: public Port {
public:
  // Construtor (recebe como parametro o numero de entradas da porta)
  // Testa o numero de entradas com validNumInputs(NI); se invalido, id_in.clear()
  Port_AND(int NI=2);
  // Retorna new Port_AND(*this)
  ptr_Port clone() const;
  // Retorna "AN"
  std::string getName() const;

  // Testa se a dimensao do vetor in_port eh igual ao numero de entradas da porta,
  // utilizando testValidSizeInputs.
  // Se nao for, faz out_port = UNDEF e retorna.
  // Armazena o valor bool3S com o resultado da simulacao (saida da porta)
  // no dado "out_port" da porta
  void simular(const std::vector<bool3S>& in_port);
};

class Port_NAND: public Port {
public:
  // Construtor (recebe como parametro o numero de entradas da porta)
  // Testa o numero de entradas com validNumInputs(NI); se invalido, id_in.clear()
  Port_NAND(int NI=2);
  // Retorna new Port_NAND(*this)
  ptr_Port clone() const;
  // Retorna "NA"
  std::string getName() const;

  // Testa se a dimensao do vetor in_port eh igual ao numero de entradas da porta,
  // utilizando testValidSizeInputs.
  // Se nao for, faz out_port = UNDEF e retorna.
  // Armazena o valor bool3S com o resultado da simulacao (saida da porta)
  // no dado "out_port" da porta
  void simular(const std::vector<bool3S>& in_port);
};

class Port_OR: public Port {
public:
  // Construtor (recebe como parametro o numero de entradas da porta)
  // Testa o numero de entradas com validNumInputs(NI); se invalido, id_in.clear()
  Port_OR(int NI=2);
  // Retorna new Port_OR(*this)
  ptr_Port clone() const;
  // Retorna "OR"
  std::string getName() const;

  // Testa se a dimensao do vetor in_port eh igual ao numero de entradas da porta,
  // utilizando testValidSizeInputs.
  // Se nao for, faz out_port = UNDEF e retorna.
  // Armazena o valor bool3S com o resultado da simulacao (saida da porta)
  // no dado "out_port" da porta
  void simular(const std::vector<bool3S>& in_port);
};

class Port_NOR: public Port {
public:
  // Construtor (recebe como parametro o numero de entradas da porta)
  // Testa o numero de entradas com validNumInputs(NI); se invalido, id_in.clear()
  Port_NOR(int NI=2);
  // Retorna new Port_NOR(*this)
  ptr_Port clone() const;
  // Retorna "NO"
  std::string getName() const;

  // Testa se a dimensao do vetor in_port eh igual ao numero de entradas da porta,
  // utilizando testValidSizeInputs.
  // Se nao for, faz out_port = UNDEF e retorna.
  // Armazena o valor bool3S com o resultado da simulacao (saida da porta)
  // no dado "out_port" da porta
  void simular(const std::vector<bool3S>& in_port);
};

class Port_XOR: public Port {
public:
  // Construtor (recebe como parametro o numero de entradas da porta)
  // Testa o numero de entradas com validNumInputs(NI); se invalido, id_in.clear()
  Port_XOR(int NI=2);
  // Retorna new Port_XOR(*this)
  ptr_Port clone() const;
  // Retorna "XO"
  std::string getName() const;

  // Testa se a dimensao do vetor in_port eh igual ao numero de entradas da porta,
  // utilizando testValidSizeInputs.
  // Se nao for, faz out_port = UNDEF e retorna.
  // Armazena o valor bool3S com o resultado da simulacao (saida da porta)
  // no dado "out_port" da porta
  void simular(const std::vector<bool3S>& in_port);
};

class Port_NXOR: public Port {
public:
  // Construtor (recebe como parametro o numero de entradas da porta)
  // Testa o numero de entradas com validNumInputs(NI); se invalido, id_in.clear()
  Port_NXOR(int NI=2);
  // Retorna new Port_NXOR(*this)
  ptr_Port clone() const;
  // Retorna "NX"
  std::string getName() const;

  // Testa se a dimensao do vetor in_port eh igual ao numero de entradas da porta,
  // utilizando testValidSizeInputs.
  // Se nao for, faz out_port = UNDEF e retorna.
  // Armazena o valor bool3S com o resultado da simulacao (saida da porta)
  // no dado "out_port" da porta
  void simular(const std::vector<bool3S>& in_port);
};

#endif // _PORT_H_
