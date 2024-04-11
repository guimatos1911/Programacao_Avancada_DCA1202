#include "incluirdvd.h"
#include "ui_incluirdvd.h"

IncluirDVD::IncluirDVD(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IncluirDVD)
{
    ui->setupUi(this);
}

IncluirDVD::~IncluirDVD()
{
    delete ui;
}

void IncluirDVD::clear(){
    ui -> recebe_nome_dvd -> setText("");
    ui -> recebe_preco_dvd -> setText("");
    ui -> recebe_duracao_dvd -> setText("");
}

void IncluirDVD::on_botao_ok_dvd_accepted()
{
    QString nome=ui->recebe_nome_dvd->text();
    QString preco=ui->recebe_preco_dvd->text();
    QString duracao=ui->recebe_duracao_dvd->text();
    emit signIncluirDVD(nome,preco,duracao);
}

