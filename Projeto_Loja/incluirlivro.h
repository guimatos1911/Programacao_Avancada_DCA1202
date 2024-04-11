#ifndef INCLUIRLIVRO_H
#define INCLUIRLIVRO_H

#include <QDialog>

namespace Ui {
class IncluirLivro;
}

class IncluirLivro : public QDialog
{
    Q_OBJECT
public:
    explicit IncluirLivro(QWidget *parent = nullptr);
    ~IncluirLivro();
    void clear();

private slots:
    void on_botao_ok_livro_accepted();

signals:
    void signIncluirLivro(QString nome, QString preco, QString autor);

private:
    Ui::IncluirLivro *ui;
};

#endif // INCLUIRLIVRO_H
