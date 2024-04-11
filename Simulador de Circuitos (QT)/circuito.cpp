#include <fstream>
#include <utility> // para std::swap
#include "circuito.h"

///
/// As strings que definem os tipos de porta
///

// Funcao auxiliar que testa se uma string com nome de porta eh valida
// Caso necessario, converte os caracteres da string para maiusculas
bool validType(std::string& Tipo)
{
  try
  {
    if (Tipo.size()!=2) throw 1;
    Tipo.at(0) = toupper(Tipo.at(0));
    Tipo.at(1) = toupper(Tipo.at(1));
    if (Tipo!="NT" &&
        Tipo!="AN" && Tipo!="NA" &&
        Tipo!="OR" && Tipo!="NO" &&
        Tipo!="XO" && Tipo!="NX") throw 2;
  }
  catch (int erro)
  {
    // Mensagem de erro para debug. Remova na versao final,
    // especialmente se estiver utilizando interface com programacao visual
    //std::cerr << "ERRO validType - string (" << Tipo
              //<< ") invalida - erro tipo " << erro << std::endl;

    return false;
  }
  return true;
}

// Funcao auxiliar que retorna um ponteiro que aponta para uma porta alocada dinamicamente
// O tipo da porta alocada depende do parametro string de entrada (AN, OR, etc.)
// Caso o tipo nao seja nenhum dos validos, retorna nullptr
// Pode ser utilizada nas funcoes: Circuito::setPort, Circuito::digitar e Circuito::ler
ptr_Port allocPort(std::string& Tipo)
{
  if (!validType(Tipo)) return nullptr;

  if (Tipo=="NT") return new Port_NOT;
  if (Tipo=="AN") return new Port_AND;
  if (Tipo=="NA") return new Port_NAND;
  if (Tipo=="OR") return new Port_OR;
  if (Tipo=="NO") return new Port_NOR;
  if (Tipo=="XO") return new Port_XOR;
  if (Tipo=="NX") return new Port_NXOR;

  // Nunca deve chegar aqui...
  return nullptr;
}

///
/// CLASSE CIRCUITO
///

/// ***********************
/// Inicializacao e finalizacao
/// ***********************

// As variaveis do tipo Circuito sao sempre criadas sem nenhum dado
// A definicao do numero de entradas, saidas e ports eh feita ao ler do teclado ou arquivo
// ou ao executar o metodo resize
Circuito::Circuito():Nin(0),id_out(),out_circ(),ports() {}

// Construtor por copia
// Nin e os vetores id_out e out_circ serao copias dos equivalentes no Circuito C
// O vetor ports terah a mesma dimensao do equivalente no Circuito C
// Serah necessario utilizar a funcao virtual clone para criar copias das portas

Circuito::Circuito(const Circuito& C) : Nin(C.Nin), id_out(C.id_out), out_circ(C.out_circ), ports() {
    for(const auto& port : C.ports){
            ports.push_back(port ? port->clone():nullptr);
    }
}

// Construtor por movimento
// Nin e os vetores id_out, out_circ e ports assumirao o conteudo dos equivalentes
// no Circuit temporario C, que serah zerado

Circuito::Circuito(Circuito&& C) : Nin(C.Nin), id_out(std::move(C.id_out)), out_circ(std::move(C.out_circ)), ports(std::move(C.ports))
{
    C.clear(); //inicializa Nin e os vetores com os valores em C, depois realiza clear(zera) em C
}

// Destrutor: chama a funcao clear()
Circuito::~Circuito()
{
  clear();
}

// Limpa todo o conteudo do circuito. Faz Nin = 0 e
// utiliza o metodo STL clear para limpar os vetores id_out, out_circ e ports
// ATENCAO: antes de dar um clear no vetor ports, tem que liberar (delete) as areas
// de memoria para as quais cada ponteiro desse vetor aponta.
void Circuito::clear()
{
    //tudo na ordem da especificacao
    Nin = 0;
    id_out.clear();
    out_circ.clear();
    for (auto& portPtr : ports) delete portPtr;
    ports.clear();
;
}

