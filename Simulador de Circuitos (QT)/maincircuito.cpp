#include "maincircuito.h"
#include "ui_maincircuito.h"
#include "modificarporta.h"
#include "modificarsaida.h"
#include <QStringList>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <time.h>
#include <cmath>
#include <vector>
#include <string>
#include "bool3S.h"

///Dupla Evenly Moreira e Guilherme Matos

MainCircuito::MainCircuito(QWidget *parent) : QMainWindow(parent)
,ui(new Ui::MainCircuito)
,numIn(new QLabel(this))
,numOut(new QLabel(this))
,numPortas(new QLabel(this))
,newCircuito(new NewCircuito(this))
,modificarPorta(new ModificarPorta(this))
,modificarSaida(new ModificarSaida(this))
,C()
{
  ui->setupUi(this);

  // Cabecalhos da tabela de portas
  ui->tablePortas->horizontalHeader()->setVisible(true);
  ui->tablePortas->verticalHeader()->setVisible(true);
  ui->tablePortas->setHorizontalHeaderLabels(QStringList() << "TIPO" << "NUM\nENTR"
                                             << "ENTR\n1" << "ENTR\n2" << "ENTR\n3" << "ENTR\n4");
  ui->tableSaidas->setHorizontalHeaderLabels(QStringList() << "ORIG\nSAIDA");

  // Insere os widgets da barra de status
  statusBar()->insertWidget(0,new QLabel("Num entradas: "));
  statusBar()->insertWidget(1,numIn);
  statusBar()->insertWidget(2,new QLabel("   Num saidas: "));
  statusBar()->insertWidget(3,numOut);
  statusBar()->insertWidget(4,new QLabel("   Num portas: "));
  statusBar()->insertWidget(5,numPortas);

  // Conecta sinais
  // Sinais da janela principal para janela novo circuito
  connect(this, &MainCircuito::signShowNewCircuito,
          newCircuito, &NewCircuito::slotShowNewCircuito);
  // Sinais da janela principal para janela modificar porta
  connect(this, &MainCircuito::signSetRangeInputs,
          modificarPorta, &ModificarPorta::slotSetRangeInputs);
  connect(this, &MainCircuito::signShowModificarPorta,
          modificarPorta, &ModificarPorta::slotShowModificarPorta);
  // Sinais da janela principal para janela modificar saida
  connect(this, &MainCircuito::signSetRangeInputs,
          modificarSaida, &ModificarSaida::slotSetRangeInputs);
  connect(this, &MainCircuito::signShowModificarSaida,
          modificarSaida, &ModificarSaida::slotShowModificarSaida);
  // Sinais da janela novo circuito para janela principal
  connect(newCircuito, &NewCircuito::signNewCircuito,
          this, &MainCircuito::slotNewCircuito);
  // Sinais da janela modificar porta para janela principal
  connect(modificarPorta, &ModificarPorta::signModificarPorta,
          this, &MainCircuito::slotModificarPorta);
  // Sinais da janela modificar saida para janela principal
  connect(modificarSaida, &ModificarSaida::signModificarSaida,
          this, &MainCircuito::slotModificarSaida);

  // Redimensiona todas as tabelas e reexibe os valores da barra de status
  // Essa funcao deve ser chamada sempre que mudar o circuito
  redimensionaTabelas();
}

MainCircuito::~MainCircuito()
{
  delete ui;
}

void MainCircuito::slotNewCircuito(int NInputs, int NOutputs, int NPortas)
{
  // O circuito deve ser criado usando a funcao apropriada da classe Circuito
  C.resize(NInputs, NOutputs, NPortas);
  // Depois do novo circuito criado, as tabelas devem ser redimensionadas
  redimensionaTabelas();
}

