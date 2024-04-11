#ifndef _CIRCUITO_H_
#define _CIRCUITO_H_

#include <iostream>
#include <string>
#include <vector>
#include "bool3S.h"
#include "port.h"

/// ###########################################################################
/// ATENCAO PARA A CONVENCAO DOS NOMES PARA OS PARAMETROS DAS FUNCOES:
/// int I: indice (de entrada de porta): de 0 a NInputs-1
/// int Id___: identificador (de entrada, saida ou porta): nao 0
/// - int IdInput: de -1 a -NumEntradasCircuito
/// - int IdOutput: de 1 a NumSaidasCircuito
/// - int IdPort: de 1 a NumPortasCircuito
/// - int IdOrig: de 1 a NumPortasCircuito ou de -1 a -NumEntradasCircuito
///       (id da origem de uma entrada de porta ou de uma saida do circuito)
/// ###########################################################################

///
/// CLASSE CIRCUIT
///

class Circuito {
private:
  /// ***********************
  /// Dados
  /// ***********************

  // Numero de entradas do circuito
  int Nin;

  // Nao precisa manter variaveis para guardar o numero de saidas e ports.
  // Essas informacoes estao armazenadas nos tamanhos (size) dos vetores correspondentes:
  // id_out e ports, respectivamente
  // Os metodos de consulta getNumInputs, getNumOutputs e getNumPorts dao acesso a essas
  // informacoes de maneira eficiente

  // As saidas
  // As ids da origem dos sinais de saida do circuito
  std::vector<int> id_out;      // vetor a ser alocado com dimensao "Nout"
  // Os valores logicos das saidas do circuito
  std::vector<bool3S> out_circ; // vetor a ser alocado com dimensao "Nout"

  // As portas
  std::vector<ptr_Port> ports;  // vetor a ser alocado com dimensao "Nports"

public:

  /// ***********************
  /// Inicializacao e finalizacao
  /// ***********************

  // As variaveis do tipo Circuit sao sempre criadas sem nenhum dado
  // A definicao do numero de entradas, saidas e ports eh feita ao ler do teclado ou arquivo
  // ou ao executar o metodo resize
  Circuito();

  // Construtor por copia
  // Nin e os vetores id_out e out_circ serao copias dos equivalentes no Circuit C
  // O vetor ports terah a mesma dimensao do equivalente no Circuit C
  // Serah necessario utilizar a funcao virtual clone para criar copias das portas
  Circuito(const Circuito& C);
  // Construtor por movimento
  // Nin e os vetores id_out, out_circ e ports assumirao o conteudo dos equivalentes
  // no Circuit temporario C, que serah zerado
  Circuito(Circuito&& C);

  // Destrutor: apenas chama a funcao clear()
  ~Circuito();
  // Limpa todo o conteudo do circuito. Faz Nin = 0 e
  // utiliza o metodo STL clear para limpar os vetores id_out, out_circ e ports
  // ATENCAO: antes de dar um clear no vetor ports, tem que liberar (delete) as areas
  // de memoria para as quais cada ponteiro desse vetor aponta.
  void clear();

  // Operador de atribuicao por copia
  // Limpa (clear) e depois atribui (faz copia) Nin e os vetores id_out e out_circ
  // O vetor ports terah a mesma dimensao do equivalente no Circuit C
  // Serah necessario utilizar a funcao virtual clone para criar copias das portas
  Circuito& operator=(const Circuito& C);
  // Operador de atribuicao por movimento
  // Limpa (clear) e depois move (swap) Nin e os vetores id_out, out_circ e ports
  Circuito& operator=(Circuito&& C);

  // Redimensiona o circuito para passar a ter NI entradas, NO saidas e NP ports
  // Inicialmente checa os parametros. Caso sejam validos:
  // - limpa conteudo anterior (clear);
  // - altera Nin;
  // - os vetores tem as dimensoes alteradas e sao inicializados (usando resize)
  //   com valores iniciais neutros ou invalidos: id_out com valor 0,
  //   out_circ com valor UNDEF e ports com valor nullptr
  void resize(int NI, int NO, int NP);