// Operador de atribuicao por copia
// Limpa (clear) e depois atribui (faz copia) Nin e os vetores id_out e out_circ
// O vetor ports terah a mesma dimensao do equivalente no Circuit C
// Serah necessario utilizar a funcao virtual clone para criar copias das portas
Circuito& Circuito::operator=(const Circuito& C)
{
    if(this != &C){ // --> para evitar autoatribui��o do operador
        clear(); //Limpa
        // Faz copia do conteudo de C
        Nin = C.Nin;
        id_out = C.id_out;
        out_circ = C.out_circ;

        ports.resize(C.ports.size());
        for(size_t i = 0; i < C.ports.size(); ++i){
            if (C.ports[i]) ports[i] = C.ports[i] ->clone();
            else ports[i] = nullptr;
        }
    }
    return *this;
}

// Operador de atribuicao por movimento
// Limpa (clear) e depois move/troca (swap) Nin e os vetores id_out, out_circ e ports
Circuito& Circuito::operator=(Circuito&& C)
{
    if(this != &C){ // --> para evitar autoatribui��o, tal qual no operador por c�pia
        clear();
        std::swap(Nin, C.Nin);
        std::swap(id_out,C.id_out);
        std::swap(out_circ,C.out_circ);
        std::swap(ports, C.ports);
    }
    return *this;
}

// Redimensiona o circuito para passar a ter NI entradas, NO saidas e NP ports
// Inicialmente checa os parametros. Caso sejam validos:
// - limpa conteudo anterior (clear);
// - altera Nin;
// - os vetores tem as dimensoes alteradas e sao inicializados (usando resize)
//   com valores iniciais neutros ou invalidos: id_out com valor 0,
//   out_circ com valor UNDEF e ports com valor nullptr
void Circuito::resize(int NI, int NO, int NP)
{
  if (NI<=0 || NO<=0 || NP<=0)
  {
    // Mensagem de erro para debug. Remova na versao final,
    // especialmente se estiver utilizando interface com programacao visual
   // std::cerr << "ERRO Circuito::resize - NI=" << NI
              //<< ",NP=" << NP
              //<< ",NO=" << NO << std::endl;
    return;
  }
    clear(); //limpa conteudo
    Nin = NI; //altera nin
    //resize nos vetores utilizando valores desejados
    id_out.resize(NO, 0);
    out_circ.resize(NO, bool3S::UNDEF);
    ports.resize(NP, nullptr);
}

/// ***********************
/// Funcoes de testagem
/// ***********************

// Retorna true se IdInput eh uma id de entrada do circuito valida (entre -1 e -NInput)
bool Circuito::validIdInput(int IdInput) const
{
  return (IdInput<=-1 && IdInput>=-getNumInputs());
}

// Retorna true se IdOutput eh uma id de saida do circuito valida (entre 1 e NOutput)
bool Circuito::validIdOutput(int IdOutput) const
{
  return (IdOutput>=1 && IdOutput<=getNumOutputs());
}

// Retorna true se IdPort eh uma id de porta do circuito valida (entre 1 e NPort)
bool Circuito::validIdPort(int IdPort) const
{
  return (IdPort>=1 && IdPort<=getNumPorts());
}

// Retorna true se IdOrig eh uma id valida para a origem do sinal de uma entrada de porta ou
// para a origem de uma saida do circuito (podem vir de uma entrada do circuito ou de uma porta)
// validIdOrig == validIdInput OR validIdPort
bool Circuito::validIdOrig(int IdOrig) const
{
  return validIdInput(IdOrig) || validIdPort(IdOrig);
}

// Retorna true se IdPort eh uma id de porta valida (validIdPort) e
// a porta estah definida (estah alocada, ou seja, != nullptr)
bool Circuito::definedPort(int IdPort) const
{
  if (validIdPort(IdPort) && ports.at(IdPort-1)!=nullptr) return true;
  return false;
}

