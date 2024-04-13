#ifndef WHATSPROG_LOGIN_H
#define WHATSPROG_LOGIN_H

#include <QDialog>

namespace Ui {
class WhatsProgLogin;
}

class WhatsProgLogin : public QDialog
{
  Q_OBJECT

public:
  explicit WhatsProgLogin(QWidget *parent = 0);
  ~WhatsProgLogin();

public slots:
  void slotLogin(bool NovoUsuario);

private slots:
  void on_buttonBox_accepted();

private:
  Ui::WhatsProgLogin *ui;

  /// Indica se a tentativa de login eh de um novo usuario (novoUsuario = true)
  /// ou de um usuario existente (novoUsuario = false)
  bool novoUsuario;

signals:
  void signConectar(QString IP, QString login,
                    QString senha, bool novoUsuario);

};

#endif // WHATSPROG_LOGIN_H
