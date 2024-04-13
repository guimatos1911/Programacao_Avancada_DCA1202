#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include "whatsprog_dados_servidor.h"

using namespace std;

/// O flag que indica que o software deve encerrar todas as threads
extern bool fim;

/// Funcao auxiliar que imprime um comando tratado pelo servidor
void imprimeComando(bool recebido, const string& user, ComandoWhatsProg cmd,
                    int id, const string& user2)
{
  cout << "CMD ";
  if (recebido) cout << "REC DE ";
  else cout << "ENV P/ ";
  cout << user << ": " << nome_cmd(cmd);
  if (id > 0) cout << ',' << id;
  if (user2.size() > 0) cout << ',' << user2;
  cout << endl;
}

/// Funcao auxiliar que imprime um comando enviado pelo servidor
void imprimeComandoEnviado(const string& user, ComandoWhatsProg cmd,
                           int id=-1, const string& user2="")
{
  imprimeComando(false, user, cmd, id, user2);
}

/// Funcao auxiliar que imprime um comando recebido pelo servidor
void imprimeComandoRecebido(const string& user, ComandoWhatsProg cmd,
                            int id=-1, const string& user2="")
{
  imprimeComando(true, user, cmd, id, user2);
}

/// Funcoes auxiliares para impressao
ostream& operator<<(ostream& O, const Mensagem& M)
{
  O << M.getId() << ';' << nome_status(M.getStatus()) << ';'
    << M.getRemetente() << ';' << M.getDestinatario() << ';'
    << M.getTexto();
  return O;
}

ostream& operator<<(ostream& O, const Usuario& U)
{
  O << U.getLogin() << ';' << U.getLastId();
  return O;
}

/// CLASSE USUARIO

/// Construtor default
Usuario::Usuario(): login(""), senha(""), s(), last_id(0) {}

/// Funcao de consulta ao valor para login
const string& Usuario::getLogin() const
{
  return login;
}

/// Fixa login e senha do usuario
/// Retorna true se OK; false em caso de erro
bool Usuario::setUsuario(const string& L, const string& S)
{
  if (!testarNomeUsuario(L) || !testarSenha(S))
  {
    return false;
  }
  login = L;
  senha = S;
  return true;
}

/// Valida uma senha, comparando com a senha armazenada
bool Usuario::validarSenha(const string& S) const
{
  return (S == senha);
}

/// Funcao de acesso ao socket do usuario
const tcp_mysocket& Usuario::getSocket() const
{
  return s;
}

/// Alteracao do socket de um usuario
void Usuario::swapSocket(tcp_mysocket& S)
{
  s.swap(S);
}

/// Consulta do estado do socket
bool Usuario::connected() const
{
  return s.connected();
}

bool Usuario::closed() const
{
  return s.closed();
}

/// Funcoes de envio de dados via socket
mysocket_status Usuario::read_int(int32_t& num, long milisec) const
{
  return s.read_int(num,milisec);
}

mysocket_status Usuario::write_int(int32_t num) const
{
  return s.write_int(num);
}

mysocket_status Usuario::read_string(string& msg, long milisec) const
{
  return s.read_string(msg,milisec);
}

mysocket_status Usuario::write_string(const string& msg) const
{
  return s.write_string(msg);
}

/// Fechamento do socket
void Usuario::close()
{
  s.close();
}

/// Consulta da ultima ID do usuario
int32_t Usuario::getLastId() const
{
  return last_id;
}

/// Alteracao da ultima ID do usuario
bool Usuario::setLastId(int32_t ID)
{
  if (ID<=0 || ID<=last_id)
  {
    return false;
  }
  last_id = ID;
  return true;
}

/// Reinicializa a ID do usuario
void Usuario::resetId()
{
  last_id=0;
}

/// Teste de igualdade com uma string (testa se a string eh igual ao login)
bool Usuario::operator==(const string& L) const
{
  return login==L;
}

/// Ler de arquivo
/// Recebe como entrada uma ifstream jah aberta
/// A ifstream deve ser aberta pela funcao de ler de um nivel superior
bool Usuario::ler(ifstream& arq)
{
  // Implementacao opcional
  // Falta fazer, caso deseje implementar o salvamento de dados
  // Deve ler, testar e alterar, caso sejam validos:
  // 1) login, senha e last_id do usuario
  return false;
}