// Retorna true se IdPort eh uma porta existente (definedPort) e
// todas as entradas da porta com Id de origem valida (usa getId_inPort e validIdOrig)
bool Circuito::validPort(int IdPort) const
{
  try
  {
    if (!definedPort(IdPort)) throw -1;
    for (int j=0; j<getNumInputsPort(IdPort); ++j)
    {
      if (!validIdOrig(getId_inPort(IdPort,j))) throw j;
    }
  }
  catch (int erro)
  {
    // Mensagem de erro para debug. Remova na versao final,
    // especialmente se estiver utilizando interface com programacao visual
   // std::cerr << "ERRO Circuito::validPort - porta (Id=" << IdPort
              //<< ") invalida - erro tipo " << erro << std::endl;

    return false;
  }
  return true;
}

// Retorna true se o circuito eh valido (estah com todos os dados corretos):
// - numero de entradas, saidas e portas valido (> 0)
// - todas as portas validas (usa validPort)
// - todas as saidas com Id de origem validas (usa getIdOutput e validIdOrig)
// Essa funcao deve ser usada antes de salvar ou simular um circuito
bool Circuito::valid() const
{
  try
  {
    if (getNumInputs()<=0) throw 1;
    if (getNumOutputs()<=0) throw 2;
    if (getNumPorts()<=0) throw 3;
    for (int i=0; i<getNumPorts(); ++i)
    {
      if (!validPort(i+1)) throw 4;
    }
    for (int i=0; i<getNumOutputs(); ++i)
    {
      if (!validIdOrig(getIdOutput(i+1))) throw 5;
    }
  }
  catch (int erro)
  {
    // Mensagem de erro para debug. Remova na versao final,
    // especialmente se estiver utilizando interface com programacao visual
   // std::cerr << "ERRO Circuito::valid - tipo " << erro << std::endl;

    return false;
  }
  return true;
}

/// ***********************
/// Funcoes de consulta
/// ***********************

// Caracteristicas do circuito

int Circuito::getNumInputs() const
{
  return Nin;
}

int Circuito::getNumOutputs() const
{
  return int(id_out.size());
}

int Circuito::getNumPorts() const
{
  return int(ports.size());
}

// Caracteristicas das saidas do circuito

// Retorna a origem (a id) do sinal de saida cuja id eh IdOutput
// Depois de testar o parametro (validIdOutput), retorna id_out.at(IdOutput-1)
// ou 0 se parametro invalido
int Circuito::getIdOutput(int IdOutput) const
{
  if (validIdOutput(IdOutput)) return id_out.at(IdOutput-1);
  return 0;
}

// Retorna o valor logico atual da saida cuja id eh IdOutput
// Depois de testar o parametro (validIdOutput), retorna out_circ.at(IdOutput-1)
// ou bool3S::UNDEF se parametro invalido
bool3S Circuito::getOutput(int IdOutput) const
{
  if (validIdOutput(IdOutput)) return out_circ.at(IdOutput-1);
  return bool3S::UNDEF;
}

// Caracteristicas das portas

// Retorna o nome da porta: AN, NX, etc
// Depois de testar se a porta existe (definedPort),
// retorna ports.at(IdPort-1)->getName()
// ou "??" se parametro invalido
std::string Circuito::getNamePort(int IdPort) const
{
  if (definedPort(IdPort))
  {
    return ports.at(IdPort-1)->getName();
  }
  return "??";
}

// Retorna o numero de entradas da porta
// Depois de testar se a porta existe (definedPort),
// retorna ports.at(IdPort-1)->getNumInputs()
// ou 0 se parametro invalido
int Circuito::getNumInputsPort(int IdPort) const
{
  if (definedPort(IdPort))
  {
    return ports.at(IdPort-1)->getNumInputs();
  }
  return 0;
}

