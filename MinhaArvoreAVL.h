#ifndef MINHA_ARVORE_AVL_HPP
#define MINHA_ARVORE_AVL_HPP

#include "ArvoreBinariaDeBusca.h"

/**
 * @brief Representa uma árvore AVL.
 *
 * @tparam T O tipo de dado guardado na árvore.
 */
template <typename T>
class MinhaArvoreAVL final : public ArvoreBinariaDeBusca<T>
{
    virtual ~MinhaArvoreAVL(){
        deletarRecursao(&this->raiz);
    };
    
    void deletarRecursao(Nodo<T> **nodo) //utilizei a logica da passagem pos-ordem para a delecao
    {
        if (*nodo == nullptr)
        {
            return;
        }
        
        deletarRecursao(&(*nodo)->filhoEsquerda);
        deletarRecursao(&(*nodo)->filhoDireita);
        delete *nodo;
        *nodo = nullptr;
    };
    
    /**
     * @brief Verifica se a arvore esta vazia
     * @return Verdade se a arvore esta vazia.
     */
    virtual bool vazia() const
    {
        if (this->raiz == nullptr)
        {
            return true;
        }

        return false;
    };

    /**
     * @brief Retornar quantidade de chaves na arvore
     * @return Numero natural que representa a quantidade de chaves na arvore
     */
    virtual int quantidade() const
    {
        int qtdChaves = 0;

        contarChaves(this->raiz, &qtdChaves);

        return qtdChaves;
    };

    void contarChaves(Nodo<T> *nodo, int *contador) const
    {
        if (nodo == nullptr)
        {
            return;
        }

        *contador = *contador + 1;
        contarChaves(nodo->filhoEsquerda, contador);
        contarChaves(nodo->filhoDireita, contador);
    };

    /**
     * @brief Verifica se a arvore contem uma chave
     * @param chave chave a ser procurada na arvore
     * @return Verdade se a arvore contem a chave
     */
    virtual bool contem(T chave) const
    {
        if (vazia())
        {
            return false;
        }

        bool existeChave = false; //inicializei em false pra evitar o retorno eventual de lixo de memoria

        buscarChave(this->raiz, chave, &existeChave);

        return existeChave;
    };

    void buscarChave(Nodo<T> *nodo, T chaveProcurada, bool *existeChave) const
    {

        if (nodo == nullptr)
        {
            return;
        }

        if (nodo->chave == chaveProcurada)
        {
            *existeChave = true;
            return;
        }
        
        if (chaveProcurada < nodo->chave) 
        {
            buscarChave(nodo->filhoEsquerda, chaveProcurada, existeChave);
        }
        else
        {
           buscarChave(nodo->filhoDireita, chaveProcurada, existeChave); 
        }

    };

    /**
     * @brief Retorna a altura da (sub)arvore
     * @param chave chave que é raiz da (sub)arvore cuja altura queremos.
     * @return Numero inteiro representando a altura da (subarvore). Se chave nao esta na arvore, retorna std::nullopt
     */
    virtual std::optional<int> altura(T chave) const
    {
        Nodo<T> *nodoPtr = nullptr; //esse ponteiro vai indicar o nodo que contem a chave
        int valorAltura = 0;

        encontrarNodo(this->raiz, &nodoPtr, chave);

        if (nodoPtr == nullptr) //se o conteudo do ponteiro ainda for 'nullptr' a chave nao foi encontrada
        {
            return std::nullopt;
        }

        calcularAltura(&nodoPtr);

        valorAltura = alturaNodo(nodoPtr);

        return valorAltura;
    };

    void encontrarNodo(Nodo<T> *nodoRaiz, Nodo<T> **nodoPtrAux,  T chave) const //como quero alterar o conteudo de nodoPtr, preciso de um parametro que seja ponteiro de ponteiro
    {
        if (nodoRaiz == nullptr)
        {
            return;
        }

        if (nodoRaiz->chave == chave)
        {
            *nodoPtrAux = nodoRaiz; //se a chave for encontrada, atualizo o ponteiro auxiliar para o nodo especifico
            return;
        }

        if (chave < nodoRaiz->chave)
        {
            encontrarNodo(nodoRaiz->filhoEsquerda, nodoPtrAux, chave);
        }
        else
        {
            encontrarNodo(nodoRaiz->filhoDireita, nodoPtrAux, chave);
        }
    };

    int alturaNodo(Nodo<T> *nodo) const
    {
        if (nodo == nullptr)
        {
            return -1;
        }

        return nodo->altura;
    };

    /**
     * @brief Insere uma chave na arvore
     * @param chave chave a ser inserida
     */
    virtual void inserir(T chave)
    {
        inserirRecursao(&this->raiz, chave);

    };

