#include "zbior.h"
#include <new>
#include <random>
#include <cassert>
#include <iostream>

void Zbior::alokuj(int n)
{
	assert(n > 0);
	m_elementy = new(std::nothrow) double[n]{};
	m_liczbaElementow = n;

}

void Zbior::zwolnij()
{
	delete[] m_elementy;
}

Zbior::Zbior(size_t liczElem, double dGran, double gGran)
{
	alokuj(liczElem);
	std::uniform_real_distribution<double> zakres(dGran, gGran);
	std::random_device generator;
	for (int elem = 0; elem < liczElem; elem++)
		m_elementy[elem] = zakres(generator);
}
