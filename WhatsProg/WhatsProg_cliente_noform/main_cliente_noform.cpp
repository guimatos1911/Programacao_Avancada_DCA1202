#include "whatsprog_dados_cliente.h"
#include "../MySocket/mysocket.h"
#include <cstdio>
#include <thread>

using namespace std;

/// Variaveis globais
/// Precisam ser acessadas em todas as threads e janelas

/// A variavel global que contem todas as msgs de todas as conversas
WhatsProgDadosCliente DC;

/// O socket do cliente, a ser utilizado por todas as threads
tcp_mysocket sock;

/// A thread de leitura do socket
thread thr;

/// Funcoes auxiliares

/// Solicita ao usuario uma opcao de conexao
/// Retorna true se ok, false se o usuario escolheu sair
bool executa_menu_conexao();
/// Solicita ao usuario uma opcao de conversa
bool executa_menu_conversa();
/// Solicita ao usuario uma opcao de mensagem
bool executa_menu_mensagem();
/// Conecta com o servidor
bool aceitaUsuario(const string& IP, const string& login,
                   const string& senha, bool novoUsuario);
/// Exibe aviso de nova msg recebida
void avisaNovaMsg(iterConversa it);
/// Emite mensagem de erro e espera confirmacao de leitura
void emiteErro(const string& msg);

/// ==============================
/// Funcao principal (dialogo com o usuario)
/// ==============================

int main(int argc, char *argv[])
{
  // Inicializa a biblioteca de sockets (exigida no Windows)
  if (mysocket::init() != mysocket_status::SOCK_OK)
  {
    cerr << "Biblioteca mysocket nao pode ser inicializada\n";
    exit(1);
  }

  // Flag de encerramento do programa a pedido do usuario
  bool fim(false);

  cout << "WhatsProg - cliente sem formatacao\n";
  do
  {
    if (!sock.connected())
    {
      // Antes do socket fechar, estava executando a thread?
      if (thr.joinable())
      {
        // Espera pelo fim da thread de recepcao
        thr.join();

        // Salva as informacoes do cliente
        DC.salvar();

        // Limpa os nomes do servidor e usuario
        DC.unsetServidorUsuario();
      }

      // Pede informacoes de conexao (servidor, login, senha) ao usuario
      fim = !executa_menu_conexao();
    }
    else
    {
      // Testa se existe uma conversa ativa
      if (DC.getConversaAtual() == DC.end())
      {
        // Nao hah conversa ativa: pede ao usuario para criar/selecionar uma
        fim = !executa_menu_conversa();
      }
      else
      {
        // Exibe as opcoes sobre as mensagens da conversa ativa (ler, escrever, etc.)
        fim = !executa_menu_mensagem();
      }
    }
  }
  while (!fim);

  // Envia o comando de logout, caso jah nao tenha sido enviado
  if (sock.connected())
  {
    sock.write_int(CMD_LOGOUT_USER);
  }

  // Fecha o socket, caso jah nao tenha sido fechado
  sock.close();

  // Salva as informacoes do cliente
  DC.salvar();

  // Espera pelo fim da thread de recepcao, caso estivesse executando
  if (thr.joinable()) thr.join();

  /// Encerramento da biblioteca de sockets
  mysocket::end();

  return 0;
}

/// ==============================
/// Esta eh a thread que recebe as informacoes do socket e
/// armazena na variavel DC, de onde elas poderao ser
/// exibidas pela interface
/// ==============================