/// Salvar em arquivo
/// Recebe como entrada uma ofstream jah aberta
/// A ofstream deve ser aberta pela funcao de salvar de um nivel superior
bool Usuario::salvar(ofstream& arq) const
{
  // Implementacao opcional
  // Falta fazer, caso deseje implementar o salvamento de dados
  // Deve salvar:
  // 1) login, senha e last_id do usuario
  return false;
}

/// CLASSE WHATSPROGDADOSSERVIDOR

/// Funcoes de consulta aos dados basicos
int WhatsProgDadosServidor::getNumUsuarios() const
{
  return LU.size();
}

int WhatsProgDadosServidor::getNumMensagens() const
{
  return LM.size();
}

/// Funcoes de acesso aas funcionalidades basicas dos sockets
mysocket_status WhatsProgDadosServidor::listen(const char *port, int nconex)
{
  return c.listen(port,nconex);
}

/// Fecha os sockets de conexao e de todos os usuarios conectados
void WhatsProgDadosServidor::closeSockets()
{
  c.close();
  for (iterUsuario iU=LU.begin(); iU!=LU.end(); ++iU)
  {
    if (iU->connected()) iU->close();
  }
}

/// Envia uma mensagem iMsg que esteja no buffer com status MSG_RECEBIDA
/// e cujo destinatario esteja conectado (caso de uso S.3)
/// Apos o envio, altera o status da msg enviada para MSG_ENTREGUE
/// === BEGIN FUNCIONAMENTO REAL ===
/// Em seguida, testa se o remetente estah conectado. Se estiver,
/// chama a funcao auxiliar enviarConfirmacao, que envia a confirmacao
/// de entrega e remove a mensagem do buffer.
/// === END FUNCIONAMENTO REAL ===
/// === BEGIN FUNCIONAMENTO FAKE ===
/// Em seguida, simula o envio da confirmacao de entrega e remove do buffer
/// === END FUNCIONAMENTO FAKE ===
void WhatsProgDadosServidor::enviarMensagem(iterMensagem iMsg, iterUsuario iDest)
{
  try
  {
    // Testa os parametros
    if (iMsg == LM.end() || iDest == LU.end() || !iDest->connected() ||
        iMsg->getStatus() !=  MsgStatus::MSG_RECEBIDA ||
        iMsg->getDestinatario() != iDest->getLogin()) throw 1;

    // Envia a mensagem pelo socket
    if (iDest->write_int(CMD_NOVA_MSG) != mysocket_status::SOCK_OK) throw 2;
    if (iDest->write_int(iMsg->getId()) != mysocket_status::SOCK_OK) throw 3;
    if (iDest->write_string(iMsg->getRemetente()) != mysocket_status::SOCK_OK) throw 4;
    if (iDest->write_string(iMsg->getTexto()) != mysocket_status::SOCK_OK) throw 5;

    // Mensagem entregue
    imprimeComandoEnviado(iMsg->getDestinatario(), CMD_NOVA_MSG,
                          iMsg->getId(), iMsg->getRemetente());
    iMsg->setStatus(MsgStatus::MSG_ENTREGUE);

    // Procura o usuario remetente
    // === BEGIN FUNCIONAMENTO FAKE ===
    // No servidor FAKE soh compara com os 2 nomes simulados
    // === END FUNCIONAMENTO FAKE ===
    if (iMsg->getRemetente()!="userfake1" && iMsg->getRemetente()!="userfake2") throw 6;

    // === BEGIN FUNCIONAMENTO REAL ===
    // Remetente existe. Testa se estah conectado.
    // Se sim, envia a confirmacao de entrega da mensagem.
    // Usa a funcao auxiliar enviarConfirmacao
    // === END FUNCIONAMENTO REAL ===
    // === BEGIN FUNCIONAMENTO FAKE ===
    // No servidor FAKE nao faz teste jah que usuarios simulados estao sempre "conectados"
    // nem envia a confirmacao para o remetente simulado.
    // Nao usa a funcao auxiliar e jah faz aqui mesmo a impressao do comando
    // e a eliminacao da msg do buffer

    // Confirmacao de entrega "enviada"
    imprimeComandoEnviado(iMsg->getRemetente(), CMD_MSG_ENTREGUE, iMsg->getId(), "");

    // Remove a msg do buffer
    LM.erase(iMsg);
    // === END FUNCIONAMENTO FAKE ===
  }
  catch (int erro)
  {
    if (erro>=2 && erro<=5)
    {
      // Desconecta o destinatario se houve erro no envio pelo socket
      iDest->close();
    }
    cerr << "Erro " << erro << " no envio da mensagem para destinatario "
         << iDest->getLogin() << endl;
  }
}

