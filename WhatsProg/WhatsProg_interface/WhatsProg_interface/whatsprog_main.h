#ifndef WHATSPROGMAIN_H
#define WHATSPROGMAIN_H

#include <QMainWindow>
#include <QLabel>
#include "whatsprog_login.h"
#include "whatsprog_novaconversa.h"
#include "whatsprog_thread.h"

namespace Ui {
class WhatsProgMain;
}

class WhatsProgMain : public QMainWindow
{
  Q_OBJECT

private:
  Ui::WhatsProgMain *ui;

  /// O objeto gerenciador da thread
  WhatsProgThread *thread;

  /// As caixas de dialogo de login e de nova conversa
  WhatsProgLogin *loginDialog;
  WhatsProgNovaConversa *novaConversa;

  /// O texto da barra de status
  QLabel *msgStatus;

  /// Os icones do status das mensagens
  QPixmap pixEnviada;
  QPixmap pixRecebida;
  QPixmap pixEntregue;
  QPixmap pixOther;

  /// Redesenha a barra de status
  void atualizaEstadoConexao();

signals:
  /// Abrir a janela de login (novo usuario/usuario existente)
  void signLogin(bool NovoUsuario);

  /// Exibe a janela de nova conversa
  void signShowNovaConversa();

  /// Iniciar a thread de leitura do socket
  void signIniciarThread();

  /// Encerrar a thread de leitura do socket
  void signEncerrarThread();

private slots:
  /// Coloca a interface em modo desconectado
  void on_actionDesconectar_triggered();

  void on_actionNovo_usuario_triggered();
  void on_actionUsuario_existente_triggered();
  void on_actionSair_triggered();
  void on_tableConversas_activated(const QModelIndex &index);
  void on_tableConversas_clicked(const QModelIndex &index);
  void on_lineEditMensagem_returnPressed();
  void on_actionNova_conversa_triggered();
  void on_actionRemover_conversa_triggered();
  void on_actionApagar_mensagens_triggered();

public slots:
  /// Conecta-se ao servidor
  void slotConectar(const QString& IP, const QString& login,
                    const QString& senha, bool novoUsuario );

  /// Redesenha a janela de conversas
  void slotExibirConversas();

  /// Redesenha a janela de mensagens
  void slotExibirMensagens();

  /// Exibe um pop-up com mensagem de erro
  void slotExibirErroMensagem(const std::string S);

public:
  explicit WhatsProgMain(QWidget *parent = 0);
  ~WhatsProgMain();

  /// Inicia a thread (emite o sinal apropriado)
  void iniciarThread();

  /// Encerra a thread (emite o sinal apropriado)
  void encerrarThread();
};

#endif // WHATSPROGMAIN_H