  /// ***********************
  /// Funcoes de testagem
  /// ***********************

  // Retorna true se IdInput eh uma id de entrada do circuito valida (entre -1 e -NInput)
  bool validIdInput(int IdInput) const;

  // Retorna true se IdOutput eh uma id de saida do circuito valida (entre 1 e NOutput)
  bool validIdOutput(int IdOutput) const;

  // Retorna true se IdPort eh uma id de porta do circuito valida (entre 1 e NPort)
  bool validIdPort(int IdPort) const;

  // Retorna true se IdOrig eh uma id valida para a origem do sinal de uma entrada de porta ou
  // para a origem de uma saida do circuito (podem vir de uma entrada do circuito ou de uma porta)
  // validIdOrig == validIdInput OR validIdPort
  bool validIdOrig(int IdOrig) const;

  // Retorna true se IdPort eh uma id de porta valida (validIdPort) e
  // a porta estah definida (estah alocada, ou seja, != nullptr)
  bool definedPort(int IdPort) const;

  // Retorna true se IdPort eh uma porta existente (definedPort) e
  // todas as entradas da porta com Id de origem valida (usa getId_inPort e validIdOrig)
  bool validPort(int IdPort) const;

  // Retorna true se o circuito eh valido (estah com todos os dados corretos):
  // - numero de entradas, saidas e portas valido (> 0)
  // - todas as portas validas (usa validPort)
  // - todas as saidas com Id de origem validas (usa getIdOutput e validIdOrig)
  // Essa funcao deve ser usada antes de salvar ou simular um circuito
  bool valid() const;

  /// ***********************
  /// Funcoes de consulta
  /// ***********************

  // Caracteristicas do circuito

  // Retorna o numero de entradas Nin
  int getNumInputs() const;
  // Retorna o tamanho (size) dos vetores correspondentes:
  // id_out e ports, respectivamente
  int getNumOutputs() const;
  int getNumPorts() const;

  // Caracteristicas das saidas do circuito

  // Retorna a origem (a id) do sinal de saida cuja id eh IdOutput
  // Depois de testar o parametro (validIdOutput), retorna id_out.at(IdOutput-1)
  // ou 0 se parametro invalido
  int getIdOutput(int IdOutput) const;

  // Retorna o valor logico atual da saida cuja id eh IdOutput
  // Depois de testar o parametro (validIdOutput), retorna out_circ.at(IdOutput-1)
  // ou bool3S::UNDEF se parametro invalido
  bool3S getOutput(int IdOutput) const;

  // Caracteristicas das portas

  // Retorna o nome da porta: AN, NX, etc
  // Depois de testar se a porta existe (definedPort),
  // retorna ports.at(IdPort-1)->getName()
  // ou "??" se parametro invalido
  std::string getNamePort(int IdPort) const;

  // Retorna o numero de entradas da porta
  // Depois de testar se a porta existe (definedPort),
  // retorna ports.at(IdPort-1)->getNumInputs()
  // ou 0 se parametro invalido
  int getNumInputsPort(int IdPort) const;

  // Retorna a origem (a id) da I-esima entrada da porta cuja id eh IdPort
  // Depois de testar se a porta IdPort existe (definedPort)
  // e se o indice I eh valido (Port::validIndex),
  // retorna ports.at(IdPort-1)->getId_in(I) ou 0 se parametro invalido
  // Nao eh obrigatorio testar I nessa funcao pois getId_in jah testa.
  int getId_inPort(int IdPort, int I) const;

  /// ***********************
  /// Funcoes de modificacao
  /// ***********************

  // Caracteristicas das saidas

  // Altera a origem da saida de id "IdOut", que passa a ser "IdOrig"
  // Depois de testar os parametros (validIdOutput,validIdOrig),
  // faz: id_out.at(IdOut-1) = IdOrig
  void setIdOutput(int IdOut, int IdOrig);

  // Caracteristicas das ports