void main_thread(void)
{
  // O status de retorno das funcoes do socket
  mysocket_status iResult;
  // O comando, lido do socket
  int32_t cmd;
  // Eventuais parametros do comando, lidos do socket
  int32_t id;
  string remetente;
  string texto;
  // Eventuais parametros retornados pelas funcoes de procura por mensagens
  iterConversa it;
  int ind_msg;

  while (sock.connected())
  {
    iResult = sock.read_int(cmd,1000*TIMEOUT_WHATSPROG);

    switch (iResult)
    {

    // A leitura do comando retornou corretamente um valor (int)
    case mysocket_status::SOCK_OK:
      // Decide o que fazer de acordo com o valor do comando lido
      switch(cmd)
      {
      case CMD_NEW_USER:
      case CMD_LOGIN_USER:
      case CMD_LOGIN_OK:
      case CMD_LOGIN_INVALIDO:
      case CMD_LOGOUT_USER:
      default:
        // Ignorar: erro do servidor
        break;
      case CMD_NOVA_MSG:
        // Receber a msg
        try
        {
          iResult = sock.read_int(id, TIMEOUT_WHATSPROG*1000);
          if (iResult != mysocket_status::SOCK_OK) throw 1;
          iResult = sock.read_string(remetente, TIMEOUT_WHATSPROG*1000);
          if (iResult != mysocket_status::SOCK_OK) throw 2;
          iResult = sock.read_string(texto, TIMEOUT_WHATSPROG*1000);
          if (iResult != mysocket_status::SOCK_OK) throw 3;

          // Mensagem a ser inserida na conversa
          Mensagem M;

          // Fixa os parametros da msg e testa se estao corretos
          if (!M.setId(id) ||
              !M.setRemetente(remetente) ||
              !M.setDestinatario(DC.getMeuUsuario()) ||
              !M.setTexto(texto) ||
              !M.setStatus(MsgStatus::MSG_ENTREGUE)) throw 4;


          it = DC.findConversa(remetente);
          if (it == DC.end()) // Nao existia conversa com esse remetente
          {
            // Cria (insere) uma nova conversa
            if (!DC.insertConversa(remetente)) throw 5;

            // A conversa com o remetente eh a recem-inserida (a ultima)
            it = DC.last();
          }

          // Insere a mensagem na conversa para a qual it aponta
          DC.pushMessage(it,M);

          // Move a conversa com a nova msg para o inicio da lista (begin)
          DC.moveConversaToBegin(it);

          // Modificou o numero de mensagens da conversa
          // Atualizar a janela de conversas
          avisaNovaMsg(it);
        }
        catch (int i)
        {
          // Nao conseguiu ler a msg recebida ou inserir na lista de mensagens
          sock.close();

          emiteErro("Erro " + to_string(i) + " apos comando CMD_NOVA_MSG");
        }
        break; // fim do case CMD_NOVA_MSG
      case CMD_MSG_RECEBIDA:
      case CMD_MSG_ENTREGUE:
        // Receber o comando
        try
        {
          iResult = sock.read_int(id, TIMEOUT_WHATSPROG*1000);
          if (iResult != mysocket_status::SOCK_OK) throw 1; // Erro socket na leitura do cmd

          // Procura se existe uma mensagem com essa id;
          DC.findMensagem(id, it, ind_msg);
          if (it==DC.end() || ind_msg<0) throw 2; // Nao encontrou a msg

          // Encontrou a msg com essa id
          // Testa o remetente
          if (it->at(ind_msg).getRemetente()!=DC.getMeuUsuario()) throw 3;

          // Testa o status atual da msg
          if (cmd==CMD_MSG_RECEBIDA)
          {
            if (it->at(ind_msg).getStatus()!=MsgStatus::MSG_ENVIADA) throw 4;

            // A msg encontrada eh de minha autoria e estah com o status apropriado
            // Altera para o novo status
            DC.setStatus(it,ind_msg,MsgStatus::MSG_RECEBIDA);
          }
          else // cmd==CMD_MSG_ENTREGUE
          {
            if (it->at(ind_msg).getStatus()!=MsgStatus::MSG_RECEBIDA) throw 5;

            // A msg encontrada eh de minha autoria e estah com o status apropriado
            // Altera para o novo status
            DC.setStatus(it,ind_msg,MsgStatus::MSG_ENTREGUE);
          }
        }
        catch (int i)
        {
          // Nao conseguiu ler o comando ou alterar o status da msg
          sock.close();

          emiteErro("Erro " + to_string(i) + " apos comando CMD_MSG_RECEBIDA ou CMD_MSG_ENTREGUE");
        }
        break; // fim do case CMD_MSG_RECEBIDA ou CMD_MSG_ENTREGUE
      case CMD_MSG_INVALIDA:
        // Receber o comando
        try
        {
          iResult = sock.read_int(id, TIMEOUT_WHATSPROG*1000);
          if (iResult != mysocket_status::SOCK_OK) throw 1; // Erro socket na leitura do cmd

          // Procura se existe uma mensagem com essa id;
          DC.findMensagem(id, it, ind_msg);
          if (it==DC.end() || ind_msg<0) throw 2; // Nao encontrou a msg

          // Encontrou a msg com essa id
          // Testa o remetente e o status
          if (it->at(ind_msg).getRemetente()!=DC.getMeuUsuario() ||
              it->at(ind_msg).getStatus()!=MsgStatus::MSG_ENVIADA) throw 3;

          // A msg encontrada eh de minha autoria e estah com o status apropriado
          // Remover msg
          DC.eraseMessage(it,ind_msg);

          // Informar erro nos parametros da msg
          emiteErro("Mensagem " + to_string(id) + " com erro");
        }
        catch (int i)
        {
          // Nao conseguiu ler o comando ou remover a msg
          sock.close();

          emiteErro("Erro " + to_string(i) + " apos comando CMD_MSG_INVALIDA");
        }
        break; // Fim do case CMD_MSG_INVALIDA
      } // Fim do switch(cmd)
      break; // Fim do case mysocket_status::SOCK_OK

    // A leitura do comando (int) retornou por timeout
    // Aproveita para salvar o arquivo com os dados
    case mysocket_status::SOCK_TIMEOUT:
      if (!DC.salvar())
      {
        emiteErro("Erro no salvamento do arquivo "+DC.getNomeArq());
      }
      break; // Fim do case mysocket_status::SOCK_TIMEOUT

    // A leitura do comando (int) retornou por
    // mysocket_status::SOCK_ERROR ou mysocket_status::SOCK_DISCONNECTED
    // A conexao encerrou, de forma correta ou com erro.
    case mysocket_status::SOCK_ERROR:
    case mysocket_status::SOCK_DISCONNECTED:
    default:
      sock.close();
      emiteErro("Erro na leitura do comando no socket");
      break; // Fim do case mysocket_status::SOCK_ERROR ou SOCK_DISCONNECTED

    } // Fim do switch (iResult)
  } // fim do while (sock.connected())

  // Envia o comando de logout
  if (sock.connected()) sock.write_int(CMD_LOGOUT_USER);

  // Encerra o socket
  sock.close();

  // Salva as informacoes do cliente
  DC.salvar();

  // Lembrar que essa eh a funcao da thread. Ela nao pode dar um "join" nela mesma
  // A thread vai se encerrar aqui e estarah pronta para que o programa principal deh join
}