// Retorna a origem (a id) da I-esima entrada da porta cuja id eh IdPort
// Depois de testar se a porta IdPort existe (definedPort)
// e se o indice I eh valido (Port::validIndex),
// retorna ports.at(IdPort-1)->getId_in(I) ou 0 se parametro invalido
// Nao eh obrigatorio testar I nessa funcao pois getId_in jah testa.
int Circuito::getId_inPort(int IdPort, int I) const
{
  if (definedPort(IdPort))
  {
    // getId_in jah testa o parametro I; nao repito aqui
    return ports.at(IdPort-1)->getId_in(I);
  }
  return 0;
}

/// ***********************
/// Funcoes de modificacao
/// ***********************

// Caracteristicas das saidas

// Altera a origem da saida de id "IdOut", que passa a ser "IdOrig"
// Depois de testar os parametros (validIdOutput,validIdOrig),
// faz: id_out.at(IdOut-1) = IdOrig
void Circuito::setIdOutput(int IdOut, int IdOrig)
{
  if (validIdOutput(IdOut) && validIdOrig(IdOrig))
  {
    id_out.at(IdOut-1) = IdOrig;
  }
}

// Caracteristicas das ports

// A porta cuja id eh IdPort passa a ser do tipo Tipo (NT, AN, etc.), com NIn entradas
// Depois de varios testes:
// 1) validIdPort(IdPort)
// 2) validType(Tipo)
// 3) (Tipo=="NT" && NIn==1) || (Tipo!="NT" && NIn>=2)
// faz:
// 1) Libera a antiga area de memoria: delete ports.at(IdPort-1)
// 2) Cria a nova porta: ports.at(IdPort-1) = allocPort(Tipo)
// 3) Fixa o numero de entrada: ports.at(IdPort-1)->setNumInputs(NIn)
void Circuito::setPort(int IdPort, std::string Tipo, int NIn)
{
  // Chegagem dos parametros
  if (!validIdPort(IdPort)) return;
  if (!validType(Tipo)) return;
  if ( (Tipo=="NT" && NIn==1) || (Tipo!="NT" && NIn>=2) )
  {
      delete ports.at(IdPort-1);
      ports.at(IdPort-1) = allocPort(Tipo);
      ports.at(IdPort-1)->setNumInputs(NIn);
  }
  else
  {
    // Mensagem de erro para debug. Remova na versao final,
    // especialmente se estiver utilizando interface com programacao visual
   // std::cerr << "ERRO Circuito::setPort - tipo ("
              //<< Tipo << ") e numero de entradas ("
             // << NIn << ") incompativeis para porta Id="
             // << IdPort << std::endl;
  }
}

