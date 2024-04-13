#include "whatsprog_dados_cliente.h"
#include <algorithm>

using namespace std;

/// Construtor default
Conversa::Conversa(): correspondente(""), LM() {}

/// Retorna o numero de mensagens da conversa
int Conversa::size() const
{
  return LM.size();
}

/// Funcao de consulta do correspondente
const string& Conversa::getCorrespondente() const
{
  return correspondente;
}

bool Conversa::setCorrespondente(const string& U)
{
  if (!testarNomeUsuario(U)) return false;
  correspondente = U;
  return true;
}

/// Apaga todas as msgs da conversa
void Conversa::clearMessages()
{
  LM.clear();
}

/// Insere nova msg no final da conversa
void Conversa::pushMessage(const Mensagem& M)
{
  LM.push_back(M);
}

/// Remove ultima msg (do final da conversa)
void Conversa::popMessage()
{
  LM.pop_back();
}

/// Retorna a i-esima mensagem da conversa
const Mensagem& Conversa::at(int i) const
{
  static Mensagem vazia;
  if (i>=0 && i<size()) return LM[i];
  /* else */return vazia;
}

/// Teste de igualdade entre uma conversa e uma string
/// Utilizado pelos algoritmos STL: find, count, etc
bool Conversa::operator==(const string& C) const
{
  return correspondente==C;
}

/// Remove a i-esima msg
void Conversa::eraseMessage(int i)
{
  if (i>=0 && i<size())
  {
    LM.erase(LM.begin()+i);
  }
}

/// Procura na conversa uma mensagem que tenha a id igual ao parametro
/// e que tenha o correspondente da conversa como destinatario
/// (ou seja, tenha o usuario como remetente)
/// Retorna o indice da mensagem dentro da conversa.
/// Caso nao exista, retorna um valor negativo.
int Conversa::findMensagem(int32_t id) const
{
  vector<Mensagem>::const_iterator iM;
  iM = find(LM.begin(), LM.end(), destId(correspondente, id));
  if (iM != LM.end())
  {
    // Achou a msg na conversa
    return (iM-LM.begin());
  }
  /* else */return -1;
}

/// Ler de arquivo
/// Recebe como entrada uma ifstream jah aberta
/// A ifstream deve ser aberta pela funcao de ler de um nivel superior
/// (da classe DadosCliente)
bool Conversa::ler(ifstream& arq)
{
  try
  {
    if (!arq.is_open()) throw 1;

    string prov_cabecalho;
    string prov_correspondente;
    int prov_size;
    arq >> prov_cabecalho >> prov_correspondente >> prov_size;
    if (prov_cabecalho != "CONVERSA" ||
        !testarNomeUsuario(prov_correspondente) ||
        prov_size<0) throw 2;

    correspondente = prov_correspondente;
    LM.resize(prov_size);
    for (int i=0; i<prov_size; ++i)
    {
      if (!LM[i].ler(arq)) throw 3;
    }
  }
  catch(int i)
  {
    correspondente = "";
    LM.clear();
    return false;
  }
  return true;
}

/// Salvar em arquivo
/// Recebe como entrada uma ofstream jah aberta
/// A ofstream deve ser aberta pela funcao de salvar de um nivel superior
/// (da classe DadosCliente)
bool Conversa::salvar(ofstream& arq) const
{
  if (!arq.is_open()) return false;
  arq << "CONVERSA " << correspondente << ' ' << LM.size() << endl;
  for (int i=0; i<int(LM.size()); ++i)
  {
    if (!LM[i].salvar(arq)) return false;
  }
  return true;
}

/// Construtor default
WhatsProgDadosCliente::WhatsProgDadosCliente():
  servidor(""),meuUsuario(""),convAtual(),idMensagem(0),LC(),mtx()
{
  // Nenhuam conversa selecionada inicialmente
  convAtual=LC.end();
}

