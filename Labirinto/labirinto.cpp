#include <iomanip>
#include <fstream>
#include <list>
#include <cmath> // para sqrt
#include <algorithm>

#include "labirinto.h"
#include "coord.h"

using namespace std;

/* ***************** */
/* CLASSE CELULA     */
/* ***************** */

string estadoCel2string(EstadoCel E)
{
  switch(E)
  {
  case EstadoCel::LIVRE:
    return "  ";
  case EstadoCel::OBSTACULO:
    return "##";
  case EstadoCel::ORIGEM:
    return "Or";
  case EstadoCel::DESTINO:
    return "De";
  case EstadoCel::CAMINHO:
    return "..";
  case EstadoCel::INVALIDO:
  default:
    break;
  }
  return "??";
}

/* ***************** */
/* CLASSE LABIRINTO  */
/* ***************** */

/// Construtores

/// Torna o mapa vazio
void Labirinto::clear()
{
  // Esvazia o mapa de qualquer conteudo anterior
  NLin = NCol = 0;
  mapa.clear();
  // Apaga a origem e destino do caminho
  orig = dest = Coord();
}

/// Limpa o caminho anterior
void Labirinto::limpaCaminho()
{
  if (!empty()) for (int i=0; i<NLin; ++i) for (int j=0; j<NCol; ++j)
      {
        if (at(i,j) == EstadoCel::CAMINHO) set(i,j, EstadoCel::LIVRE);
      }
}

/// Retorna o estado da celula correspondente ao i-j-esimo elemento do mapa
EstadoCel Labirinto::at(int i, int j) const
{
  if (i<0 || i>=NLin || j<0 || j>=NCol)
  {
    cerr << "Coordenadas invalidas para o labirinto" << endl;
    return EstadoCel::INVALIDO;
  }
  return mapa.at(NCol*i+j);
}

/// Funcao set de alteracao de valor
void Labirinto::set(int i, int j, EstadoCel valor)
{
  if (i<0 || i>=NLin || j<0 || j>=NCol)
  {
    cerr << "Coordenadas invalidas para o labirinto" << endl;
    return;
  }
  if (valor == EstadoCel::INVALIDO)
  {
    cerr << "Valor invalido para celula" << endl;
    return;
  }
  mapa.at(NCol*i+j) = valor;
}

/// Testa se uma coordenada de celula eh valida para os limites de um mapa
bool Labirinto::coordValida(const Coord& C) const
{
  if (C.lin<0 || C.lin>=NLin || C.col<0 || C.col>=NCol) return false;
  return true;
}

/// Testa se uma celula eh valida e estah livre (nao eh obstaculo) em um mapa
bool Labirinto::celulaValidaLivre(const Coord& C) const
{
  if (!coordValida(C)) return false;
  if (at(C) == EstadoCel::OBSTACULO) return false;
  return true;
}

/// Testa se um movimento MovDe->MovPara eh valido
bool Labirinto::movimentoValido(const Coord& MovDe, const Coord& MovPara) const
{
  // Soh pode mover de e para celulas validas e livres
  if (!celulaValidaLivre(MovDe)) return false;
  if (!celulaValidaLivre(MovPara)) return false;

  // Soh pode mover para celulas vizinhas, ou seja, a diferenca absoluta
  // na coordenada tanto da linha quanto da coluna eh no maximo 1
  Coord delta=abs(MovPara-MovDe);
  if (delta.lin>1 || delta.col>1) return false;

  // Nao pode mover em diagonal se colidir com alguma quina
  // Se o movimento nao for diagonal, esses testes sempre dao certo,
  // pois jah testou que MovDe e MovPara estao livres e ou a linha ou a
  // coluna de MovDe e MovPara sao iguais
  if (!celulaValidaLivre(Coord(MovDe.lin,MovPara.col))) return false;
  if (!celulaValidaLivre(Coord(MovPara.lin,MovDe.col))) return false;

  // Movimento valido
  return true;
}

/// Fixa a origem do caminho a ser encontrado
bool Labirinto::setOrigem(const Coord& C)
{
  if (!celulaValidaLivre(C)) return false;
  // Se for a mesma origen nao faz nada
  if (C==orig) return true;

  limpaCaminho();

  // Apaga a origem anterior no mapa, caso esteja definida
  if (coordValida(orig)) set(orig, EstadoCel::LIVRE);

  // Fixa a nova origem
  orig = C;
  // Marca a nova origem no mapa
  set(orig, EstadoCel::ORIGEM);

  return true;
}

/// Fixa o destino do caminho a ser encontrado
bool Labirinto::setDestino(const Coord& C)
{
  if (!celulaValidaLivre(C)) return false;
  // Se for o mesmo destino nao faz nada
  if (C==dest) return true;

  limpaCaminho();

  // Apaga o destino anterior no mapa, caso esteja definido
  if (coordValida(dest)) set(dest, EstadoCel::LIVRE);

  // Fixa o novo destino
  dest = C;
  // Marca o novo destino no mapa
  set(dest, EstadoCel::DESTINO);

  return true;
}