void MainCircuito::slotModificarPorta(int IdPort, QString TipoPort, int NumInputsPort,
                                      int IdInput0, int IdInput1, int IdInput2, int IdInput3)
{
  // Aqui deve ser chamado um metodo da classe Circuito que altere a porta cuja
  // id eh IdPort para que ela assuma as caracteristicas especificadas por
  // TipoPort, NumInputsPort
  C.setPort(IdPort, TipoPort.toStdString(),NumInputsPort);
  // Aqui devem ser chamados metodos da classe Circuito que altere a porta cuja
  // id eh IdPort para que as origens de suas entradas sejam dadas pelas ids em IdInput[0-3]
  // O parametro IdInput0 eh sempre levado em conta (toda porta tem ao menos 1 entrada)
  // O parametro IdInput1 soh eh levado em conta se NumInputsPort>=2
  // O parametro IdInput2 soh eh levado em conta se NumInputsPort>=3
  // O parametro IdInput3 soh eh levado em conta se NumInputsPort>=4
  //if (C.validIdPort(IdPort)){
      C.setId_inPort(IdPort, 0, IdInput0);
      if (NumInputsPort >= 2) C.setId_inPort(IdPort, 1, IdInput1);
      if (NumInputsPort >= 3) C.setId_inPort(IdPort, 2, IdInput2);
      if (NumInputsPort >= 4) C.setId_inPort(IdPort, 3, IdInput3);
 // }
  // Depois de alterada, deve ser reexibida a porta correspondente e limpa a tabela verdade
  showPort(IdPort-1);
  limparTabelaVerdade();
}

void MainCircuito::slotModificarSaida(int IdSaida, int IdOrigemSaida)
{
  // Aqui deve ser chamado um metodo da classe Circuito que altere a saida cuja
  // id eh IdSaida para que ela assuma a origem especificada por
  // IdOrigemSaida
  C.setIdOutput(IdSaida, IdOrigemSaida);

  // Depois de alterada, deve ser reexibida a saida correspondente e limpa a tabela verdade
  showOutput(IdSaida-1);
  limparTabelaVerdade();
}

// Redimensiona todas as tabelas e reexibe todos os valores da barra de status
// Essa funcao deve ser chamada sempre que mudar o circuito (criar novo ou ler de arquivo)
void MainCircuito::redimensionaTabelas()
{
  // Esses numeros devem ser inicializados a partir de metodos de consulta da classe Circuito
  int numInputs= C.getNumInputs();
  int numOutputs= C.getNumOutputs();
  int numPorts= C.getNumPorts();

  // Variaveis auxiliares
  //QString texto; -> nao foi utilizada
  QLabel *prov;
  int i;

  // ==========================================================
  // Ajusta os valores da barra de status
  // ==========================================================

  numIn->setNum(numInputs);
  numOut->setNum(numOutputs);
  numPortas->setNum(numPorts);

  // ==========================================================
  // Redimensiona a tabela das portas e conexoes
  // ==========================================================

  // Limpa o conteudo, mas nao os cabecalhos
  ui->tablePortas->clearContents();

  ui->tablePortas->setRowCount(numPorts);
  for (i=0; i<numPorts; i++)
  {
    showPort(i);
  }

  // ==========================================================
  // Redimensiona a tabela de saidas
  // ==========================================================

  // Limpa o conteudo, mas nao os cabecalhos
  ui->tableSaidas->clearContents();

  ui->tableSaidas->setRowCount(numOutputs);
  for (i=0; i<numOutputs; i++)
  {
    showOutput(i);
  }

  // ==========================================================
  // Redimensiona a tabela verdade
  // ==========================================================

  // Antes de redimensionar a tabela, anula todos os setSpan da primeira linha
  // Se nao fizer isso, dah erro ao redimensionar
  ui->tableTabelaVerdade->clearSpans();

  // Limpa todo o conteudo, inclusive cabecalhos
  ui->tableTabelaVerdade->clear();

  // Modifica as dimensoes
  ui->tableTabelaVerdade->setColumnCount(numInputs+numOutputs);
  int numCombinacoesEntrada = (numInputs>0 ? round(pow(3,numInputs)) : 0);
  // a tabela verdade tem uma linha a mais do que o numero de combinacoes de entrada
  // para conter o pseudocabecalho
  ui->tableTabelaVerdade->setRowCount(1 + numCombinacoesEntrada);

  // Cria o pseudocabecalho na primeira linha
  // Entradas
  if (numInputs > 0)
  {
    if (numInputs > 1) ui->tableTabelaVerdade->setSpan(0,0,1,numInputs);
    prov = new QLabel("ENTRADAS");
    prov->setAlignment(Qt::AlignCenter);
    prov->setStyleSheet("font-weight: bold");
    ui->tableTabelaVerdade->setCellWidget(0,0,prov);
  }
  // Saidas
  if (numOutputs > 0)
  {
    if (numOutputs > 1) ui->tableTabelaVerdade->setSpan(0,numInputs,1,numOutputs);
    prov = new QLabel("SAIDAS");
    prov->setAlignment(Qt::AlignCenter);
    prov->setStyleSheet("font-weight: bold");
    ui->tableTabelaVerdade->setCellWidget(0,numInputs,prov);
  }

  // ==========================================================
  // Fixa os limites para os spin boxs (emit signSetRangeInputs)
  // de entradas de portas (classe ModificarPorta) e
  // de origem das saidas (classe ModificarSaida)
  // emitindo um sinal que serah recebido por slots dessas classes
  // ==========================================================

  emit signSetRangeInputs(-numInputs, numPorts);
}

