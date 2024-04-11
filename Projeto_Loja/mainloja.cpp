#include "mainloja.h"
#include "ui_mainloja.h"

#include "loja.h"
#include "loja.cpp"
#include "incluircd.h"
#include "incluirdvd.h"
#include "incluirlivro.h"

#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <string>

using namespace std;

MainLoja::MainLoja(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainLoja)
    , X()
    , inclLivro(new IncluirLivro(this))
    , inclCD(new IncluirCD(this))
    , inclDVD(new IncluirDVD(this))
    , total_itens (new QLabel ("0", this))

{
    ui->setupUi(this);

    statusBar()->insertWidget(0,new QLabel("Total de itens: "));
    statusBar()->insertWidget(1, total_itens);

    connect(inclLivro, SIGNAL(signIncluirLivro(QString,QString,QString)),this,SLOT(slotIncluirLivro(QString,QString,QString)));
    connect(inclCD, SIGNAL(signIncluirCD(QString,QString,QString)),this,SLOT(slotIncluirCD(QString,QString,QString)));
    connect(inclDVD, SIGNAL(signIncluirDVD(QString,QString,QString)),this,SLOT(slotIncluirDVD(QString,QString,QString)));

    ///ESTILO
    //cores dos qlabels
    ui->rot_livro->setStyleSheet("background-color: lightgray;");
    ui->rot_cd->setStyleSheet("background-color: lightgray;");
    ui->rot_dvd->setStyleSheet("background-color: lightgray;");

    //rótulos widgets
    ui->wid_livro->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: lightgray; }");
    ui->wid_cd->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: lightgray; }");
    ui->wid_dvd->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: lightgray; }");

    //ajustes tabelas/colunas
    ui->wid_livro->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->wid_livro->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->wid_livro->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    ui->wid_cd->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->wid_cd->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->wid_cd->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    ui->wid_dvd->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->wid_dvd->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->wid_dvd->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    ui->wid_livro->setHorizontalHeaderLabels(QStringList() << "NOME" << "PRECO" << "AUTOR");
    ui->wid_cd->setHorizontalHeaderLabels(QStringList() << "NOME" << "PRECO" << "N FAIXAS");
    ui->wid_dvd->setHorizontalHeaderLabels(QStringList() << "NOME" << "PRECO" << "DURACAO");
}

MainLoja::~MainLoja(){delete ui;}

void MainLoja::TotalDeItens()
{
    int tItens = X.getNumLivro() + X.getNumCD() + X.getNumDVD();
    total_itens->setText(QString::number(tItens));
}
//slots
void MainLoja::slotIncluirLivro(QString nome, QString preco, QString autor){
    string novo_nome = nome.toStdString();
    int novo_preco = round(preco.toFloat()*100);
    string novo_autor = autor.toStdString();
    if (novo_nome == "" || novo_preco <= 0 || novo_autor == ""){
        QMessageBox::critical(this,"Livro inválido", "Não foi possível incluir o livro:\nNome="+nome+"\nPreço="+preco+"\nAutor="+autor);
    }else{
        Livro L(novo_nome, novo_preco/100.0, novo_autor);
        X.incluirLivro(L);
        mostrar_livro();
        TotalDeItens();
    }
}

void MainLoja::slotIncluirCD(QString nome, QString preco, QString numfaixas){
    string novo_nome = nome.toStdString();
    int novo_preco = round(preco.toFloat()*100);
    int novo_faixas = numfaixas.toInt();
    if (novo_nome == "" || novo_preco <= 0 || novo_faixas <= 0){
        QMessageBox::critical(this,"CD inválido", "Não foi possível incluir o CD:\nNome="+nome+"\nPreço="+preco+"\nNum faixas="+numfaixas);
    }else{
        CD cd(novo_nome, novo_preco/100.0, novo_faixas);
        X.incluirCD(cd);
        mostrar_cd();
        TotalDeItens();
    }
}

void MainLoja::slotIncluirDVD(QString nome, QString preco, QString duracao){
    string novo_nome = nome.toStdString();
    int novo_preco = round(preco.toFloat()*100);
    int nova_duracao = round(duracao.toInt());
    if (novo_nome == "" || novo_preco <= 0 || nova_duracao <= 0){
        QMessageBox::critical(this,"DVD inválido", "Não foi possível incluir o DVD:\nNome="+nome+"\nPreço="+preco+"\nDuraçãor="+duracao);
    }else{
        DVD dvd(novo_nome, novo_preco/100.0, nova_duracao);
        X.incluirDVD(dvd);
        mostrar_dvd();
        TotalDeItens();
    }
}


//tratamento das listas
void MainLoja::on_wid_livro_cellDoubleClicked(int row, int /*column*/)
{
    if(row >= 0 && row < X.getNumLivro()){
        X.excluirLivro(row);
        mostrar_livro();
        TotalDeItens();
    }
}

