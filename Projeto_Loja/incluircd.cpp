#include "incluircd.h"
#include "ui_incluircd.h"

IncluirCD::IncluirCD(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IncluirCD)
{
    ui->setupUi(this);
}

IncluirCD::~IncluirCD()
{
    delete ui;
}

void IncluirCD::clear(){
    ui -> recebe_nome_cd -> setText("");
    ui -> recebe_preco_cd -> setText("");
    ui -> recebe_nfaixas_cd -> setText("");
}

void IncluirCD::on_botao_ok_cd_accepted()
{
    QString nome=ui->recebe_nome_cd->text();
    QString preco=ui->recebe_preco_cd->text();
    QString nFaixas=ui->recebe_nfaixas_cd->text();
    emit signIncluirCD(nome,preco,nFaixas);
}