// Exibe os dados da i-esima porta (porta cuja id eh i+1)
// Essa funcao deve ser chamada sempre que mudar caracteristicas da porta
// A funcao redimensiona_tabela jah chama essa funcao para todas as portas
void MainCircuito::showPort(unsigned i)
{
  // Testa se indice i eh valido, usando uma funcao de teste da classe Circuito:
  // se i+1 eh uma id valida para uma porta

  // Provisoriamente, o teste eh sempre falso
  bool indice_valido=C.validIdPort(i+1);
  if (!indice_valido) return;  // Encerra a funcao sem fazer nada

  // Esses valores (namePort, numInputsPort)
  // devem ser lidos a partir de metodos de consulta da classe Circuito
  // Provisoriamente, estao sendo inicializados com valores nulos ou invalidos
  QString namePort=QString::fromStdString(C.getNamePort(i+1));
  int numInputsPort=C.getNumInputsPort(i+1);

  // Variaveis auxiliares
  QLabel *prov;
  int j;

  // As id das entradas da porta
  int idInputPort[4];
  // Esses valores (idInputPorta[])
  // devem ser lidos a partir de metodos de consulta da classe Circuito
  // Provisoriamente, estao sendo inicializados com valores nulos
  for (j=0; j<numInputsPort; j++)
  {
    idInputPort[j] =C.getId_inPort(i + 1, j);
  }

  // Cria e define valores dos widgets da linha da tabela que corresponde aa porta

  // Coluna 0
  prov = new QLabel(namePort);
  prov->setAlignment(Qt::AlignCenter);
  ui->tablePortas->setCellWidget(i,0,prov);
  // Coluna 1
  prov = new QLabel;
  prov->setAlignment(Qt::AlignCenter);
  prov->setNum(numInputsPort);
  ui->tablePortas->setCellWidget(i,1,prov);

  // As entradas de cada porta
  for (j=0; j<4; j++)
  {
    // Cria os widgets das celulas da tabela de portas
    // Coluna 2 em diante
    prov = new QLabel;
    prov->setAlignment(Qt::AlignCenter);
    if (j<numInputsPort) prov->setNum(idInputPort[j]);
    ui->tablePortas->setCellWidget(i,2+j,prov);
  }
}

// Exibe os dados da i-esima saida (saida cuja id eh i+1)
// Essa funcao deve ser chamada sempre que mudar valores da saida
// A funcao redimensiona_tabela jah chama essa funcao para todas as saidas
void MainCircuito::showOutput(unsigned i)
{
  // Testa se indice i eh valido, usando uma funcao de teste da classe Circuito:
  // se i+1 eh uma id valida para uma saida

  // Provisoriamente, o teste eh sempre falso
  bool indice_valido=C.validIdOutput(i + 1);
  if (!indice_valido) return;  // Encerra a funcao sem fazer nada

  // Esse valor (idOutput) deve ser lido a partir de metodos de consulta da classe Circuito
  // Provisoriamente, estah sendo inicializado com valor nulo
  int idOutput=C.getIdOutput(i+1);

  // Variavel auxiliar
  QLabel *prov;

  // Cria o widget da celula da tabela de saidas
  // Coluna 0 (unica)
  prov = new QLabel;
  prov->setAlignment(Qt::AlignCenter);
  prov->setNum(idOutput);
  ui->tableSaidas->setCellWidget(i,0,prov);
}