    void inserirRecursao(Nodo<T> **nodo, T chave) //ponteiro de ponteiro para ser possivel alterar o valor da raiz
    {
        if (*nodo == nullptr)
        {
            (*nodo) = new Nodo<T>();
            (*nodo)->chave = chave;
            return;
        }

        if (chave < (*nodo)->chave)
        {
            inserirRecursao(&(*nodo)->filhoEsquerda, chave);
        }
        else if (chave > (*nodo)->chave)
        {
            inserirRecursao(&(*nodo)->filhoDireita, chave);
        }
        else if (chave == (*nodo)->chave)
        {
            return;
        }

        balancearNodo(&(*nodo), chave);
    };

    void calcularAltura(Nodo<T> **nodo) const
    {
        int valorAltura = 0;

        if (*nodo != nullptr)
        {
            valorAltura = 1 + std::max(alturaNodo((*nodo)->filhoEsquerda), alturaNodo((*nodo)->filhoDireita));
            (*nodo)->altura = valorAltura;
        }
    };

    int calcularFatorBalanceamento(Nodo<T> **nodo)
    {
        if ((*nodo) == nullptr)
        {
            return 0;
        }
        
        return (alturaNodo((*nodo)->filhoEsquerda)) - (alturaNodo((*nodo)->filhoDireita));
    };

    void rotacaoDireita(Nodo<T> **nodo)
    {
        Nodo<T> *novoPai = (*nodo)->filhoEsquerda;
        Nodo<T> *aux = novoPai->filhoDireita;

        novoPai->filhoDireita = *nodo;
        (*nodo)->filhoEsquerda = aux;

        calcularAltura(&(*nodo));
        calcularAltura(&novoPai);

        *nodo = novoPai;
    };

    void rotacaoEsquerda(Nodo<T> **nodo)
    {
        Nodo<T> *novoPai = (*nodo)->filhoDireita;
        Nodo<T> *aux = novoPai->filhoEsquerda;

        novoPai->filhoEsquerda = *nodo;
        (*nodo)->filhoDireita = aux;

        calcularAltura(&(*nodo));
        calcularAltura(&novoPai);

        *nodo = novoPai;
    };

    void rotacaoEsquerdaDireita(Nodo<T> **nodo)
    {
        rotacaoEsquerda(&(*nodo)->filhoEsquerda);

        rotacaoDireita(&(*nodo));
    };

    void rotacaoDireitaEsquerda(Nodo<T> **nodo)
    {
        rotacaoDireita(&(*nodo)->filhoDireita);

        rotacaoEsquerda(&(*nodo));
    };

    void balancearNodo(Nodo<T> **nodo, T chave)
    {
        int fatorBalanceamento = 0;
        int fatorBalanceamentoEsq = 0;
        int fatorBalanceamentoDir = 0;

        calcularAltura(&(*nodo));
        fatorBalanceamento = calcularFatorBalanceamento(&(*nodo));
        fatorBalanceamentoEsq = calcularFatorBalanceamento(&(*nodo)->filhoEsquerda);
        fatorBalanceamentoDir = calcularFatorBalanceamento(&(*nodo)->filhoDireita);

        if (fatorBalanceamento > 1 && fatorBalanceamentoEsq >= 0)
        {
            rotacaoDireita(&(*nodo));
        }

        if (fatorBalanceamento < -1 && fatorBalanceamentoDir <= 0)
        {
            rotacaoEsquerda(&(*nodo));
        }

        if (fatorBalanceamento > 1 && fatorBalanceamentoEsq < 0)
        {
            rotacaoEsquerdaDireita(&(*nodo));
        }

        if (fatorBalanceamento < -1 && fatorBalanceamentoDir > 0)
        {
            rotacaoDireitaEsquerda(&(*nodo));
        }

    };

    /**
     * @brief Remove uma chave da arvore
     * @param chave chave a removida
     */
    virtual void remover(T chave)
    {
        removerRecursao(&this->raiz, chave);
    };

    void removerRecursao(Nodo<T> **nodo, T chave)
    {
        if ((*nodo) == nullptr)
        {
            return;
        }

        if (chave < (*nodo)->chave)
        {
            removerRecursao(&(*nodo)->filhoEsquerda, chave);
        }
        else if (chave > (*nodo)->chave)
        {
            removerRecursao(&(*nodo)->filhoDireita, chave);
        }
        else
        {
            deletarNodo(&(*nodo), chave);
        }
        
        if ((*nodo) != nullptr)
        {
            balancearNodo(&(*nodo), chave);
            calcularAltura(&(*nodo));
        }        
    };

