#ifndef WHATSPROG_DADOS_H
#define WHATSPROG_DADOS_H

#include <iostream>
#include <fstream>
#include <string>
//#include <stdint.h>
#include <list>

// A porta de comunicacao cliente-servidor
#define PORTA_WHATSPROG "23456"

// Timeout (em segundos) para esperar o envio de um dado obrigatorio pelo socket
#define TIMEOUT_WHATSPROG 15
// Timeout (em segundos)  para o cliente enviar o nome de login apos conexao
#define TIMEOUT_LOGIN_WHATSPROG 60

// Numero de bytes das informacoes a serem enviadas/recebidas
#define TAM_MIN_NOME_USUARIO 6
#define TAM_MAX_NOME_USUARIO 12
#define TAM_MIN_SENHA 6
#define TAM_MAX_SENHA 12
#define TAM_MAX_MSG 255

/// Os possiveis estados de uma mensagem
enum class MsgStatus
{
  MSG_ENVIADA,
  MSG_RECEBIDA,
  MSG_ENTREGUE,
  MSG_INVALIDA
};

/// Os tipos de comando
/// Declarados como um tipo enum normal (e nao enum class) porque
/// os comandos precisam ser tratados como inteiros em algumas situacoes
enum ComandoWhatsProg
{
  CMD_NEW_USER=1001,
  CMD_LOGIN_USER=1002,
  CMD_LOGIN_OK=1003,
  CMD_LOGIN_INVALIDO=1004,
  CMD_NOVA_MSG=1005,
  CMD_MSG_INVALIDA=1006,
  CMD_MSG_RECEBIDA=1007,
  CMD_MSG_ENTREGUE=1008,
  CMD_LOGOUT_USER=1009
};

/// Testa um possivel login: tamanho e soh contem caracteres de 33 (!) a 126 (~)
bool testarNomeUsuario(const std::string& S);

/// Testa uma possivel senha: tamanho e soh contem caracteres de 33 (!) a 126 (~)
bool testarSenha(const std::string& S);

/// Testa o texto de uma mensagem: tamanho
bool testarTextoMensagem(const std::string& S);

/// Testa um possivel status (todos os valores possiveis)
bool testarStatus(MsgStatus S);

/// Retorna uma string com o nome do comando
std::string nome_cmd(ComandoWhatsProg cmd);

/// Retorna uma string com o nome de um status
std::string nome_status(MsgStatus S);

/// Alguns tipos auxiliares que sao utilizados pelos algoritmos STL:
/// find, count, etc
/// Sao definidos diferentes tipos para que se possa fazer diferentes
/// buscas em uma lista de mensagens, sobrecarregando de formas diferentes
/// o operador == para cada um desses tipos

/// Buscar (com find) mensagens com um dado remetente e status
struct remetStatus
{
  std::string remetente;
  MsgStatus status;
  remetStatus(const std::string& R, MsgStatus S);
};
/// Buscar (com find) mensagens com um dado remetente e id
struct remetId
{
  std::string remetente;
  int32_t id;
  remetId(const std::string& R, int32_t I);
};
/// Buscar (com find) mensagens com um dado destinatario e status
struct destStatus
{
  std::string destinatario;
  MsgStatus status;
  destStatus(const std::string& D, MsgStatus S);
};
/// Buscar (com find) mensagens com um dado destinatario e id
struct destId
{
  std::string destinatario;
  int32_t id;
  destId(const std::string& D, int32_t I);
};

/// As mensagens armazenadas no cliente e no servidor
class Mensagem
{
private:

  /// Identificador da mensagem
  int32_t id;

  /// Estado (status) da mensagem
  MsgStatus status;

  /// Nomes do remetente e do destinatario
  std::string remetente;
  std::string destinatario;

  /// Texto da mensagem
  std::string texto;

public:

  /// Construtor default
  Mensagem();

  /// Funcoes de consulta/alteracao de parametros
  /// As funcoes de alteracao retornam false em caso de erro, true se OK

  int32_t getId() const;
  bool setId(int32_t I);

  const std::string& getRemetente() const;
  bool setRemetente(const std::string& R);

  const std::string& getDestinatario() const;
  bool setDestinatario(const std::string& D);

  const std::string& getTexto() const;
  bool setTexto(const std::string& T);

  MsgStatus getStatus() const;
  bool setStatus(MsgStatus S);

  /// Os testes de igualdade entre uma msg e os tipos auxiliares
  /// que sao utilizados pelos algoritmos STL: find, count, etc

  /// Buscar (com find) mensagens com um dado remetente e status
  bool operator==(const remetStatus& T) const;
  /// Buscar (com find) mensagens com um dado remetente e id
  bool operator==(const remetId& T) const;
  /// Buscar (com find) mensagens com um dado destinatario e status
  bool operator==(const destStatus& T) const;
  /// Buscar (com find) mensagens com um dado destinatario e id
  bool operator==(const destId& T) const;

  /// Ler/salvar em arquivo
  /// Recebe como entrada uma fstream jah aberta
  /// A fstream deve ser aberta pela funcao de ler/salvar de um nivel superior
  /// (da classe WhatsProgDadosCliente)
  bool ler(std::ifstream& arq);
  bool salvar(std::ofstream& arq) const;
};

#endif // WHATSPROG_DADOS_H