// Limpa a tabela verdade
// Deve ser chamada sempre que alguma caracteristica do circuito (porta, saida) for alterada
void MainCircuito::limparTabelaVerdade()
{
  // Esses valores (numInputs, numOutputs)
  // devem ser lidos a partir de metodos de consulta da classe Circuito

  // Provisoriamente, estao sendo inicializados com valores nulos
  int numInputs=C.getNumInputs();
  int numOutputs=C.getNumOutputs();

  // Variavel auxiliar
  QLabel *prov;

  // Limpa todo o conteudo, inclusive cabecalhos
  ui->tableTabelaVerdade->clear();

  // Recoloca os textos dos cabecalhos na primeira linha
  // Entradas
  if (numInputs > 0)
  {
    prov = new QLabel("ENTRADAS");
    prov->setAlignment(Qt::AlignCenter);
    prov->setStyleSheet("font-weight: bold");
    ui->tableTabelaVerdade->setCellWidget(0,0,prov);
  }
  // Saidas
  if (numOutputs > 0)
  {
    prov = new QLabel("SAIDAS");
    prov->setAlignment(Qt::AlignCenter);
    prov->setStyleSheet("font-weight: bold");
    ui->tableTabelaVerdade->setCellWidget(0,numInputs,prov);
  }
}

void MainCircuito::on_actionSair_triggered()
{
  QCoreApplication::quit();
}

// Exibe a caixa de dialogo para fixar caracteristicas de um novo circuito
void MainCircuito::on_actionNovo_triggered()
{
  emit signShowNewCircuito();
}

// Abre uma caixa de dialogo para ler um arquivo
void MainCircuito::on_actionLer_triggered()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Arquivo de circuito"), "../Circuito",
                                                  tr("Circuitos (*.txt);;Todos (*.*)"));

  if (!fileName.isEmpty()) {
    // Leh o circuito do arquivo com nome "fileName", usando a funcao apropriada da classe Circuito
    // e testa se a leitura deu certo
    // Provisoriamente, o teste eh sempre falso
    bool leitura_OK = C.ler(fileName.toStdString());
    if (!leitura_OK)
    {
      // Exibe uma msg de erro na leitura
      QMessageBox msgBox;
      msgBox.setText("Erro ao ler um circuito a partir do arquivo:\n"+fileName);
      msgBox.exec();
    }

    // Feita a leitura, reexibe todas as tabelas
    redimensionaTabelas();
  }
}

// Abre uma caixa de dialogo para salvar um arquivo
void MainCircuito::on_actionSalvar_triggered()
{
  // Soh pode salvar se o Circuito for valido

  // Provisoriamente, o teste eh sempre falso
  bool circuito_valido = C.valid();
  if (!circuito_valido)
  {
    QMessageBox msgBox;
    msgBox.setText("O Circuito nao esta completamente definido.\nNao pode ser salvo.");
    msgBox.exec();
    return;
  }

  QString fileName = QFileDialog::getSaveFileName(this, tr("Arquivo de circuito"), "../Circuito",
                                                  tr("Circuitos (*.txt);;Todos (*.*)"));

  if (!fileName.isEmpty()) {
    // Salva o circuito no arquivo com nome "fileName", usando a funcao apropriada da classe Circuito

    // Provisoriamente, o teste eh sempre falso
    bool escrita_OK = C.salvar(fileName.toStdString());
    if (!escrita_OK)
    {
      // Exibe uma msg de erro na leitura
      QMessageBox msgBox;
      msgBox.setText("Erro ao salvar o circuito no arquivo:\n"+fileName);
      msgBox.exec();
    }
  }
}

