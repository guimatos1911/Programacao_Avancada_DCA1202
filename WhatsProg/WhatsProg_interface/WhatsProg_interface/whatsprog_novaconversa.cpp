#include <QString>
#include <string>
#include <QMessageBox>
#include "whatsprog_novaconversa.h"
#include "ui_whatsprog_novaconversa.h"
#include "whatsprog_dados_cliente.h"

extern WhatsProgDadosCliente DC;

WhatsProgNovaConversa::WhatsProgNovaConversa(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::WhatsProgNovaConversa)
{
  ui->setupUi(this);
}

WhatsProgNovaConversa::~WhatsProgNovaConversa()
{
  delete ui;
}

void WhatsProgNovaConversa::on_buttonBox_accepted()
{
  // Recupera e testa nome da nova conversa
  QString nomeConversa = ui->lineEditConversa->text();
  if (nomeConversa.isEmpty()){
      //QMessageBox::warning(this, "Aviso", "O nome da conversa não pode estar vazio");
      return;
  }
  // Insere nova conversa na base de dados
  if(!DC.insertConversa(nomeConversa.toStdString())){
      //QMessageBox::critical (this, "Erro", "Erro ao inserir nova conversa.");
      return;
  }
  // Faz a conversa recem inserida (a ultima) ser a conversa ativa
  DC.setConversaAtual(nomeConversa.toStdString());
  // Sinaliza que houve alteracao na janelas: Conversas e Mensagens
  emit signAtualizaConversas();
}
