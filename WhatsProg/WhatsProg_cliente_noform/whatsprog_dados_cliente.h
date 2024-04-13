#ifndef WHATSPROG_DADOS_CLIENTE_H
#define WHATSPROG_DADOS_CLIENTE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
//#include <stdint.h>
#include <mutex>
#include "whatsprog_dados.h"
#include "../MySocket/mysocket.h"

// Classe utilizada para armazenar uma conversa no cliente
class Conversa
{
private:
  /// Correspondente
  /// Deve ser o remetente ou destinatario de todas as mensagens da conversa.
  std::string correspondente;

  /// Todas as mensagens da conversa
  std::vector<Mensagem> LM;

private:
  /// Funcoes de alteracao do correspondente
  /// Retorna false em caso de erro, true se OK
  bool setCorrespondente(const std::string& U);

  /// Apaga todas as msgs da conversa
  void clearMessages();

  /// Insere nova msg no final da conversa
  void pushMessage(const Mensagem& M);

  /// Remove ultima msg (do final da conversa)
  void popMessage();

  /// Remove a i-esima msg
  void eraseMessage(int i);

  /// Ler/salvar em arquivo
  /// Recebe como entrada uma fstream jah aberta
  /// A fstream deve ser aberta pela funcao de ler/salvar de um nivel superior
  /// (da classe DadosCliente)
  bool ler(std::ifstream& arq);
  bool salvar(std::ofstream& arq) const;

public:
  /// Construtor default
  Conversa();

  /// Retorna o numero de mensagens da conversa
  int size() const;

  /// Funcao de consulta do correspondente
  const std::string& getCorrespondente() const;

  /// Funcao de consulta da i-esima mensagem da conversa (nao alteravel)
  const Mensagem& at(int i) const;

  /// Teste de igualdade entre uma conversa e uma std::string
  /// Utilizado pelos algoritmos STL: find, count, etc
  bool operator==(const std::string& C) const;

  /// Procura na conversa uma mensagem que tenha a id passada
  /// como parametro e que tenha o usuario como remetente.
  /// Retorna o indice da mensagem dentro dessa conversa.
  /// Caso nao exista, retorna um valor negativo.
  int findMensagem(int32_t id) const;

  friend class WhatsProgDadosCliente;
};

typedef std::list<Conversa> listConversa;
typedef listConversa::iterator iterConversa;

class WhatsProgDadosCliente
{
private:
  /// O computador (IP) onde roda o programa servidor
  std::string servidor;

  /// O nome do usuario do cliente
  std::string meuUsuario;

  /// O iterador para a conversa atualmente sendo visualizada
  /// Se end(), nenhuma conversa estah selecionada
  iterConversa convAtual;

  /// A ultima ID utilizada em msg enviada por mim
  /// Maior que zero; se igual a 0, nenhuma msg foi enviada ainda
  uint32_t idMensagem;

  /// Todas as conversas
  listConversa LC;

  /// A variavel de exclusao mutua, que protege os dados do cliente de
  /// serem alterados simultaneamente por duas threads
  std::mutex mtx;
public:
  /// Construtor default
  WhatsProgDadosCliente();

  /// Destrutor
  ~WhatsProgDadosCliente();

  /// Iteradores de inicio e fim (depois do ultimo) da lista de conversa
  iterConversa begin();
  iterConversa end();

  /// Iterador para a ultima conversa da lista de conversas
  iterConversa last();

  /// Retorna o numero de conversas
  int size() const;

  /// Testa se o servidor estah com os dados de conexao preenchidos
  bool connected() const;

  /// Funcoes de consulta dos dados principais (servidor, login do usuario)
  const std::string& getServidor() const;
  const std::string& getMeuUsuario() const;

  /// Define os parametros principais da conexao: servidor e usuario (login)
  /// Retorna false em caso de erro, true se OK
  bool setServidorUsuario(const std::string& IP, const std::string& MU);

  /// Limpa o servidor e o nome do usuario (deslogar)
  void unsetServidorUsuario();

  /// Gera (incrementa) uma id valida para a proxima msg
  uint32_t getNovaIdMensagem();

  /// Funcao de consulta da conversa atual
  iterConversa getConversaAtual() const;

  /// Funcao de alteracao da conversa atual
  /// Recebe como parametro o nome do correspondente da conversa
  /// Retorna false em caso de erro, true se OK
  bool setConversaAtual(const std::string& U);

  /// Funcao de alteracao da conversa atual
  /// Recebe como parametro o iterador para a conversa
  /// Retorna false em caso de erro, true se OK
  bool setConversaAtual(iterConversa it);

  /// Limpa conversa atual
  void unsetConversaAtual();

  /// Inclui (cria) uma nova conversa com o correspondente passado como parametro
  /// Se for a unica conversa, torna-a a conversa ativa
  /// Retorna false em caso de erro, true se OK
  bool insertConversa(const std::string& U);

  /// Retorna o iterador para a conversa cujo correspondente seja igual ao parametro
  /// Retorna end() caso nao exista a conversa
  iterConversa findConversa(const std::string& U);

  /// Move uma conversa para a primeira posicao da lista (begin)
  bool moveConversaToBegin(iterConversa it);

  /// Exclui a conversa
  /// Retorna false em caso de erro, true se OK
  bool eraseConversa(iterConversa it);

  /// Apaga todas as conversas
  void clearConversas();

  /// Procura em todas as conversa uma mensagem que tenha a id igual ao parametro
  /// e que tenha o correspondente da conversa como destinatario
  /// (ou seja, tenha o usuario como remetente)
  /// Retorna o iterator para a conversa onde estah a mensagem que tem essa id
  /// e o indice da mensagem dentro dessa conversa
  /// Caso nao exista em nenhuma conversa uma msg com essa id,
  /// retorna o iterator igual a end() e o indice negativo
  void findMensagem(int32_t id, iterConversa& it, int& ind_msg);

  /// Altera o status da i-esima mensagem da conversa it
  void setStatus(iterConversa it, int i, MsgStatus S);

  /// Insere nova msg M no final da conversa it
  void pushMessage(iterConversa it, const Mensagem& M);

  /// Remove ultima msg da conversa it
  void popMessage(iterConversa it);

  /// Apaga todas as msgs da conversa
  void clearMessages(iterConversa it);

  /// Apaga a i-esima mensagem da conversa it
  void eraseMessage(iterConversa it, int i);

  /// Retorna o nome do arquivo para ler/salvar os dados
  std::string getNomeArq() const;

  /// Ler/salvar em arquivo
  /// O nome do arquivo eh padronizado (ver getNomeArq)
  bool ler();
  bool salvar();
};

#endif // WHATSPROG_DADOS_CLIENTE_H