/// Envia uma confirmacao de entrega de mensagem iMsg
/// que esteja no buffer com status MSG_ENTREGUE
/// e cujo remetente esteja conectado (caso de uso S.4)
/// Apos o envio da confirmacao, remove a msg do buffer
void WhatsProgDadosServidor::enviarConfirmacao(iterMensagem iMsg, iterUsuario iRemet)
{
  try
  {
    // Testa os parametros
    if (iMsg == LM.end() || iRemet == LU.end() || !iRemet->connected() ||
        iMsg->getStatus() !=  MsgStatus::MSG_ENTREGUE ||
        iMsg->getRemetente() != iRemet->getLogin()) throw 1;

    // Envia a confirmacao pelo socket
    if (iRemet->write_int(CMD_MSG_ENTREGUE) != mysocket_status::SOCK_OK) throw 2;
    if (iRemet->write_int(iMsg->getId()) != mysocket_status::SOCK_OK) throw 3;

    // Confirmacao de entrega enviada
    imprimeComandoEnviado(iMsg->getRemetente(), CMD_MSG_ENTREGUE, iMsg->getId(), "");

    // Remove a msg do buffer
    LM.erase(iMsg);
  }
  catch (int erro)
  {
    if (erro>=2 && erro<=3)
    {
      // Desconecta o remetente se houve erro no envio pelo socket
      iRemet->close();
    }
    cerr << "Erro " << erro << " no envio de confirmacao de entrega para remetente "
         << iRemet->getLogin() << endl;
  }
}

/// === BEGIN FUNCIONAMENTO FAKE ===
/// Funcao auxiliar para testar se um usuario estah conectado ou nao.
/// A ser utilizada, junto com count_if, para contar o numero de usuarios conectados.
/// Soh tem utilidade no servidor fake, pois soh pode haver um usuario conectado.
bool usuario_conectado(const Usuario& U)
{
  return U.connected();
}
/// === END FUNCIONAMENTO FAKE ===