  // A porta cuja id eh IdPort passa a ser do tipo Tipo (NT, AN, etc.), com NIn entradas
  // Depois de varios testes (Id, tipo, num de entradas), faz:
  // 1) Libera a antiga area de memoria: delete ports.at(IdPort-1)
  // 2) Cria a nova porta: ports.at(IdPort-1) = new ... (de acordo com tipo)
  // 3) Fixa o numero de entrada: ports.at(IdPort-1)->setNumInputs(NIn)
  void setPort(int IdPort, std::string Tipo, int NIn);

  // Altera a origem da I-esima entrada da porta cuja id eh IdPort, que passa a ser "IdOrig"
  // Depois de varios testes (definedPort, Port::validIndex, validIdOrig),
  // faz: ports.at(IdPort-1)->setId_in(I,Idorig)
  void setId_inPort(int IdPort, int I, int IdOrig) const;

  /// ***********************
  /// E/S de dados
  /// ***********************

  // Entrada dos dados de um circuito via teclado
  // Se o usuario digitar um dado invalido, o metodo deve pedir que ele digite novamente
  // Inicialmente, o usuario digita o numero de entradas, saidas e portas
  // Apos certificar que dados sao validos, redimensiona (resize) o circuito.
  // Em seguida, para cada porta o usuario digita o tipo (NT,AN,NA,OR,NO,XO,NX),
  // que eh conferido (validType).
  // Apos criada dinamicamente a porta do tipo correto (allocPort), chama a funcao digitar
  // da classe Port na porta recem-criada. Cada porta digitada eh conferida (validPort).
  // Em seguida, o usuario digita as ids de origem de cada uma das saidas,
  // que sao conferidas (validIdOrig)
  void digitar();

  // Entrada dos dados de um circuito via arquivo
  // Se ler um dado invalido, o metodo deve encerrar a leitura, fazer o circuito ficar
  // vazio e retornar false.
  // Leh do arquivo o cabecalho "CIRCUITO" e o numero de entradas, saidas e portas
  // Apos certificar que dados sao validos, redimensiona (resize) o circuito.
  // Leh do arquivo o cabecalho "PORTAS"
  // Em seguida, para cada porta leh e confere a id (==i+1) e o tipo (validType)
  // Apos criada dinamicamente (allocPort) a porta do tipo correto, chama a
  // funcao ler da classe Port na porta recem-criada. O retorno da leitura da porta eh
  // conferido bem como se a porta lida eh valida (validPort) para o circuito.
  // Leh do arquivo o cabecalho "SAIDAS"
  // Em seguida, para cada saida leh e confere a id (==i+1) e id de origem (validIdOrig)
  // Retorna true se deu tudo OK.
  bool ler(const std::string& arq);

  // Saida dos dados de um circuito (em tela ou arquivo, a mesma funcao serve para os dois)
  // Imprime os cabecalhos e os dados do circuito, caso o circuito seja valido
  // Deve utilizar os metodos de impressao da classe Port
  // Retorna uma referencia aa mesma ostream que recebeu como parametro
  std::ostream& imprimir(std::ostream& O=std::cout) const;

  // Salvar circuito em arquivo, caso o circuito seja valido
  // Abre a stream, chama o metodo imprimir e depois fecha a stream
  // Retorna true se deu tudo OK; false se deu erro
  bool salvar(const std::string& arq) const;

  /// ***********************
  /// SIMULACAO (funcao principal do circuito)
  /// ***********************

  // Calcula a saida das ports do circuito para os valores de entrada
  // passados como parametro, caso o circuito e a dimensao da entrada sejam validos
  // (valid, in_circ.size== getNumInputs). Caso contrario retorna false.
  // A entrada eh um vetor de bool3S, com dimensao igual ao numero de entradas do circuito.
  // Depois de simular todas as portas do circuito, calcula as saidas do circuito (out_circ)
  // Retorna true se a simulacao foi OK; false caso deh erro
  bool simular(const std::vector<bool3S>& in_circ);

};

// Operador de impressao da classe Circuit
// Utiliza o metodo imprimir
// Retorna uma referencia aa mesma ostream que recebeu como parametro
std::ostream& operator<<(std::ostream& O, const Circuito& C);

#endif // _CIRCUITO_H_