/// Imprime o mapa no console
void Labirinto::imprimir() const
{
  if (empty())
  {
    cout << "+------------+" << endl;
    cout << "| MAPA VAZIO |" << endl;
    cout << "+------------+" << endl;
    return;
  }

  int i,j;

  // Impressao do cabecalho
  cout << "    ";
  for (j=0; j<NCol; j++)
  {
    cout << setfill('0') << setw(2) << j << setfill(' ') << setw(0) << ' ' ;
  }
  cout << endl;

  cout << "   +";
  for (j=0; j<NCol; j++) cout << "--+" ;
  cout << endl;

  // Imprime as linhas
  for (i=0; i<NLin; i++)
  {
    cout << setfill('0') << setw(2) << i << setfill(' ') << setw(0) << " |" ;
    for (j=0; j<NCol; j++)
    {
      cout << estadoCel2string(at(i,j)) << '|' ;
    }
    cout << endl;

    cout << "   +";
    for (j=0; j<NCol; j++) cout << "--+" ;
    cout << endl;
  }
}

/// Leh um mapa do arquivo nome_arq
/// Caso nao consiga ler do arquivo, cria mapa vazio
/// Retorna true em caso de leitura bem sucedida
bool Labirinto::ler(const string& nome_arq)
{
  // Limpa o mapa
  clear();

  // Abre o arquivo
  ifstream arq(nome_arq);

  // Resultado logico da leitura
  bool resultado=true;

  try
  {
    if (!arq.is_open()) throw 1;

    string prov;
    int numL, numC;
    int valor;

    // Leh o cabecalho
    arq >> prov >> numL >> numC;
    if (!arq.good() || prov != "LABIRINTO" ||
        numL<=0 || numC<=0 ) throw 2;

    // Redimensiona o mapa
    NLin = numL;
    NCol = numC;
    mapa.resize(NLin*NCol);

    // Leh as celulas do arquivo
    for (int i=0; i<NLin; i++)
      for (int j=0; j<NCol; j++)
      {
        arq >> valor;
        if (!arq.good()) throw 3;

        if (valor == 0) set(i,j, EstadoCel::OBSTACULO);
        else set(i,j, EstadoCel::LIVRE);
      }
  }
  catch (int i)
  {
    string msg;
    switch(i)
    {
      case 1: msg = "Erro na abertura"; break;
      case 2: msg = "Dimensoes incompativeis"; break;
      case 3: msg = "Erro na leitura de celula"; break;
      default: msg = "Erro desconhecido"; break;
    }
    cerr << "Arquivo " << nome_arq << " - " << msg << endl;
    resultado = false;
  }
  return resultado;
}

/// Salva um mapa no arquivo nome_arq
/// Retorna true em caso de escrita bem sucedida
bool Labirinto::salvar(const string& nome_arq) const
{
  // Testa o mapa
  if (empty()) return false;

  // Abre o arquivo
  ofstream arq(nome_arq);
  if (!arq.is_open())
  {
    return false;
  }

  // Salva o cabecalho
  arq << "LABIRINTO " << NLin << ' ' << NCol << endl;

  // Salva as celulas do mapa
  for (int i=0; i<NLin; i++)
  {
    for (int j=0; j<NCol; j++)
    {
      if (at(i,j) == EstadoCel::OBSTACULO) arq << 0;
      else arq << 1;
      arq << ' ';
    }
    arq << endl;
  }

  return true;
}

/// Gera um novo mapa aleatorio
/// numL e numC sao as dimensoes do labirinto
/// perc_obst eh o percentual de casas ocupadas (obstaculos) no mapa.
void Labirinto::gerar(int numL, int numC, double perc_obst)
{
  // Testa os parametros
  if (numL<=0) numL = ALTURA_PADRAO_MAPA;
  if (numC<=0) numC = LARGURA_PADRAO_MAPA;
  if (perc_obst <= 0.0 || perc_obst >= 1.0) perc_obst = PERC_PADRAO_OBST;

  // Limpa o mapa
  clear();

  // Inicializa a semente de geracao de numeros aleatorios
  srand(time(nullptr));

  // Assume as dimensoes passadas como parametro
  NLin = numL;
  NCol = numC;

  // Redimensiona o mapa
  mapa.resize(NLin*NCol);

  // Preenche o mapa
  bool obstaculo;
  for (int i=0; i<NLin; i++) for (int j=0; j<NCol; j++)
    {
      obstaculo = (rand()/double(RAND_MAX) <= perc_obst);
      if (obstaculo) set(i,j, EstadoCel::OBSTACULO);
      else set(i,j, EstadoCel::LIVRE);
    }
}