/// Funcao que efetivamente desempenha as tarefas do servidor
int WhatsProgDadosServidor::main_thread()
{
  // A fila para select (esperar dados em varios sockets)
  mysocket_queue f;

  // O status de retorno das funcoes do socket
  mysocket_status iResult;
  // O comando recebido/enviado
  int32_t cmd;
  // Eventuais parametros de comandos lidos do socket
  int32_t id;
  string destinatario;
  string texto;
  // Mensagem recebida/enviada
  Mensagem M;
  // Variaveis auxiliares
  iterMensagem iMsg;
  iterUsuario iU,iU2;

  // === BEGIN FUNCIONAMENTO FAKE ===
  // Array para armazenar a ultima id das msgs "enviadas" pelos 2 usuarios fake.
  // Nao existe no servidor real, jah que cada usuario real tem sua last_id.
  // Cria jah inicializando cada elemento do array com valor zero.
  int32_t last_id[2] = {0,0};
  // === END FUNCIONAMENTO FAKE ===

  while (!fim)
  {
    try // Erros graves: catch encerra o servidor
    {
      // Se socket de conexoes nao estah aceitando conexoes, encerra o servidor
      if (!c.accepting()) throw 1; // Erro grave: encerra o servidor

      // Inclui na fila de sockets para o select todos os sockets que eu
      // quero monitorar para ver se houve chegada de dados
      f.clear();
      // Inclui o socket de conexoes
      f.include(c);
      // Inclui o socket de todos os clientes conectados
      for (iU=LU.begin(); iU!=LU.end(); ++iU)
      {
        if (iU->connected())
        {
          f.include(iU->getSocket());
        }
      }

      // Espera que chegue algum dado em qualquer dos sockets da fila
      iResult = f.wait_read(TIMEOUT_WHATSPROG*1000);

      switch (iResult) // resultado do wait_read
      {
      case mysocket_status::SOCK_ERROR: // resultado do wait_read
      default:
        // Erro no select
        throw 2; // Erro grave: encerra o servidor
        break;
      case mysocket_status::SOCK_TIMEOUT: // resultado do wait_read
        // Saiu por timeout: nao houve atividade em nenhum socket da fila
        // === BEGIN FUNCIONAMENTO REAL ===
        // No servidor real, aproveita para salvar o arquivo de dados (opcional):
        // if (!salvar())
        // {
        //   cerr << "Erro no salvamento do arquivo " << getNomeArq() << endl;
        // }
        // break;
        // === END FUNCIONAMENTO REAL ===
        // === BEGIN FUNCIONAMENTO FAKE ===
        // NESTE SERVIDOR FAKE, VAMOS UTILIZAR O TIMEOUT PARA SIMULAR O COMPORTAMENTO DOS
        // USUARIOS FAKE: userfake1 e userfake2
        //
        // Simula o eventual envio de msgs dos fakes para algum dos usuarios (conectado ou nao)
        // Esta parte simula muito simplificadamente o que deveria acontecer no servidor
        // real apos receber um comando CMD_NEW_MSG de algum outro usuario
        //
        // Tem um usuario definido (mesmo que nao esteja conectado)?
        // Senao, nao envia mensagens simuladas
        if (LU.size() > 0)
        {
          // O remetente
          string remetente = "userfake?";
          // O destinatario
          int N_dest;
          // Variavel auxiliar
          int j;
          for (int i=0; i<2; ++i) // Simula os 2 usuarios fake
          {
            // Soh escreve uma mensagem nova a cada 4 iteracoes em media
            if (rand()%4==0)
            {
              // Fixa o remetente
              remetente[8] = char('1'+i);  // userfake1 ou userfake2
              ++last_id[i]; // Incrementa a ultima id antes de enviar proxima msg

              // Escolhe o destinatario: aleatorio entre 0 e tamanho de LU - 1
              N_dest = rand()%LU.size();
              // iU2 vai apontar para o N_dest-esimo usuario
              j=0;
              iU2 = LU.begin();
              while (j<N_dest)
              {
                ++iU2;
                ++j;
              }

              // Forma a mensagem
              M.setStatus(MsgStatus::MSG_RECEBIDA);
              M.setRemetente(remetente);
              M.setDestinatario(iU2->getLogin());
              M.setId(last_id[i]);
              M.setTexto(string("Msg")+to_string(last_id[i]));

              // Informa que foi "recebida" uma nova mensagem
              imprimeComandoRecebido(remetente, CMD_NOVA_MSG, M.getId(), iU2->getLogin());
              // Insere a mensagem no buffer
              LM.push_back(M);
              iMsg = --LM.end(); // Aponta para ultima msg, a que acabou de inserir
              // Informa que foi "enviada" a confirmacao de recebimento para o remetente fake
              imprimeComandoEnviado(remetente, CMD_MSG_RECEBIDA, last_id[i]);

              // Testa se o destinatario estah conectado
              // Se sim, envia a mensagem e muda status para MSG_ENTREGUE
              if (iU2->connected())
              {
                enviarMensagem(iMsg, iU2);
              }
            } // fim if (rand()%3==0)
          } // fim for
        } // if (LU.size() > 0)
        // === END FUNCIONAMENTO FAKE ===
        break; // fim do case mysocket_status::SOCK_TIMEOUT
      case mysocket_status::SOCK_OK: // resultado do wait_read
        // Houve atividade em algum socket da fila
        // Testa em qual socket houve atividade.

        try // Erros nos clientes: catch fecha a conexao com esse cliente
        {
          // Primeiro, testa os sockets dos clientes
          for (iU=LU.begin(); !fim && iU!=LU.end(); ++iU)
          {
            if (!fim && iU->connected() && f.had_activity(iU->getSocket()))
            {
              // Leh o comando recebido do cliente
              iResult = iU->read_int(cmd);
              // Pode ser mysocket_status::SOCK_OK, mysocket_status::SOCK_TIMEOUT,
              // mysocket_status::SOCK_DISCONNECTED ou mysocket_status::SOCK_ERRO
              // Nao deve ser mysocket_status::SOCK_TIMEOUT porque a funcao read_int
              // nao foi chamada com tempo maximo
              if (iResult != mysocket_status::SOCK_OK) throw 1;

              // Executa o comando lido
              switch(cmd)
              {
              case CMD_NEW_USER:
              case CMD_LOGIN_USER:
              case CMD_LOGIN_OK:
              case CMD_LOGIN_INVALIDO:
              case CMD_MSG_INVALIDA:
              case CMD_MSG_RECEBIDA:
              case CMD_MSG_ENTREGUE:
              default:
                // Comando invalido
                throw 2;
                break;
              case CMD_NOVA_MSG:
                // Receber a msg
                iResult = iU->read_int(id, TIMEOUT_WHATSPROG*1000);
                if (iResult != mysocket_status::SOCK_OK) throw 3;
                iResult = iU->read_string(destinatario, TIMEOUT_WHATSPROG*1000);
                if (iResult != mysocket_status::SOCK_OK) throw 4;
                iResult = iU->read_string(texto, TIMEOUT_WHATSPROG*1000);
                if (iResult != mysocket_status::SOCK_OK) throw 5;

                // Pedido de nova mensagem
                imprimeComandoRecebido(iU->getLogin(), CMD_NOVA_MSG, id, destinatario);

                // Preenche o status e remetente da mensagem recebida
                if (!M.setStatus(MsgStatus::MSG_RECEBIDA) ||
                    !M.setRemetente(iU->getLogin())) throw 6;

                // Testa se a id da msg estah correta
                if (!M.setId(id) || id <= iU->getLastId()) throw 7;

                // Testa se o texto da msg estah correto
                if (!M.setTexto(texto)) throw 8;

                // Procura o destinatario na lista de usuarios cadastrado
                // === BEGIN FUNCIONAMENTO REAL ===
                // No servidor real:
                // Procura o destinatario na lista de usuarios
                // === END FUNCIONAMENTO REAL ===
                // === BEGIN FUNCIONAMENTO FAKE ===
                // Neste servidor fake, nao hah lista de usuarios cadastrados
                // Os unicos destinatarios possiveis sao "userfake1" e "userfake2"
                // === END FUNCIONAMENTO FAKE ===

                // Testa se o destinatario eh um usuario cadastrado
                // === BEGIN FUNCIONAMENTO REAL ===
                // No servidor real:
                // Testa se a busca retornou que o destinatario existe na lista de usuarios
                // === END FUNCIONAMENTO REAL ===
                // === BEGIN FUNCIONAMENTO FAKE ===
                // Neste servidor fake, testa se o destinatario eh um dos usuarios fake possiveis
                if (destinatario!="userfake1" && destinatario!="userfake2") throw 9;
                // === END FUNCIONAMENTO FAKE ===

                // Testa se o destinatario eh valido
                if (!M.setDestinatario(destinatario)) throw 9;

                // Mensagem valida
                // Insere a mensagem no buffer
                LM.push_back(M);
                iMsg = --LM.end(); // Aponta para ultima msg, a que acabou de inserir

                // Atualiza a ultima ID recebida
                iU->setLastId(id);

                // Envia a confirmacao de recebimento
                if (iU->write_int(CMD_MSG_RECEBIDA) != mysocket_status::SOCK_OK) throw 10;
                if (iU->write_int(id) != mysocket_status::SOCK_OK) throw 11;

                // Confirmacao de recebimento enviada
                imprimeComandoEnviado(iU->getLogin(), CMD_MSG_RECEBIDA, id);

                // Testa se o destinatario estah conectado
                // Se sim, envia a mensagem e muda status para MSG_ENTREGUE
                // === BEGIN FUNCIONAMENTO REAL ===
                // No servidor real:
                // 1) Testa se o destinatario estah conectado
                // 2) Se sim, utiliza a funcao auxiliar enviarMensagem
                // === END FUNCIONAMENTO REAL ===
                // === BEGIN FUNCIONAMENTO FAKE ===
                // Neste servidor fake, userfake1 e userfake2 estao sempre "conectados"
                // e os envios para eles sempre dao certo...
                // Vamos fazer aqui mesmo as operacoes que, no servidor real, seriam
                // executadas na funcao auxiliar enviarMensagem:
                // a) informa que foi "enviada" a mensagem para o remetente fake
                imprimeComandoEnviado(iMsg->getDestinatario(), CMD_NOVA_MSG,
                                      iMsg->getId(), iMsg->getRemetente());
                // b) Mensagem "enviada": atualiza status no "buffer"
                iMsg->setStatus(MsgStatus::MSG_ENTREGUE);
                // c) Testa se o remetente estah conectado (normalmente deve estar)
                // Se sim, envia a confirmacao de entrega da mensagem
                if (iU->connected())
                {
                  enviarConfirmacao(iMsg, iU);
                }
                // === END FUNCIONAMENTO FAKE ===
                break; // Fim do case CMD_NOVA_MSG
              case CMD_LOGOUT_USER:
                imprimeComandoRecebido(iU->getLogin(), CMD_LOGOUT_USER);
                iU->close();
                break; // Fim do case CMD_LOGOUT_USER
              } // Fim do switch(cmd)
            } // Fim do if (... && had_activity) no socket do cliente
          } // Fim do for para todos os clientes
        } // Fim do try para erros nos clientes
        catch (int erro) // Erros na leitura do socket de algum cliente
        {
          if (erro>=6 && erro<=9)
          {
            // Comunicacao OK, mensagem invalida
            // Envia comando informando msg mal formatada
            iU->write_int(CMD_MSG_INVALIDA);
            iU->write_int(id);
            // Comando de mensagem invalida enviado
            imprimeComandoEnviado(iU->getLogin(), CMD_MSG_INVALIDA, id, destinatario);
          }
          else // erro 1-6 e 10-11
          {
            // Erro na comunicacao
            // Fecha o socket
            iU->close();
            // Informa o erro imprevisto
            cerr << "Erro " << erro << " na leitura de nova mensagem do cliente "
                 << iU->getLogin() << endl;
          }
        }

        // Depois de testar os sockets dos clientes,
        // testa se houve atividade no socket de conexao
        if (f.had_activity(c))
        {
          tcp_mysocket t;
          string login, senha;

          // Aceita provisoriamente a nova conexao
          if (c.accept(t) != mysocket_status::SOCK_OK) throw 3; // Erro grave: encerra o servidor

          try // Erros na conexao de cliente: fecha socket temporario ou desconecta novo cliente
          {
            // Leh o comando
            iResult = t.read_int(cmd, TIMEOUT_LOGIN_WHATSPROG*1000);
            if (iResult != mysocket_status::SOCK_OK) throw 1;

            // Testa o comando
            if (cmd!=CMD_LOGIN_USER && cmd!=CMD_NEW_USER) throw 2;

            // Leh o parametro com o login do usuario que deseja se conectar
            iResult = t.read_string(login, TIMEOUT_LOGIN_WHATSPROG*1000);
            if (iResult != mysocket_status::SOCK_OK) throw 3;

            // Leh o parametro com a senha do usuario que deseja se conectar
            iResult = t.read_string(senha, TIMEOUT_LOGIN_WHATSPROG*1000);
            if (iResult != mysocket_status::SOCK_OK) throw 4;

            // Nova tentativa de conexao recebida
            imprimeComandoRecebido(login, ComandoWhatsProg(cmd));

            // Testa o login e senha
            if (!testarNomeUsuario(login) || !testarSenha(senha)) throw 5;

            // === BEGIN FUNCIONAMENTO FAKE ===
            // Este servidor fake soh pode ter um usuario conectado a cada instante
            // Soh vai aceitar o novo cliente se ninguem estiver conectado
            int N_conectados = count_if(LU.begin(), LU.end(), usuario_conectado);
            if (N_conectados>0) throw 10; // Mesma excecao (10) para caso de usuario jah conectado
            // === END FUNCIONAMENTO FAKE ===

            // Verifica se jah existe um usuario cadastrado com esse login
            iU = find(LU.begin(), LU.end(), login);

            // Testa se o usuario eh adequado, o que depende se estah se logando como
            // novo cliente ou como cliente jah cadastrado
            if (cmd == CMD_NEW_USER)
            {
              if (iU!=LU.end()) throw 6; // Erro se jah existir

              // Insere novo usuario
              Usuario novo;
              if (!novo.setUsuario(login,senha)) throw 7;
              novo.swapSocket(t);
              LU.push_back(novo);
              iU = --LU.end(); // iU aponto para o ultimo elemento de LU == novo
            }
            else  // else cmd == CMD_NEW_USER; implica cmd eh CMD_LOGIN_USER
            {
              if (iU==LU.end()) throw 8; // Erro se nao existir

              // Testa se a senha confere
              if (!iU->validarSenha(senha)) throw 9; // Senha nao confere
              // Testa se o cliente jah estah conectado
              if (iU->connected()) throw 10; // Usuario jah conectado
              // Associa o socket que se conectou a um usuario cadastrado
              iU->swapSocket(t);
            } // fim cmd eh CMD_LOGIN_USER

            // Envia a confirmacao de conexao para o novo cliente
            if (iU->write_int(CMD_LOGIN_OK) != mysocket_status::SOCK_OK) throw 11;
            imprimeComandoEnviado(iU->getLogin(), CMD_LOGIN_OK);

            // Se for um cliente antigo, envia para o cliente que se reconectou:
            // a) as mensagens enviadas para ele que estao no buffer
            // b) as confirmacoes de visualizacao para ele que estao no buffer
            if (cmd==CMD_LOGIN_USER)
            {
              // mensagens enviadas para ele que estao no buffer
              do
              {
                iMsg = find(LM.begin(), LM.end(),
                            destStatus(iU->getLogin(),MsgStatus::MSG_RECEBIDA));
                if (iMsg != LM.end())
                {
                  enviarMensagem(iMsg,iU);
                  // Apos isso, a msg nao estarah com status MSG_RECEBIDA
                  // e nao serah mais encontrada no novo find
                }
              }
              while (iMsg != LM.end());
              // as confirmacoes de entrega para ele que estao no buffer
              do
              {
                iMsg = find(LM.begin(), LM.end(),
                            remetStatus(iU->getLogin(),MsgStatus::MSG_ENTREGUE));
                if (iMsg != LM.end())
                {
                  enviarConfirmacao(iMsg,iU);
                  // Apos isso, a msg nao estarah mais no buffer
                  // e nao serah mais encontrada no novo find
                }
              }
              while (iMsg != LM.end());
            } // fim do if (cmd==CMD_LOGIN_USER)
          } // Fim do try para erros na comunicacao com novo cliente
          catch (int erro) // Erros na conexao do novo cliente
          {
            if (erro>=5 && erro<=10)
            {
              // Comunicacao com socket temporario OK, login invalido
              // Envia comando informando login invalido
              t.write_int(CMD_LOGIN_INVALIDO);

              // Comando login invalido enviado
              imprimeComandoEnviado(login, CMD_LOGIN_INVALIDO);

              // Fecha o socket temporario
              t.close();
            }
            else // erro 1-4 ou 11
            {
              // Erro na comunicacao
              // Fecha o socket
              if (erro==11)
              {
                // Erro na comunicacao com socket do novo cliente
                iU->close();
              }
              else  // erro 1-4
              {
                // Erro na comunicacao com socket temporario
                t.close();
              }
              // Informa erro nao previsto
              cerr << "Erro " << erro << " na conexao de novo cliente" << endl;
            }
          } // fim catch
        } // fim if (had_activity) no socket de conexoes
        break; // fim do case mysocket_status::SOCK_OK - resultado do wait_read
      } // fim do switch (iResult) - resultado do wait_read

    } // Fim do try para erros criticos no servidor
    catch (int erro) // Erros criticos no servidor
    {
      cerr << "Erro " << erro << " no servidor. Encerrando\n";
      fim = true;
    }

  } // Fim do while (!fim), laco principal da thread

  cout << "\nServidor encerrado.\n";

  return 0;
}

/// Retorna o nome do arquivo para ler/salvar os dados
string WhatsProgDadosServidor::getNomeArq() const
{
  return "whatsdata_servidor.wds";
}

/// Ler de arquivo
/// O nome do arquivo eh padronizado
bool WhatsProgDadosServidor::ler()
{
  // Implementacao opcional
  // Falta fazer, caso deseje implementar o salvamento de dados
  // Deve utilizar:
  // 1) A funcao WhatsProgDadosServidor::getNomeArq para obter o nome do arquivo de onde ler
  // 2) A funcao Usuario::ler para ler cada usuario
  // 3) A funcao Mensagem::ler para ler cada mensagem
  return false;
}

/// Salvar em arquivo
/// O nome do arquivo eh padronizado
bool WhatsProgDadosServidor::salvar() const
{
  // Implementacao opcional
  // Falta fazer, caso deseje implementar o salvamento de dados
  // Deve utilizar:
  // 1) A funcao WhatsProgDadosServidor::getNomeArq para obter o nome do arquivo onde salvar
  // 2) A funcao Usuario::salvar para salvar cada usuario
  // 3) A funcao Mensagem::salvar para salvar cada mensagem
  return false;
}