    void deletarNodo(Nodo<T> **nodo, T chave)
    {
        Nodo<T> *nodoAux = *nodo; //variavel que vai apontar para a regiao a ser deletada
        Nodo<T> *nodoMinimoDireita = nullptr; //sera usado eventualmente para um dos casos

        if ((*nodo)->filhoEsquerda == nullptr && (*nodo)->filhoDireita == nullptr)
        {
            (*nodo) = nullptr;
            delete nodoAux;
        }
        else if ((*nodo)->filhoEsquerda != nullptr && (*nodo)->filhoDireita != nullptr)
        {
            encontrarMinimoDireita((*nodo)->filhoDireita, &nodoMinimoDireita);
            
            (*nodo)->chave = nodoMinimoDireita->chave;
            
            removerRecursao(&(*nodo)->filhoDireita, nodoMinimoDireita->chave);
        }
        else if ((*nodo)->filhoDireita == nullptr)
        {
            (*nodo) = (*nodo)->filhoEsquerda;
            delete nodoAux;
        }
        else if ((*nodo)->filhoEsquerda == nullptr)
        {
            (*nodo) = (*nodo)->filhoDireita;
            delete nodoAux;
        }
        
        if ((*nodo) != nullptr)
        {
            balancearNodo(&(*nodo), chave);
            calcularAltura(&(*nodo));
        }
    };
    
    void encontrarMinimoDireita(Nodo<T> *nodo, Nodo<T> **nodoMinimoDireita)
    {
        if (nodo->filhoEsquerda == nullptr)
        {
            (*nodoMinimoDireita) = nodo;
            return;
        }

        encontrarMinimoDireita(nodo->filhoEsquerda, &(*nodoMinimoDireita));
    };

    /**
     * @brief Busca a chave do filho a esquerda de uma (sub)arvore
     * @param chave chave da arvore que eh pai do filho a esquerda
     * @return Chave do filho a esquerda. Se chave nao esta na arvore, retorna std::nullopt
     */
    virtual std::optional<T> filhoEsquerdaDe(T chave) const
    {
        Nodo<T> *nodoPtr = nullptr;

        encontrarNodo(this->raiz, &nodoPtr, chave);

        if (nodoPtr == nullptr || nodoPtr->filhoEsquerda == nullptr) //se o conteudo do ponteiro ainda for 'nullptr' a chave nao foi encontrada || idem
        {
            return std::nullopt;
        }

        return nodoPtr->filhoEsquerda->chave;
    };

    /**
     * @brief Busca a chave do filho a direita de uma (sub)arvore
     * @param chave chave da arvore que eh pai do filho a direita
     * @return Chave do filho a direita. Se chave nao esta na arvore, retorna nullptr
     */
    virtual std::optional<T> filhoDireitaDe(T chave) const
    {
        Nodo<T> *nodoPtr = nullptr;

        encontrarNodo(this->raiz, &nodoPtr, chave);

        if (nodoPtr == nullptr || nodoPtr->filhoDireita == nullptr) //se o conteudo do ponteiro ainda for 'nullptr' a chave nao foi encontrada || idem
        {
            return std::nullopt;
        }

        return nodoPtr->filhoDireita->chave;
    };

    /**
     * @brief Lista chaves visitando a arvore em ordem
     * @return Lista encadeada contendo as chaves em ordem.
     */
    virtual ListaEncadeadaAbstrata<T> *emOrdem() const
    {
        MinhaListaEncadeada<T> *minhaLista = new MinhaListaEncadeada<T>();

        inserirNaListaEm(this->raiz, minhaLista);

        return minhaLista;
    };

    void inserirNaListaEm(Nodo<T> *nodo, MinhaListaEncadeada<T> *lista) const
    {
        if (nodo == nullptr)
        {
            return;
        }

        inserirNaListaEm(nodo->filhoEsquerda, lista);
        lista->inserirNoFim(nodo->chave);
        inserirNaListaEm(nodo->filhoDireita, lista);
    };


    /**
     * @brief Lista chaves visitando a arvore em pre-ordem
     * @return Lista encadeada contendo as chaves em pre-ordem.
     */
    virtual ListaEncadeadaAbstrata<T> *preOrdem() const
    {
        MinhaListaEncadeada<T> *minhaLista = new MinhaListaEncadeada<T>();

        inserirNaListaPre(this->raiz, minhaLista);

        return minhaLista;
    };

    void inserirNaListaPre(Nodo<T> *nodo, MinhaListaEncadeada<T> *lista) const
    {
        if (nodo == nullptr)
        {
            return;
        }

        lista->inserirNoFim(nodo->chave);
        inserirNaListaPre(nodo->filhoEsquerda, lista);
        inserirNaListaPre(nodo->filhoDireita, lista);
    };

    /**
     * @brief Lista chaves visitando a arvore em pos-ordem
     * @return Lista encadeada contendo as chaves em pos ordem.
     */
    virtual ListaEncadeadaAbstrata<T> *posOrdem() const
    {
        MinhaListaEncadeada<T> *minhaLista = new MinhaListaEncadeada<T>();

        inserirNaListaPos(this->raiz, minhaLista);

        return minhaLista;
    };

    void inserirNaListaPos(Nodo<T> *nodo, MinhaListaEncadeada<T> *lista) const
    {
        if (nodo == nullptr)
        {
            return;
        }

        inserirNaListaPos(nodo->filhoEsquerda, lista);
        inserirNaListaPos(nodo->filhoDireita, lista);
        lista->inserirNoFim(nodo->chave);
    };
};

#endif
