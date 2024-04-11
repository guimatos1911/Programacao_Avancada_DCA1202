#ifndef MAINLOJA_H
#define MAINLOJA_H

#include <QMainWindow>
#include <QLabel>

#include "loja.h"
#include "incluirlivro.h"
#include "incluirdvd.h"
#include "incluircd.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainLoja; }
QT_END_NAMESPACE

class MainLoja : public QMainWindow
{
    Q_OBJECT

public:
    MainLoja(QWidget *parent = nullptr);
    ~MainLoja();

private slots:
    void TotalDeItens();

    //tratamento das colunas
    void slotIncluirLivro(QString nome, QString preco, QString autor);
    void slotIncluirCD(QString nome, QString preco, QString numfaixas);
    void slotIncluirDVD(QString nome, QString preco, QString duracao);
    void on_wid_livro_cellDoubleClicked(int row, int /*column*/);
    void on_wid_cd_cellDoubleClicked(int row, int /*column*/);
    void on_wid_dvd_cellDoubleClicked(int row, int /*column*/);
    void mostrar_livro();
    void mostrar_cd();
    void mostrar_dvd();

    //janela 'arquivo'
    void on_actionLer_triggered();
    void on_actionSalvar_triggered();
    void on_actionSair_triggered();

    //janela 'item'
    void on_actionIncluir_Livro_triggered();
    void on_actionIncluir_CD_triggered();
    void on_actionIncluir_DVD_triggered();

private:
    Ui::MainLoja *ui;
    Loja X;
    IncluirLivro* inclLivro;
    IncluirCD* inclCD;
    IncluirDVD* inclDVD;
    QLabel* total_itens;
};
#endif // MAINLOJA_H