/// Destrutor
WhatsProgDadosCliente::~WhatsProgDadosCliente() {}

/// Iteradores de inicio e fim (depois do ultimo) da lista de conversa
iterConversa WhatsProgDadosCliente::begin()
{
  return LC.begin();
}
iterConversa WhatsProgDadosCliente::end()
{
  return LC.end();
}

/// Retorna iterador para a ultima conversa da lista de conversas
iterConversa WhatsProgDadosCliente::last()
{
  iterConversa it = LC.end();
  if (LC.size() > 0) --it;
  return it;
}

/// Retorna o numero de conversas
int WhatsProgDadosCliente::size() const
{
  return LC.size();
}

/// Testa se o servidor estah com os dados de conexao preenchidos
bool WhatsProgDadosCliente::connected() const
{
  return (servidor!="" && testarNomeUsuario(meuUsuario));
}

/// Funcoes de consulta dos dados principais (servidor, login do usuario)
const string& WhatsProgDadosCliente::getServidor() const
{
  return servidor;
}
const string& WhatsProgDadosCliente::getMeuUsuario() const
{
  return meuUsuario;
}

bool WhatsProgDadosCliente::setServidorUsuario(const string& IP, const string& MU)
{
  if (IP=="" || !testarNomeUsuario(MU)) return false;

  // Entra na secao critica para alterar dados
  mtx.lock();

  // Sempre que altera a conexao, apaga todas as conversas
  // Nao pode usar clearConversas pq iria tentar dar outro lock no mutex
  // Em vez, chama LC.clear, que eh a mesma cois
  LC.clear();
  convAtual = LC.end();

  // Altera os dados
  servidor = IP;
  meuUsuario = MU;

  // Sai da secao critica
  mtx.unlock();

  return true;
}

void WhatsProgDadosCliente::unsetServidorUsuario()
{
  // Entra na secao critica para alterar dados
  mtx.lock();

  servidor = "";
  meuUsuario = "";
  // Nao pode usar clearConversas pq iria tentar dar outro lock no mutex
  // Em vez, chama LC.clear, que eh a mesma cois
  LC.clear();
  convAtual = LC.end();

  // Sai da secao critica
  mtx.unlock();
}

/// Gera (incrementa) uma id valida para a proxima msg
uint32_t WhatsProgDadosCliente::getNovaIdMensagem()
{
  // Entra na secao critica para alterar dados
  mtx.lock();

  ++idMensagem;

  // Sai da secao critica
  mtx.unlock();

  return idMensagem;
}

/// Funcao de consulta da conversa atual
iterConversa WhatsProgDadosCliente::getConversaAtual() const
{
  return convAtual;
}

/// Funcao de alteracao da conversa atual
/// Recebe como parametro o nome do correspondente da conversa
/// Retorna false em caso de erro, true se OK
bool WhatsProgDadosCliente::setConversaAtual(const string& U)
{
  if (!testarNomeUsuario(U)) return false;
  iterConversa it = find(LC.begin(), LC.end(), U);
  return setConversaAtual(it);
}

/// Funcao de alteracao da conversa atual
/// Recebe como parametro o iterador para a conversa
/// Retorna false em caso de erro, true se OK
bool WhatsProgDadosCliente::setConversaAtual(iterConversa it)
{
  if (it == LC.end()) return false;

  // Entra na secao critica para alterar dados
  mtx.lock();

  convAtual = it;

  // Sai da secao critica
  mtx.unlock();

  return true;
}

/// Limpa conversa atual
void WhatsProgDadosCliente::unsetConversaAtual()
{
  // Entra na secao critica para alterar dados
  mtx.lock();

  convAtual=LC.end();

  // Sai da secao critica
  mtx.unlock();
}

