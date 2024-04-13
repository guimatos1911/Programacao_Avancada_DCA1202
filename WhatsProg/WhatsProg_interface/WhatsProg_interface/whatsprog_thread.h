#ifndef WHATSPROG_THREAD_H
#define WHATSPROG_THREAD_H
#include <QObject>
#include <string>
#include <thread>

class WhatsProgThread: public QObject
{
  Q_OBJECT

private:
  /// A thread
  std::thread thr;

  /// Esta eh a funcao principal da thread
  /// Ela recebe as informacoes do socket e armazena na variavel DC,
  /// de onde elas serao exibidas pela interface visual atraves da emissao de sinais
  int main_thread();

  /// A funcao main_thread() executa o metodo main_thread()
  friend int main_thread(WhatsProgThread *MT);

signals:
  void signExibirConversas();
  void signExibirMensagens();
  void signExibirErroMensagem(const std::string&);
  void signDesconectarInterface();

public slots:
  /// Iniciar a thread
  void slotIniciar();
  /// Terminar a thread
  void slotEncerrar();

public:
  explicit WhatsProgThread(QObject *parent = nullptr);
};

#endif // WHATSPROG_THREAD_H
