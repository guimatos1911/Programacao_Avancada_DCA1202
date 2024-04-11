#include "incluirlivro.h"
#include "ui_incluirlivro.h"

IncluirLivro::IncluirLivro(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IncluirLivro)
{
    ui->setupUi(this);
}

IncluirLivro::~IncluirLivro()
{
    delete ui;
}

void IncluirLivro::clear(){
    ui -> recebe_nome_livro -> setText("");
    ui -> recebe_preco_livro -> setText("");
    ui -> recebe_autor_livro -> setText("");
}

void IncluirLivro::on_botao_ok_livro_accepted()
{
    QString nome=ui->recebe_nome_livro->text();
    QString preco=ui->recebe_preco_livro->text();
    QString autor=ui->recebe_autor_livro->text();
    emit signIncluirLivro(nome,preco,autor);
}