/// Inclui (cria) uma nova conversa com o correspondente passado como parametro
/// Se for a unica conversa, torna-a a conversa ativa
/// Retorna false em caso de erro, true se OK
bool WhatsProgDadosCliente::insertConversa(const string& U)
{
  // Soh pode inserir se nao existir uma conversa com mesmo correspondente
  if (findConversa(U) != LC.end()) return false;

  Conversa C;
  // Testa erro na atribuicao do nome da conversa
  if (!C.setCorrespondente(U)) return false;

  // Entra na secao critica para alterar dados
  mtx.lock();

  LC.push_back(C);
  if (LC.size() == 1) convAtual = LC.begin();

  // Sai da secao critica
  mtx.unlock();

  return true;
}

/// Retorna o iterador para uma conversa cujo correspondente seja igual ao parametro
/// Retorna end() caso nao exista a conversa
iterConversa WhatsProgDadosCliente::findConversa(const string& U)
{
  iterConversa it=find(LC.begin(), LC.end(), U);
  return it;
}

/// Move uma conversa para a primeira posicao da lista (begin)
bool WhatsProgDadosCliente::moveConversaToBegin(iterConversa it)
{
  if (LC.size()==0 || it==LC.end()) return false;

  // Testa se a conversa jah nao estah no inicio da lista
  if (it != LC.begin())
  {
    // Entra na secao critica para alterar dados
    mtx.lock();

    // Move a conversa para o inicio da lista
    LC.splice(LC.begin(), LC, it);

    // Sai da secao critica
    mtx.unlock();
  }
  return true;
}

/// Exclui a conversa
/// Retorna false em caso de erro, true se OK
bool WhatsProgDadosCliente::eraseConversa(iterConversa it)
{
  if (LC.size()==0 || it == LC.end()) return false;

  // Entra na secao critica para alterar dados
  mtx.lock();

  // Apaga a conversa
  LC.erase(it);
  // Se a conversa excluida era a conversa atual,
  // faz com que nenhuma conversa esteja selecionada
  if (it==convAtual)
  {
    convAtual = LC.end();
  }

  // Sai da secao critica
  mtx.unlock();

  return true;
}

/// Apaga todas as conversas
void WhatsProgDadosCliente::clearConversas()
{
  // Entra na secao critica para alterar dados
  mtx.lock();

  LC.clear();

  // Sai da secao critica
  mtx.unlock();
}

/// Procura em todas as conversa uma mensagem que tenha a id igual ao parametro
/// e que tenha o correspondente da conversa como destinatario
/// (ou seja, tenha o usuario como remetente)
/// Retorna o iterator para a conversa onde estah a mensagem que tem essa id
/// e o indice da mensagem dentro dessa conversa
/// Caso nao exista em nenhuma conversa uma msg com essa id,
/// retorna o iterator igual a end() e o indice negativo
void WhatsProgDadosCliente::findMensagem(int32_t id, iterConversa& it, int& ind_msg)
{
  for (it=LC.begin(); it!=LC.end(); ++it)
  {
    ind_msg = it->findMensagem(id);
    if (ind_msg >= 0)
    {
      // Achou a msg na conversa ind_conv
      return;
    }
  }
  // Nao achou nenhuma msg em nenhuma conversa com essa id
  it = LC.end();
  ind_msg = -1;
}

/// Altera o status da i-esima mensagem da conversa it
void WhatsProgDadosCliente::setStatus(iterConversa it, int i, MsgStatus S)
{
  if (LC.size()==0 || it==LC.end() || i<0 || i>=it->size() || !testarStatus(S)) return;

  // Entra na secao critica para alterar dados
  mtx.lock();

  it->LM[i].setStatus(S);

  // Sai da secao critica
  mtx.unlock();
}

/// Insere nova msg no final de uma conversa
void WhatsProgDadosCliente::pushMessage(iterConversa it, const Mensagem& M)
{
  if (LC.size()==0 || it==LC.end()) return;

  // Entra na secao critica para alterar dados
  mtx.lock();

  it->pushMessage(M);

  // Sai da secao critica
  mtx.unlock();
}

