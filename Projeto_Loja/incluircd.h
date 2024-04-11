#ifndef INCLUIRCD_H
#define INCLUIRCD_H

#include <QDialog>

namespace Ui {
class IncluirCD;
}

class IncluirCD : public QDialog
{
    Q_OBJECT

public:
    explicit IncluirCD(QWidget *parent = nullptr);
    ~IncluirCD();
    void clear();

signals:
    void signIncluirCD(QString nome, QString preco, QString nFaixas);

private slots:
    void on_botao_ok_cd_accepted();

private:
    Ui::IncluirCD *ui;
};

#endif // INCLUIRCD_H