/// *******************************************************************************
/// Calcula o caminho entre a origem e o destino do labirinto usando o algoritmo A*
/// *******************************************************************************

/// Retorna o comprimento do caminho (<0 se nao existe)
///
/// O parametro prof deve conter o numero de nos (profundidade) do caminho encontrado
/// ou <0 caso nao exista caminho.
///
/// O parametro NAbert deve conter o numero de nos em aberto ao termino do algoritmo A*
/// O parametro NFech deve conter o numero de nos em fechado ao termino do algoritmo A*
/// Mesmo quando nao existe caminho, esses parametros devem ter valor atribuido.
double Labirinto::calculaCaminho(int& prof, int& NAbert, int& NFech)
{
  double compr;

  if (empty() || !origDestDefinidos())
  {
    // Impossivel executar o algoritmo
    compr = -1.0;
    prof = -1;
    NAbert = NFech = -1;
    return compr;
  }

  // Apaga um eventual caminho anterior
  limpaCaminho();

  // Testa se origem igual a destino
  if (orig==dest)
  {
    // Caminho tem comprimento e profundidade nula
    compr = 0.0;
    prof = 0;
    // Algoritmo de busca nao gerou nenhum noh
    NAbert = NFech = 0;
    // Caminho tem comprimento nulo
    return compr;
  }
  //Algoritmo A* utilizando list como conteiner

  list <Noh> Abertos;
  list <Noh> Fechados;
  Noh atual;
  Coord origem = getOrig();
  Coord destino = getDest();
  atual.pos = getOrig();
  atual.ant = getOrig();
  atual.g = 0;
  atual.h = atual.pos.hDiagonal(destino);

  Abertos.push_back(atual);

  Coord mov;
  Coord mov_ant;
  Coord mov_viz;
  Coord prox;
  Coord viz;

  do{
    atual = Abertos.front();
    Abertos.pop_front();
    Fechados.push_back(atual);
    if (atual.pos != destino){
        mov_ant = atual.pos - atual.ant;
        for (mov.lin = -1; mov.lin <= 1; mov.lin++){
            for (mov.col = -1; mov.col <= 1; mov.col++){
                if (mov != Coord (0,0)){
                    prox = atual.pos + mov;
                    if (movimentoValido(atual.pos,prox)){
                        Noh sucessor;
                        sucessor.pos = prox;
                        sucessor.ant = atual.pos;

                        double angulo;
                        if (mov_ant != Coord(0,0)) {
                            angulo = acos((mov*mov_ant)/(mov.modulo()*mov_ant.modulo()));
                        } else {
                            angulo = 0.0;
                        }
                        sucessor.g = atual.g + (angulo/2) + modulo(mov);
                        sucessor.h = prox.hDiagonal(destino);
                        for (mov_viz.lin = -1; mov_viz.lin <= 1; mov_viz.lin++){
                            for (mov_viz.col = -1; mov_viz.col <= 1; mov_viz.col++){
                                if(mov_viz != Coord(0,0)){
                                    viz = sucessor.pos + mov_viz;
                                    if (!celulaValidaLivre(viz)) {
                                        sucessor.g = sucessor.g + 0.0001;
                                    }
                                }
                            }
                        }
                        bool eh_inedito = true;
                        auto old = find(Abertos.begin(), Abertos.end(), sucessor.pos);
                        if (old != Abertos.end()){
                            if (sucessor(*old)) {
                                Abertos.erase(old);
                            }else {
                                eh_inedito = false;
                            }
                        }else{
                            auto old = find(Fechados.begin(),Fechados.end(), sucessor.pos);
                            if (old != Fechados.end()){
                                if (sucessor(*old)) {
                                    Fechados.erase(old);
                                } else  {
                                    eh_inedito = false;
                                }
                            }
                        }
                        if (eh_inedito){
                            auto big = find_if(Abertos.begin(),Abertos.end(), Noh(sucessor));
                            Abertos.insert(big,sucessor);
                        }
                    }
                }
            }
        }
    }
  }while(atual.pos != destino && !Abertos.empty());
  NFech = Fechados.size();
  NAbert = Abertos.size();
  if (atual.pos != destino) {
        compr = -1.0;
        prof = -1;
  } else {
        compr = atual.g;
        prof = 1;
  }
  while(atual.ant != origem && prof >= 0){
        set(atual.ant,EstadoCel::CAMINHO);
        auto pai = find(Fechados.begin(), Fechados.end(), atual.ant);
        if (pai != Fechados.end()) {
            atual = *pai;
            prof++;
        }
        else {
            cerr <<"Erro no algoritmo" << endl;
            prof = -1;
        }
  }
return compr;
}