// Gera e exibe a tabela verdade para o circuito
// Chama a funcao simular da classe circuito
void MainCircuito::on_actionGerar_tabela_triggered()
{
  // Soh pode simular se o Circuito for valido
  // Provisoriamente, o teste eh sempre falso
  bool circuito_valido = C.valid();
  if (!circuito_valido)
  {
    QMessageBox msgBox;
    msgBox.setText("O Circuito nao esta completamente definido.\nNao pode ser simulado.");
    msgBox.exec();
    return;
  }

  //
  // Cria e inicializa o vetor de entradas do circuito
  //

  // Esses valores (numInputs, numOutputs)
  // devem ser lidos a partir de metodos de consulta da classe Circuito

  // Provisoriamente, estao sendo inicializados com valores nulos
  int numInputs=C.getNumInputs();
  int numOutputs=C.getNumOutputs();
  // Calcula o numero de combinacoes de entrada
  int numCombinacoesEntrada = (numInputs>0 ? round(pow(3,numInputs)) : 0);

  // As entradas do circuito
  std::vector<bool3S> in_circ(numInputs);    // bool3S::UNDEF; bool3S::FALSE; bool3S::TRUE

  // Variaveis auxiliares
  QLabel *prov;
  int i,j;

  // Inicializa todas as entradas com bool3S::UNDEF
  for (i=0; i<numInputs; i++) in_circ[i] = bool3S::UNDEF;

  //
  // Gera todas as combinacoes de entrada e as linhas correspondentes da tabela verdade
  //
  for (i=0; i<numCombinacoesEntrada; i++)
  {
    //
    // Exibe a i-esima combina��o de entrada
    //

    // Cria os QLabels correspondentes aas entradas in_circ[j]
    for (j=0; j<numInputs; j++)
    {
      prov = new QLabel( QString( toChar(in_circ[j]) ) );
      prov->setAlignment(Qt::AlignCenter);
      ui->tableTabelaVerdade->setCellWidget(i+1, j, prov);
    }

    //
    // Simula a saida para a i-esima combinacao de entrada
    //

    // Chama o metodo de simulacao da classe Circuito
    C.simular(in_circ);

    //
    // Exibe a saida correspondente aa i-esima combinacao de entrada
    //

    // Cria os QLabels correspondentes aas saidas outputs[j]
    for (j=0; j<numOutputs; j++)
    {
      // A saida deve ser obtida com um metodo de consulta da classe Circuito:
      // linha j da tabela deve exibir o valor da saida do circuito cuja id = j+1
      // Provisoriamente, estah sendo inicializada sempre com valor UNDEF
      bool3S output = C.getOutput(j+1);
      prov = new QLabel( QString( toChar(output) ) );
      prov->setAlignment(Qt::AlignCenter);
      ui->tableTabelaVerdade->setCellWidget(i+1, j+numInputs, prov);
    }

    // Gera a proxima combinacao de entrada

    // Incrementa a ultima entrada que nao for TRUE
    // Se a ultima for TRUE, faz essa ser UNDEF e tenta incrementar a anterior
    j = numInputs-1;
    while (in_circ[j]==bool3S::TRUE && j>=0)
    {
      in_circ[j] = bool3S::UNDEF;
      j--;
    };
    // Incrementa a input selecionada
    if (j>=0) in_circ[j]++;
  }

}

// Exibe a caixa de dialogo para fixar caracteristicas de uma porta
void MainCircuito::on_tablePortas_activated(const QModelIndex &index)
{
  // Converte de indice de linha para Id de porta
  int idPort = index.row()+1;

  // Esses valores (namePort, numInputsPort)
  // devem ser lidos a partir de metodos de consulta da classe Circuito

  // Provisoriamente, estao sendo inicializados com valores invalidos ou nulos
  QString namePort = QString::fromStdString(C.getNamePort(idPort));
  int numInputsPort = C.getNumInputsPort(idPort);

  // As id das entradas da porta
  int idInputPort[4];
  // Esses valores (idInputPorta[])
  // devem ser lidos a partir de metodos de consulta da classe Circuito

  // Provisoriamente, estao sendo inicializados com valores nulos
  for (int j=0; j<numInputsPort; j++)
  {
    idInputPort[j] = C.getId_inPort(idPort, j);
  }

  // Informa as caracteristicas atuais da porta (emit signShowModificarPorta)
  emit signShowModificarPorta(idPort, namePort, numInputsPort,
                              idInputPort[0],idInputPort[1],idInputPort[2],idInputPort[3]);
}

// Exibe a caixa de dialogo para fixar caracteristicas de uma saida
void MainCircuito::on_tableSaidas_activated(const QModelIndex &index)
{
  // Converte de indice de linha para Id de porta
  int idSaida = index.row()+1;

  // Esse valor (idOrigemSaida)
  // deve ser lido a partir de metodo de consulta da classe Circuito

  // Provisoriamente, estah sendo inicializado com valor nulo
  int idOrigemSaida = C.getIdOutput(idSaida);

  // Informa as caracteristicas atuais da saida (emit signShowModificarSaida)
  emit signShowModificarSaida(idSaida, idOrigemSaida);
}
