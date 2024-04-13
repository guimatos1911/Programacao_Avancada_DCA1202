#include "whatsprog_thread.h"
#include "mysocket.h"
#include "whatsprog_dados_cliente.h"

using namespace std;

/// A variavel externa (global) que contem todas as msgs de todas as conversas
/// Falta fazer
extern WhatsProgDadosCliente DC;
/// A variavel externa (global) do socket do cliente, a ser utilizado por todas as threads
/// Falta fazer
extern tcp_mysocket sock;

WhatsProgThread::WhatsProgThread(QObject *parent) :
  QObject(parent), thr()
{
}

/// Esta eh a funcao principal da thread
/// Ela recebe as informacoes do socket e armazena na variavel apropriada,
/// de onde elas serao exibidas pela interface visual atraves da emissao de sinais
int WhatsProgThread::main_thread()
{
  // while (socket conectado) { ... }
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
                    emit signExibirConversas();
                }
                catch (int i)
                {
                    // Nao conseguiu ler a msg recebida ou inserir na lista de mensagens
                    sock.close();

                    emit signExibirErroMensagem("Erro " + to_string(i) + " apos comando CMD_NOVA_MSG");
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

                    emit signExibirErroMensagem("Erro " + to_string(i) + " apos comando CMD_MSG_RECEBIDA ou CMD_MSG_ENTREGUE");
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
                    emit signExibirErroMensagem("Mensagem " + to_string(id) + " com erro");
                }
                catch (int i)
                {
                    // Nao conseguiu ler o comando ou remover a msg
                    sock.close();

                    emit signExibirErroMensagem("Erro " + to_string(i) + " apos comando CMD_MSG_INVALIDA");
                }
                break; // Fim do case CMD_MSG_INVALIDA
            } // Fim do switch(cmd)
            break; // Fim do case mysocket_status::SOCK_OK

            // A leitura do comando (int) retornou por timeout
            // Aproveita para salvar o arquivo com os dados
        case mysocket_status::SOCK_TIMEOUT:
            if (!DC.salvar())
            {
                emit signExibirErroMensagem("Erro no salvamento do arquivo "+DC.getNomeArq());
            }
            break; // Fim do case mysocket_status::SOCK_TIMEOUT

            // A leitura do comando (int) retornou por
            // mysocket_status::SOCK_ERROR ou mysocket_status::SOCK_DISCONNECTED
            // A conexao encerrou, de forma correta ou com erro.
        case mysocket_status::SOCK_ERROR:
        case mysocket_status::SOCK_DISCONNECTED:
        default:
            sock.close();
            emit signExibirErroMensagem("Erro na leitura do comando no socket");
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
  // Coloca a interface em modo desconectado
  // Vai salvar as informacoes do cliente
  emit signDesconectarInterface();

  return 0;
}

/// Funcao que executa o metodo main_thread da classe WhatsProgThread
int main_thread(WhatsProgThread *MT)
{
  return MT->main_thread();
}

/// Iniciar a thread
void WhatsProgThread::slotIniciar()
{
  thr = std::thread(::main_thread, this);
}

/// Terminar a thread
void WhatsProgThread::slotEncerrar(void)
{
  // Encerra o socket
  sock.close();
  // Espera o encerramento da thread
  if (thr.joinable()) thr.join();
  thr = std::thread(); // Thread invalida (nao em execucao)
}
