#ifndef MINHALISTAENCADEADA_H
#define MINHALISTAENCADEADA_H

#include "ListaEncadeadaAbstrata.h"

template <typename T>
class MinhaListaEncadeada :  public ListaEncadeadaAbstrata<T>
{
public:
    /**
     * @brief Destrutor. Destroi todos os elementos da lista
     */
    virtual ~MinhaListaEncadeada(){
        Elemento<T> *aux = this->_primeiro; 
        Elemento<T> *tmp = nullptr;
        
        while(aux) {
            tmp = aux; 
            aux = aux->proximo; 
            
            delete tmp;
        }
    }

    /**
     * @brief Obtém a quantidade de itens na lista.
     *
     * @return Um inteiro maior ou igual a 0.
     */
    virtual std::size_t tamanho() const
    {
        return this->_tamanho;
    };

    /**
     * @brief Indica se há algum item na lista ou não.
     *
     * @return false se houver algum item na lista; true caso contrário.
     */
    virtual bool vazia() const
    {
        if (this->_tamanho == 0) {
            return true;
        }

        return false;
    };

    /**
     * @brief Obtém a posição de um item na lista. Lança
     * ExcecaoListaEncadeadaVazia caso a lista esteja vazia ou
     * ExcecaoDadoInexistente caso o item não esteja contido na lista.
     *
     * @param dado O item cuja posição deseja-se obter.
     * @return Um inteiro na faixa [0, tamanho); se houver mais que um mesmo
     * item na lista, a posição da primeira ocorrência.
     */
    virtual std::size_t posicao(T dado) const
    {
        if (vazia()) {
            throw ExcecaoListaEncadeadaVazia();
        }

        Elemento<T> *aux = this->_primeiro;
        std::size_t contador = 0;

        while (aux) {
            if (aux->dado == dado) {
                return contador;
            }

            aux = aux->proximo;
            contador++;
        }

        throw ExcecaoDadoInexistente();
    };

    /**
     * @brief Indica se um dado item está contido na lista ou não.
     *
     * @param dado O item sendo buscado.
     * @return true se o item está contido na lista; false caso contrário.
     */
    virtual bool contem(T dado) const
    {
        if (vazia()) {
            return false;
        }

        Elemento<T> *aux = this->_primeiro;

        while(aux) {
            if (aux->dado == dado) {
                return true;
            }

            aux = aux->proximo;
        }

        return false;
    };

    /**
     * @brief Insere um item no início da lista.
     *
     * @param dado O item sendo inserido.
     */
    virtual void inserirNoInicio(T dado)
    {
        Elemento<T> *aux = this->_primeiro;
        this->_primeiro = new Elemento<T>(dado, aux);
        this->_tamanho++;
    };

    /**
     * @brief Insere um item em uma posição específica da lista. Lança
     * ExcecaoPosicaoInvalida caso a posição não esteja na faixa
     * [0, tamanho].
     *
     * @param posicao Um inteiro dentro da faixa [0, tamanho]. Ao se inserir em
     * uma posição já ocupada, a posição do item que já estava naquela posição
     * será posicao + 1; inserir-se na posicao tamanho significa inserir-se no
     * fim da lista.
     * @param dado O item sendo inserido.
     */
    virtual void inserir(std::size_t posicao, T dado)
    {
      if (posicao > this->_tamanho) {
            throw ExcecaoPosicaoInvalida();
      } else if (posicao == this->_tamanho) {
            inserirNoFim(dado);
      } else if (posicao == 0) {
            inserirNoInicio(dado);
      } else {
            Elemento<T> *aux = this->_primeiro;
            for (std::size_t i = 0; i < posicao - 1; i++) {
                aux = aux->proximo;
            }

            Elemento<T> *tmp = aux->proximo;
            aux->proximo = new Elemento<T>(dado, tmp);
            this->_tamanho++;
      }
    };


    /**
     * @brief Insere um item no fim da lista.
     *
     * @param dado O item sendo inserido.
     */
    virtual void inserirNoFim(T dado)
    {
        if (vazia()) {
            this->_primeiro = new Elemento<T>(dado, nullptr);
            this->_tamanho++;
        } else {
            Elemento<T> *aux = this->_primeiro;

            while(aux->proximo) {
                aux = aux->proximo;
            }

            aux->proximo = new Elemento<T>(dado, nullptr);
            this->_tamanho++;
        }
    };

    /**
     * @brief Remove o primeiro item da lista. Lança ExcecaoListaEncadeadaVazia
     * caso não haja nenhum item na lista.
     *
     * @return O item removido.
     */
    virtual T removerDoInicio()
    {
        if (vazia()) {
            throw ExcecaoListaEncadeadaVazia();
        }

        T dadoRemovido = this->_primeiro->dado;
        Elemento<T> *aux = this->_primeiro;
        this->_primeiro = aux->proximo;

        delete aux;

        this->_tamanho--;

        return dadoRemovido;
    };

    /**
     * @brief Remove um item de uma posição específica da lista. Lança
     * ExcecaoPosicaoInvalida caso a posição não esteja na faixa [0, tamanho).
     *
     * @param posicao Um inteiro dentro da faixa [0, tamanho).
     * @return O item removido.
     */
    virtual T removerDe(std::size_t posicao)
    {
        if (posicao >= this->_tamanho) {
            throw ExcecaoPosicaoInvalida();
        } else if (posicao == 0) {
            T dadoRemovido = removerDoInicio();
            return dadoRemovido;
        }

        Elemento<T> *aux = this->_primeiro;

        for (std::size_t i = 0; i < posicao - 1; i++) {
            aux = aux->proximo;
        }

        Elemento<T> *elementoExcluido = aux->proximo;
        T dadoRemovido = elementoExcluido->dado;
        aux->proximo = elementoExcluido->proximo;

        delete elementoExcluido;

        this->_tamanho--;

        return dadoRemovido;
    };

    /**
     * @brief Remove o último item da lista. Lança ExcecaoListaEncadeadaVazia
     * caso não haja nenhum item na lista.
     *
     * @return O item removido.
     */
    virtual T removerDoFim()
    {
        if (vazia()) {
            throw ExcecaoListaEncadeadaVazia();
        } else if (this->_tamanho == 1) {
            T dadoRemovido = this->_primeiro->dado;
            delete this->_primeiro;
            this->_primeiro = nullptr;

            this->_tamanho--;

            return dadoRemovido;
        }

        Elemento<T> *aux = this->_primeiro;

        for (std::size_t i = 0; i < this->_tamanho - 2; i++) {
            aux = aux->proximo;
        }

        T dadoRemovido = aux->proximo->dado;
        delete aux->proximo;

        aux->proximo = nullptr;

        this->_tamanho--;

        return dadoRemovido;
    };

    /**
     * @brief Remove um item específico da lista. Lança
     * ExcecaoListaEncadeadaVazia caso não haja nenhum item na lista ou
     * ExcecaoDadoInexistente caso o item não esteja contido na lista.
     *
     * @param dado O item a ser removido. Se houver mais que um item com
     * o mesmo valor, remove a primeira ocorrência.
     */
    virtual void remover(T dado)
    {
        if (vazia()) {
            throw ExcecaoListaEncadeadaVazia();
        }

        Elemento<T> *aux = this->_primeiro;
        std::size_t contador = 0;

        while(aux) {
            if (aux->dado == dado) {
                T dadoRemovido = removerDe(contador);
                return;
            }
        
            contador++;
            aux = aux->proximo; 
        }

        throw ExcecaoDadoInexistente();
    };
};

#endif
