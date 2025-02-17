#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "Wybor.h"
#include <new>
#include <random>
#include <cassert>
#include <vector>
#include <algorithm>
#include <functional>
#include <cstring>

using namespace std;

/*
* Uwaga: Zadania kod rozwiazania zaania umieszczać pomiędzy #ifdef a #endif. 
*/

#ifdef Kol_1

class Zbior {
    size_t m_liczbaElementow = 0;
    double* m_elementy = nullptr;
    Zbior* m_asocjacja = nullptr;

    void alokuj(size_t n) {
        if (n > 0) {
            m_elementy = new double[n] {};
            m_liczbaElementow = n;
        }
    }

    void zwolnij() {
        delete[] m_elementy;
        m_elementy = nullptr;
        m_liczbaElementow = 0;
        if (m_asocjacja) {
            m_asocjacja->m_asocjacja = nullptr;
            m_asocjacja = nullptr;
        }
    }

public:
    Zbior(size_t liczElem = 0) { alokuj(liczElem); }

    Zbior(const Zbior& other) {
        alokuj(other.m_liczbaElementow);
        std::copy(other.m_elementy, other.m_elementy + other.m_liczbaElementow, m_elementy);
    }

    Zbior& operator=(const Zbior& other) {
        if (this != &other) {
            zwolnij();
            alokuj(other.m_liczbaElementow);
            std::copy(other.m_elementy, other.m_elementy + other.m_liczbaElementow, m_elementy);
        }
        return *this;
    }

    Zbior(Zbior&& other) noexcept {
        m_liczbaElementow = other.m_liczbaElementow;
        m_elementy = other.m_elementy;
        m_asocjacja = other.m_asocjacja;
        other.m_elementy = nullptr;
        other.m_liczbaElementow = 0;
        other.m_asocjacja = nullptr;
    }

    Zbior& operator=(Zbior&& other) noexcept {
        if (this != &other) {
            zwolnij();
            m_liczbaElementow = other.m_liczbaElementow;
            m_elementy = other.m_elementy;
            other.m_elementy = nullptr;
            other.m_liczbaElementow = 0;
        }
        return *this;
    }

    ~Zbior() { zwolnij(); }

    void polaczAsocjacje(Zbior* inny) {
        if (inny == nullptr || inny == this || m_asocjacja || inny->m_asocjacja) {
            return;
        }
        m_asocjacja = inny;
        inny->m_asocjacja = this;
    }

    void zerwijAsocjacje() {
        if (m_asocjacja) {
            m_asocjacja->m_asocjacja = nullptr;
            m_asocjacja = nullptr;
        }
    }

    bool isAssociated() const { return m_asocjacja != nullptr; }
};

class ZbiorPredykat {
    const Zbior& referencyjny;
public:
    ZbiorPredykat(const Zbior& zbiorRef) : referencyjny(zbiorRef) {}
    bool operator()(const Zbior& zbior) const { return zbior.isAssociated(); }
};

void testZerwaniaAsocjacji() {
    Zbior* zbior1 = new Zbior();
    Zbior* zbior2 = new Zbior();
    zbior1->polaczAsocjacje(zbior2);
    delete zbior1;
    std::cout << (zbior2->isAssociated() ? "BLAD" : "OK") << std::endl;
    delete zbior2;
}

void testKonstruktoraPrzenoszacego() {
    Zbior zbior1(5);
    Zbior zbior2(3);
    zbior1.polaczAsocjacje(&zbior2);
    Zbior zbior3 = std::move(zbior1);
    std::cout << ((zbior1.isAssociated() == false && zbior3.isAssociated()) ? "OK" : "BLAD") << std::endl;
}

int main() {
    std::vector<Zbior> lista(10);
    for (size_t i = 0; i < lista.size(); i += 2) {
        if (i + 1 < lista.size()) {
            lista[i].polaczAsocjacje(&lista[i + 1]);
        }
    }
    int liczbaAsocjacji = std::count_if(lista.begin(), lista.end(), [](const Zbior& zbior) { return zbior.isAssociated(); });
    std::cout << "Liczba instancji z asocjacja: " << liczbaAsocjacji << std::endl;
    testZerwaniaAsocjacji();
    testKonstruktoraPrzenoszacego();
    return 0;
}

#endif