/// ==============================
/// Funcoes auxiliares
/// ==============================

/// Solicita ao usuario uma opcao de conexao
/// Retorna true se ok, false se o usuario escolheu sair (opcao 0)
bool executa_menu_conexao()
{
  int opcao;

  if (sock.connected())
  {
    emiteErro("Esta funcao soh deve ser chamada quando o cliente estah desconectado");
    return false;
  }
  if (thr.joinable())
  {
    emiteErro("Esta funcao soh deve ser chamada quando a thread nao estah executando (joinable)");
    return false;
  }

  cout << endl;
  cout << "1 - Conectar criando um novo login\n";
  cout << "2 - Conectar com um login jah cadastrado\n";
  cout << "0 - Sair\n";
  do
  {
    cout << "Opcao: ";
    cin >> opcao;
  }
  while (opcao<0 || opcao>2);
  if (opcao != 0)
  {
    string IP;
    string login;
    string senha;
    bool novoUsuario = (opcao==1); // true se for 1, false se for 0

    // Leh os paramentros da conexao
    do
    {
      cout << "IP: ";
      cin >> IP;
    }
    while (IP.size() == 0);
    do
    {
      cout << "Login: ";
      cin >> login;
    }
    while (!testarNomeUsuario(login));
    do
    {
      cout << "Senha: ";
      cin >> senha;
    }
    while (!testarSenha(senha));

    // A funcao aceita usuario faz a conexao do socket e cria a thread
    if (!aceitaUsuario(IP, login, senha, novoUsuario))
    {
      emiteErro("Erro na conexao com IP "+IP+" e login "+login);
      return false;
    }

    // Executou corretamente a opcao escolhida (1 ou 2)
    return true;
  }

  // Usuario escolheu opcao 0 - Sair
  return false;
}

