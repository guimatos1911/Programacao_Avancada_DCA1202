#ifndef WHATSPROG_DADOS_SERVIDOR_H
#define WHATSPROG_DADOS_SERVIDOR_H

#include <string>
#include <list>
#include "../MySocket/mysocket.h"
#include "whatsprog_dados.h"

/// Os usuarios cadastrados no servidor
class Usuario
{
private:
  // Identificacao do usuario
  std::string login;
  std::string senha;

  // Socket de comunicacao (idle se nao conectado)
  tcp_mysocket s;

  // Ultima id de mensagem recebida
  int32_t last_id;

public:
  // Construtor default
  Usuario();

  // Funcao de consulta ao valor para login
  const std::string& getLogin() const;

  // Funcao de fixacao de valor para login e senha
  // Retorna true em caso de sucesso, false se erro
  bool setUsuario(const std::string& L, const std::string& S);

  // Valida uma senha, comparando com a senha armazenada
  bool validarSenha(const std::string& S) const;

  // Funcao de acesso ao socket do usuario
  const tcp_mysocket& getSocket() const;

  // Alteracao do socket de um usuario
  void swapSocket(tcp_mysocket& S);

  // Consulta do estado do socket
  bool connected() const;
  bool closed() const;

  // Funcoes de envio de dados via socket
  mysocket_status read_int(int32_t& num, long milisec=-1) const;
  mysocket_status write_int(int32_t num) const;
  mysocket_status read_string(std::string& msg, long milisec=-1) const;
  mysocket_status write_string(const std::string& msg) const;

  // Fechamento do socket
  void close();

  // Consulta da ultima ID do usuario
  int32_t getLastId() const;

  // Alteracao da ultima ID do usuario
  bool setLastId(int32_t ID);

  // Reinicializa a ID do usuario
  void resetId();

  // Teste de igualdade com uma string (testa se a string eh igual ao login)
  bool operator==(const std::string& L) const;

  // Ler/salvar em arquivo
  // Recebe como entrada uma fstream jah aberta
  // A fstream deve ser aberta pela funcao de ler/salvar de um nivel superior
  bool ler(std::ifstream& arq);
  bool salvar(std::ofstream& arq) const;
};

typedef std::list<Mensagem> listMensagem;
typedef listMensagem::iterator iterMensagem;

typedef std::list<Usuario> listUsuario;
typedef listUsuario::iterator iterUsuario;

class WhatsProgDadosServidor
{
private:
  // O socket de conexoes
  tcp_mysocket_server c;

  // Lista de todos os usuarios (conectados ou nao)
  listUsuario LU;

  // Buffer de mensagens pendentes no servidor
  listMensagem LM;

  //
  // FUNCOES AUXILIARES PARA EVITAR DUPLICACAO DE CODIGO
  //

  // Envia uma mensagem iMsg que esteja no buffer com status MSG_RECEBIDA
  // e cujo destinatario iDest esteja conectado (caso de uso S.3)
  // Apos o envio, altera o status da msg enviada para MSG_ENTREGUE
  void enviarMensagem(iterMensagem iMsg, iterUsuario iDest);

  // Envia uma confirmacao de entrega da mensagem iMsg
  // que esteja no buffer com status MSG_ENTREGUE
  // e cujo remetente iRemet esteja conectado (caso de uso S.4)
  // Apos o envio da confirmacao, remove a msg do buffer
  void enviarConfirmacao(iterMensagem iMsg, iterUsuario iRemet);

public:
  // Funcoes de consulta aos dados basicos
  int getNumUsuarios() const;
  int getNumMensagens() const;

  // Funcoes de acesso aas funcionalidades basicas dos sockets
  mysocket_status listen(const char *port, int nconex=1);

  // Fecha os sockets de conexao e de todos os usuarios conectados
  void closeSockets();

  // Retorna o nome do arquivo para ler/salvar os dados
  std::string getNomeArq() const;

  // Ler/salvar em arquivo
  // O nome do arquivo eh padronizado (ver getNomeArq)
  bool ler();
  bool salvar() const;

  // Thread que efetivamente desempenha as tarefas do servidor
  int main_thread();
};

#endif // WHATSPROG_DADOS_SERVIDOR_H