// Altera a origem da I-esima entrada da porta cuja id eh IdPort, que passa a ser "IdOrig"
// Depois de VARIOS testes (definedPort, validIndex, validIdOrig),
// faz: ports.at(IdPort-1)->setId_in(I,Idorig)
void Circuito::setId_inPort(int IdPort, int I, int IdOrig) const
{
  // Chegagem dos parametros
  if (!definedPort(IdPort)) return;
  if (!ports.at(IdPort-1)->validIndex(I)) return;
  if (!validIdOrig(IdOrig)) return;
  // Fixa a origem da entrada
  ports.at(IdPort-1)->setId_in(I,IdOrig);
}

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
void Circuito::digitar(void)
{
  int NI, NO, NP;
  std::string tipo;
  int i;

  // Lendo as dimensoes do circuito
  std::cout << "CIRCUITO:\n";
  do
  {
    std::cout << "Numero de entradas do circuito (>0): ";
    std::cin >> NI;
  }
  while (NI<=0);
  do
  {
    std::cout << "Numero de saidas do circuito (>0): ";
    std::cin >> NO;
  }
  while (NO<=0);
  do
  {
    std::cout << "Numero de ports do circuito (>0): ";
    std::cin >> NP;
  }
  while (NP<=0);
  // Redimensionando o circuito
  resize(NI, NO, NP);

  std::cout << "PORTAS:\n";
  for (i=0; i<getNumPorts(); ++i)
  {
    // Lendo o tipo de uma porta
    std::cout << "Definindo a porta id=" << i+1 << std::endl;
    do
    {
      std::cout << "  Tipo da porta [NT,AN,NA,OR,NO,XO,NX]: ";
      std::cin >> tipo;
    }
    while (!validType(tipo));

    if(ports.at(i) != nullptr)delete ports.at(i);

    ports.at(i) = allocPort(tipo);

    do{
        ports.at(i) -> digitar();
    }
    while(!validPort(i + 1));

  }

  std::cout << "SAIDAS:\n";
  for (i=0; i<getNumOutputs(); ++i){
    int idOutput;

    std::cout << "Digite a id da saida " << i+1<< ": "; //semelhante ao Not, ao analisar
    std::cin >> idOutput;

    while(!validIdOrig(idOutput)){
        std::cout << "Erro - Id invalido, digite novamente outra Id para a saida " << i+1 << ": ";
        std::cin >> idOutput;
    }
    id_out.at(i) = (idOutput);
  }
}

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
bool Circuito::ler(const std::string& arq)
{
  std::ifstream myfile(arq);
  try
  {
    std::string pS;
    int NI,NO,NP;
    char c;
    std::string tipo;
    int id,i,idOrig;;

    if (!myfile.is_open()) throw 1;
    // Lendo as dimensoes do circuito
    myfile >> pS >> NI >> NO >> NP;
    if (!myfile.good() || pS!="CIRCUITO" ||
        NI<=0 || NO<=0 || NP<=0) throw 2;
    // Redimensionando o circuito
    resize(NI, NO, NP);

    // Lendo as portas do circuito
    myfile >> pS;
    if (!myfile.good() || pS!="PORTAS") throw 3;
    for (i=0; i<getNumPorts(); ++i)
    {
      // Lendo o tipo de uma porta
      myfile >> id >> c >> tipo;
      if (!myfile.good() || id != i+1 || c!=')' || !validType(tipo)) throw 4;

      ports.at(i) = allocPort(tipo);
      if((!ports.at(i)->ler(myfile)) || (!validPort(i + 1)))throw 5;
    }

    // Lendo as id de origem das saidas do circuito
    myfile >> pS;
    if (!myfile.good() || pS!="SAIDAS") throw 6;
    for (i=0; i<getNumOutputs(); ++i)
    {
        myfile >> id >> c >> idOrig;
        if (!myfile.good() || id != i + 1 || c != ')'|| !validIdOrig(idOrig)) throw 7;
        setIdOutput(i + 1, idOrig);
    }
  }
  catch (int erro)
  {
    // Mensagem de erro para debug. Remova na versao final,
    // especialmente se estiver utilizando interface com programacao visual
   // std::cerr << "ERRO Circuito::ler - arquivo (" << arq
             // << ") invalido - erro tipo " << erro << std::endl;
    if (myfile.is_open()) myfile.close();
    clear();
    return false;
  }
  myfile.close();
  return true;
}

// Saida dos dados de um circuito (em tela ou arquivo, a mesma funcao serve para os dois)
// Imprime os cabecalhos e os dados do circuito, caso o circuito seja valido
// Deve utilizar os metodos de impressao da classe Port
// Retorna uma referencia aa mesma ostream que recebeu como parametro
std::ostream& Circuito::imprimir(std::ostream& O) const
{
  if (!valid()) return O;
  O << "CIRCUITO " << getNumInputs() << " " << getNumOutputs() << " " << getNumPorts() << std::endl;
  O << "PORTAS\n";
  for (int I = 0; I < getNumPorts(); ++I){
        O << I + 1 << ") " << getNamePort(I + 1) << " " << getNumInputsPort(I + 1) << ": ";
        for (int j = 0; j < getNumInputsPort(I + 1); ++j){
                O << getId_inPort(I + 1, j);
                if (j < getNumInputsPort(I + 1) - 1) O << " ";
        }
        O << std::endl;
  }
  O << "SAIDAS\n";
  for (int i = 0; i < getNumOutputs(); ++i) O << i + 1 << ") " << getIdOutput(i+1) << "\n";
  return O;
}