/// Solicita ao usuario uma opcao de conversa
/// Retorna true se ok, false se o usuario escolheu sair (opcao 0)
bool executa_menu_conversa()
{
  int opcao;

  if (!sock.connected())
  {
    emiteErro("Esta funcao soh deve ser chamada quando o cliente estah conectado");
    return false;
  }
  if (!thr.joinable())
  {
    emiteErro("Esta funcao soh deve ser chamada quando a thread estah executando (joinable)");
    return false;
  }
  if (DC.getConversaAtual() != DC.end())
  {
    emiteErro("Esta funcao soh deve ser chamada quando a conversa ativa nao estiver definida");
    return false;
  }

  cout << "1 - Listar conversas\n";
  cout << "2 - Criar nova conversa e torna-la ativa\n";
  cout << "3 - Alterar conversa ativa\n";
  cout << "4 - Apagar conversa\n";
  cout << "5 - Desconectar\n";
  cout << "0 - Sair\n";
  do
  {
    cout << "Opcao: ";
    cin >> opcao;
  }
  while (opcao<0 || opcao>5);

  // Nome da conversa (opcao 2, 3 ou 4)
  string conversa;
  // Apontador para a conversa (opcao 4)
  iterConversa it;

  switch (opcao)
  {
  case 1:
    cout << endl;
    for (iterConversa it=DC.begin(); it!=DC.end(); ++it)
    {
      if (it==DC.getConversaAtual()) cout << '*';
      else cout << ' ';
      cout << it->getCorrespondente() << " ("
           << it->size() << ") msgs\n";
    }
    break; // fim case 1
  case 2:
  case 3:
    do
    {
      cout << "Conversa: ";
      cin >> conversa;
    }
    while (!testarNomeUsuario(conversa));

    // Insere nova conversa na base de dados, se for opcao 2
    if (opcao == 2)
    {
      if (!DC.insertConversa(conversa))
      {
        emiteErro("Nova conversa "+conversa+" invalida");
        break;
      }
    }

    // Faz a conversa ser a conversa ativa
    if (DC.setConversaAtual(conversa))
    {
      cout << "Conversa " << conversa << " eh a nova conversa ativa\n";
    }
    else
    {
      emiteErro("Erro ao fazer a conversa "+conversa+" ser a nova conversa ativa");
    }
    break; // fim case 2, case 3
  case 4:
    do
    {
      cout << "Conversa: ";
      cin >> conversa;
    }
    while (!testarNomeUsuario(conversa));

    // procura a conversa
    it = DC.findConversa(conversa);
    if (it == DC.end())
    {
      emiteErro("Conversa "+conversa+" inexistente");
      break;
    }
    if (!DC.eraseConversa(it))
    {
      emiteErro("Erro ao excluir conversa "+conversa);
      break;
    }
    cout << "Conversa " << conversa << " apagada\n";
    break; // fim case 4
  case 5:
    if (sock.connected())
    {
      // Envia o comando de logout para o servidor
      sock.write_int(CMD_LOGOUT_USER);
    }

    // Fecha o socket e, consequentemente,
    // encerra a thread de leitura de dados do socket
    sock.close();
    break; // fim case 5
  case 0:
    return false;
  }  // fim switch
  return true;
}

/// Solicita ao usuario uma opcao de mensagem
/// Retorna true se ok, false se o usuario escolheu sair (opcao 0)
bool executa_menu_mensagem()
{
  int opcao;

  if (!sock.connected())
  {
    emiteErro("Esta funcao soh deve ser chamada quando o cliente estah conectado");
    return false;
  }
  if (!thr.joinable())
  {
    emiteErro("Esta funcao soh deve ser chamada quando a thread estah executando (joinable)");
    return false;
  }
  iterConversa it = DC.getConversaAtual();
  if (it == DC.end())
  {
    emiteErro("Esta funcao soh deve ser chamada quando a conversa ativa estiver definida");
    return false;
  }

  cout << "1 - Listar mensagens\n";
  cout << "2 - Escrever mensagem\n";
  cout << "3 - Apagar todas as mensagens\n";
  cout << "4 - Voltar ao menu de conversas\n";
  cout << "0 - Sair\n";
  do
  {
    cout << "Opcao: ";
    cin >> opcao;
  }
  while (opcao<0 || opcao>4);

  // Texto e msg a serem enviados (opcao 2)
  string texto;
  Mensagem M;

  switch (opcao)
  {
  case 1:
    cout << "\nConversa " << it->getCorrespondente() << ":\n";
    for (int i=0; i<it->size(); ++i)
    {
      cout << it->at(i).getId() << ") "
           << it->at(i).getRemetente() << "->"
           << it->at(i).getDestinatario() << ": "
           << it->at(i).getTexto();
      if (it->at(i).getRemetente() == DC.getMeuUsuario())
      {
        // A mensagem eh de minha autoria
        cout << " (";
        if (it->at(i).getStatus() == MsgStatus::MSG_ENVIADA) cout << "?";
        if (it->at(i).getStatus() == MsgStatus::MSG_RECEBIDA) cout << "V";
        if (it->at(i).getStatus() == MsgStatus::MSG_ENTREGUE) cout << "VV";
        cout << ")";
      }
      cout << endl;
    } // fim for
    break; // fim case 1
  case 2:
    cout << "\nMensagem para " << it->getCorrespondente() << ":\n";
    do
    {
      cout << "Texto (1 a 255 chars): ";
      cin >> ws;
      getline(cin,texto);
    }
    while (!testarTextoMensagem(texto));

    try
    {
      // Define os campos da msg
      if (!M.setId(DC.getNovaIdMensagem()) ||
          !M.setRemetente(DC.getMeuUsuario()) ||
          !M.setDestinatario(it->getCorrespondente()) ||
          !M.setTexto(texto) ||
          !M.setStatus(MsgStatus::MSG_ENVIADA)) throw 1;

      // Inclui a msg na base de dados local, acrescentando no final da conversa
      //
      // Isso tem que ser feito antes de enviar via socket porque se a resposta do
      // servidor confirmando o recebimento for enviada imediatamente, a mensagem jah
      // tem que estar na base de dados para que o cliente modifique corretamente seu status
      DC.pushMessage(it,M);

      // Move a conversa com a nova msg para o inicio da lista (begin)
      DC.moveConversaToBegin(it);

      // Envia a msg via socket para o servidor
      if (sock.write_int(CMD_NOVA_MSG) != mysocket_status::SOCK_OK) throw 2;
      if (sock.write_int(M.getId()) != mysocket_status::SOCK_OK) throw 3;
      if (sock.write_string(M.getDestinatario()) != mysocket_status::SOCK_OK) throw 4;
      if (sock.write_string(M.getTexto()) != mysocket_status::SOCK_OK) throw 5;
    }
    catch (int i)
    {
      // Desconecta
      sock.close();

      if (i>1)
      {
        // Remove ultima msg da conversa
        DC.popMessage(it);
      }
      emiteErro("Erro " + to_string(i) + " no envio da mensagem");
    }
    break; // fim case 2
  case 3:
    DC.clearMessages(it);
    break; // fim case 3
  case 4:
    DC.unsetConversaAtual();
    break; // fim case 4
  case 0:
    return false;
  }  // fim switch
  return true;
}