void MainLoja::on_wid_cd_cellDoubleClicked(int row, int /*column*/)
{
    if(row >= 0 && row < X.getNumCD()){
        X.excluirCD(row);
        mostrar_cd();
        TotalDeItens();
    }
}

void MainLoja::on_wid_dvd_cellDoubleClicked(int row, int /*column*/)
{
    if(row >= 0 && row < X.getNumDVD()){
        X.excluirDVD(row);
        mostrar_dvd();
        TotalDeItens();
    }
}

void MainLoja::mostrar_livro()
{
    ui -> wid_livro -> clearContents();
    int num_livros = X.getNumLivro();
    ui-> wid_livro -> setRowCount(num_livros);
    for (int i = 0; i <= num_livros-1; i++){
        Livro livro = X.getLivro(i);
        QLabel* l_nome(new QLabel(livro.getNome().c_str()));
        QLabel* l_preco(new QLabel(QString::number(livro.getPreco(), 'f', 2)));
        QLabel* l_autor(new QLabel(livro.getAutor().c_str()));

        l_nome -> setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        ui -> wid_livro -> setCellWidget(i, 0, l_nome);
        l_preco -> setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        ui -> wid_livro -> setCellWidget(i, 1, l_preco);
        l_autor -> setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        ui -> wid_livro -> setCellWidget(i, 2, l_autor);
    }
}

void MainLoja::mostrar_cd()
{
    ui -> wid_cd -> clearContents();
    int num_cds = X.getNumCD();
    ui-> wid_cd -> setRowCount(num_cds);
    for (int i = 0; i <= num_cds-1; i++){
        CD cd = X.getCD(i);
        QLabel* l_nome(new QLabel(cd.getNome().c_str()));
        QLabel* l_preco(new QLabel(QString::number(cd.getPreco(), 'f', 2)));
        QLabel* l_numFaixas(new QLabel(QString::number(cd.getNumFaixas())));

        l_nome -> setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        ui -> wid_cd -> setCellWidget(i, 0, l_nome);
        l_preco -> setAlignment(Qt::AlignRight|Qt::AlignVCenter);
        ui -> wid_cd -> setCellWidget(i, 1, l_preco);
        l_numFaixas -> setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        ui -> wid_cd -> setCellWidget(i, 2, l_numFaixas);
    }
}

void MainLoja::mostrar_dvd()
{
    ui -> wid_dvd -> clearContents();
    int num_dvds = X.getNumDVD();
    ui-> wid_dvd -> setRowCount(num_dvds);
    for (int i = 0; i <= num_dvds-1; i++){
        DVD dvd = X.getDVD(i);
        QLabel* l_nome(new QLabel(dvd.getNome().c_str()));
        QLabel* l_preco(new QLabel(QString::number(dvd.getPreco(), 'f', 2)));
        QLabel* l_duracao(new QLabel(QString::number(dvd.getDuracao())));

        l_nome -> setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
         ui -> wid_dvd -> setCellWidget(i, 0, l_nome);
        l_preco -> setAlignment(Qt::AlignRight|Qt::AlignVCenter);
         ui -> wid_dvd -> setCellWidget(i, 1, l_preco);
        l_duracao -> setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        ui -> wid_dvd -> setCellWidget(i, 2, l_duracao);
    }
}

//janela 'arquivo'
void MainLoja::on_actionLer_triggered()
{
    QString arquivo = QFileDialog::getOpenFileName(this, "Ler estoque", QString(),"*.txt");
    string novo_arquivo = arquivo.toStdString();
    if(!arquivo.isEmpty()){
        if(!X.ler(novo_arquivo)) QMessageBox::critical(this, tr("Erro"), tr("Não foi possível executar a leitura do arquivo."));
    }
    mostrar_livro();
    mostrar_cd();
    mostrar_dvd();
    TotalDeItens();
}

void MainLoja::on_actionSalvar_triggered()
{
    QString arquivo = QFileDialog::getSaveFileName(this, "Salvar estoque", QString(), "*.txt");
    if (arquivo.isEmpty()) return;
    string novo_arquivo = arquivo.toStdString();
    if(!X.salvar(novo_arquivo))QMessageBox::critical(this, tr("Erro"), tr("Não foi possível executar a escrita do arquivo."));
}

void MainLoja::on_actionSair_triggered()
{
    QCoreApplication::quit();
}

//janela 'item'
void MainLoja::on_actionIncluir_Livro_triggered()
{
    inclLivro -> clear();
    inclLivro -> show();
}

void MainLoja::on_actionIncluir_CD_triggered()
{
    inclCD -> clear();
    inclCD -> show();
}

void MainLoja::on_actionIncluir_DVD_triggered()
{
    inclDVD -> clear();
    inclDVD -> show();
}
