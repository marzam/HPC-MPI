/*
 * Esse exercício resolve a integra x^2 no intervalo [0:1], ou seja,
 * o valor exato é de 1/3
 */

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <string>
using namespace std;

double func(double x){ return pow(x, 2.0); }
int main(int ac, char**av) {
  double h = stof(av[1]),
		 		 k = h / 2,
		     x = 0.0,
		     sum = 0.0;

  unsigned int steps = static_cast<unsigned int>(1.0 / h);

  cout << "Integração de uma função f(x)" << endl;
	cout << "Qtde. de trapézios: " << steps << " - valor do h = " << h << endl;

  for (unsigned int i = 0; i <= steps; i++){
		  double offset = h * static_cast<double> (i);
		  double aux = 2.0 * k;
		  if ((i == 0) || (i == steps))
			  aux = k;
		  sum += (func(x + offset) * aux);
  }

  cout << "Resultado da integração: " << sum << endl;
  

  return EXIT_SUCCESS;
}