/// Conecta com o servidor
bool aceitaUsuario(const string& IP, const string& login,
                   const string& senha, bool novoUsuario )
{
  try
  {
    if (sock.connected()) throw 1;
    if (thr.joinable()) throw 2;

    int32_t cmd;

    // Conecta o socket
    if (sock.connect(IP, PORTA_WHATSPROG) != mysocket_status::SOCK_OK) throw 3;

    // Envia a msg de conexao para o servidor, atraves do socket
    if (novoUsuario)
    {
      // Testa se eh possivel cadastrar um novo usuario com esses ip, login e senha
      // Envia o comando CMD_NEW_USER
      if (sock.write_int(CMD_NEW_USER) != mysocket_status::SOCK_OK) throw 4;
    }
    else
    {
      // Testa se eh possivel se conectar com esses ip, login e senha
      // Envia o comando CMD_LOGIN_USER
      if (sock.write_int(CMD_LOGIN_USER) != mysocket_status::SOCK_OK) throw 5;
    }

    // Envia os parametros do comando (login, senha)
    if (sock.write_string(login) != mysocket_status::SOCK_OK) throw 6;
    if (sock.write_string(senha) != mysocket_status::SOCK_OK) throw 7;

    // Leh a resposta do servidor ao pedido de conexao
    if (sock.read_int(cmd,1000*TIMEOUT_WHATSPROG) != mysocket_status::SOCK_OK) throw 8;
    if (cmd != CMD_LOGIN_OK) throw 9;

    // Armazena o novo nome do servidor e do usuario
    DC.setServidorUsuario(IP,login);

    // Leh o arquivo com os dados de conexao anterior, caso exista
    DC.ler();

    // Lanca a thread de leitura dos dados do socket
    // Tem que ser depois de ler o arquivo com os dados da conexao anterior
    // Senao, pode ficar com duas conversas referentes ao mesmo usuario:
    // 1) Chegou uma msg que estava retida no servidor
    // 2) Leu uma conversa previa com mesmo usuario do arquivo
    thr = thread(main_thread);

    // Se soh tiver uma conversa, seleciona-a como ativa
    if (DC.size() == 1)
    {
      DC.setConversaAtual(DC.begin());
    }
  }
  catch (int i)
  {
    sock.close();
    emiteErro("Erro " + to_string(i) + " na conexao com servidor");
    return false;
  }

  // Tudo OK!
  return true;
}

/// Exibe aviso de nova msg recebida
void avisaNovaMsg(iterConversa it)
{
  cout << "\nNova msg recebida (" << it->size()
       << ") na conversa " << it->getCorrespondente() << endl;
}

/// Emite mensagem de erro
void emiteErro(const string& msg)
{
  cerr << "\nERRO: " << msg << "!\n";
}