/// Remove ultima msg da conversa it
void WhatsProgDadosCliente::popMessage(iterConversa it)
{
  if (LC.size()==0 || it==LC.end()) return;

  // Entra na secao critica para alterar dados
  mtx.lock();

  it->popMessage();

  // Sai da secao critica
  mtx.unlock();
}

/// Apaga todas as msgs da conversa it
void WhatsProgDadosCliente::clearMessages(iterConversa it)
{
  if (LC.size()==0 || it==LC.end()) return;

  // Entra na secao critica para alterar dados
  mtx.lock();

  it->clearMessages();

  // Sai da secao critica
  mtx.unlock();
}

/// Apaga a i-esima mensagem da conversa it
void WhatsProgDadosCliente::eraseMessage(iterConversa it, int i)
{
  if (LC.size()==0 || it==LC.end() || i<0 || i>=it->size()) return;

  // Entra na secao critica para alterar dados
  mtx.lock();

  it->eraseMessage(i)  ;

  // Sai da secao critica
  mtx.unlock();
}

/// Retorna o nome do arquivo para ler/salvar os dados
string WhatsProgDadosCliente::getNomeArq() const
{
  if (!testarNomeUsuario(meuUsuario) || servidor.size()==0) return "";
  return "whatsdata_cliente_"+meuUsuario+"_AT_"+servidor+".wdc";
}

/// Ler de arquivo
/// O nome do arquivo eh padronizado
bool WhatsProgDadosCliente::ler()
{
  string prov_cabecalho;
  string prov_servidor;
  string prov_meuUsuario;
  int prov_idMensagem;
  int prov_size;
  ifstream arq(getNomeArq());

  try
  {
    // Soh leh se servidor e usuario estiverem definidos
    if (servidor.size()<=0 || !testarNomeUsuario(meuUsuario)) throw 1;

    // Se nao conseguir abrir o arquivo, encerra
    if (!arq.is_open()) throw 2;

    arq >> prov_cabecalho >> prov_servidor >> prov_meuUsuario
        >> prov_idMensagem >> prov_size;

    // Se os dados iniciais nao forem validos, encerra
    if (prov_cabecalho != "DADOS_CLIENTE" ||
        prov_servidor.size()==0 ||
        !testarNomeUsuario(prov_meuUsuario) ||
        prov_idMensagem<0 || prov_size<0) throw 3;
  }
  catch (int i)
  {
    return false;
  }

  // Entra na secao critica para alterar dados
  mtx.lock();

  // Atualiza os dados gerais do cliente
  servidor = prov_servidor;
  meuUsuario = prov_meuUsuario;
  idMensagem = prov_idMensagem;
  // Limpa os dados
  LC.clear();
  convAtual = LC.end();

  // Leh as conversas
  Conversa prov_C;
  bool leitura_OK = true;
  try
  {
    for (int i=0; i<prov_size; ++i)
    {
      if (!prov_C.ler(arq)) throw 1;
      LC.push_back(prov_C);
    }
  }
  catch(int i)
  {
    leitura_OK = false;
  }

  // Sai da secao critica
  mtx.unlock();

  return leitura_OK;
}

/// Salvar em arquivo
/// O nome do arquivo eh padronizado
bool WhatsProgDadosCliente::salvar()
{
  // Soh salva se servidor e usuario estiverem definidos
  if (servidor.size()<=0 || !testarNomeUsuario(meuUsuario)) return false;

  // Se nao conseguir abrir o arquivo, encerra
  ofstream arq(getNomeArq());
  if (!arq.is_open()) return false;

  arq << "DADOS_CLIENTE " << servidor << ' ' << meuUsuario << ' '
      << idMensagem << ' ' << LC.size() << endl;
  for (listConversa::const_iterator i=LC.begin(); i!=LC.end(); ++i)
  {
    if (!i->salvar(arq)) return false;
  }

  return true;
}