// Operador de impressao da classe Circuito
// Utiliza o metodo imprimir
// Retorna uma referencia aa mesma ostream que recebeu como parametro
std::ostream& operator<<(std::ostream& O, const Circuito& C)
{
  return C.imprimir(O);
}

// Salvar circuito em arquivo, caso o circuito seja valido
// Abre a stream, chama o metodo imprimir e depois fecha a stream
// Retorna true se deu tudo OK; false se deu erro
bool Circuito::salvar(const std::string& arq) const
{
  if (!valid()) return false;

  std::ofstream myfile(arq);
  if (!myfile.is_open()) return false;
  imprimir(myfile);
  myfile.close();
  return true;
}

/// ***********************
/// SIMULACAO (funcao principal do circuito)
/// ***********************

// Calcula a saida das ports do circuito para os valores de entrada
// passados como parametro, caso o circuito e a dimensao da entrada sejam validos
// (valid, in_circ.size==getNumInputs). Caso contrario retorna false.
// A entrada eh um vetor de bool3S, com dimensao igual ao numero de entradas do circuito.
// Depois de simular todas as portas do circuito, calcula as saidas do circuito (out_circ)
// Retorna true se a simulacao foi OK; false caso deh erro
bool Circuito::simular(const std::vector<bool3S>& in_circ)
{
    if (!valid() || int(in_circ.size())!=getNumInputs())
    {
    // Mensagem de erro para debug. Remova na versao final,
    // especialmente se estiver utilizando interface com programacao visual
       // std::cerr << "ERRO Circuito::simular - circuito (" << getNumInputs()
                 // << "entradas) ou vetor de entradas (tamanho " << in_circ.size()
                 // << ") invalido" << std::endl;

        return false;
    }
    // Inicializacao das portas
    for (int i = 0; i < getNumPorts(); ++i){
        ports[i]->setOutput(bool3S::UNDEF);
    }
    bool tudo_def, alguma_def;
    int id;

    do
    {
        tudo_def = true;
        alguma_def = false;

        for (int i = 0; i < getNumPorts(); ++i){
            if (ports[i]->getOutput() == bool3S::UNDEF){

                // Ajusta o tamanho de in_port igual ao n�mero de entradas da porta, evitando "resize"
                std::vector<bool3S> in_port(ports[i]->getNumInputs());
                for (int j = 0; j < ports[i]->getNumInputs(); ++j)
                {
                    id = ports[i]->getId_in(j);
                    // Obt�m valor da entrada
                    if (id > 0){
                        in_port[j] = ports[id - 1]->getOutput();
                    }else{
                        in_port[j] = in_circ[-id - 1];
                    }
                }
                // Simula a porta
                ports[i]->simular(in_port);
                if (ports[i]->getOutput() == bool3S::UNDEF){
                    tudo_def = false;
                }else{
                    alguma_def = true;
                }
            }
        }

    } while (!tudo_def && alguma_def);

    // Determinacao das saidas
    for (int j = 0; j < getNumOutputs(); ++j){
        //como faz uso de getIdOutput, foi necessario alterar para j+1 (para manter o for int j = 0...),
        //tal como utilizado em funcoes pre-implementadas, pois o getIdOutput retorna idOut - 1
        id = getIdOutput(j+1);
        // Obtem valor da saida
        if (id > 0){
            out_circ[j] = ports[id - 1]->getOutput();
        }else{
            out_circ[j] = in_circ[-id - 1];
        }
    }
  return true;
}




