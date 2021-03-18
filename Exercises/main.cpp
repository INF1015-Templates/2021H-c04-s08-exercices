///
/// Ce projet utilise quelques librairies disponibles sur Vcpkg et permet entre autre d'en tester le bon fonctionnement.
///


#include <cstddef>
#include <cstdint>

#include <array>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <span>
#include <utility>
#include <list>

#include <cppitertools/itertools.hpp>
#include <cppitertools/zip.hpp>

#include "MyClass.hpp"
#include "List.hpp"

using namespace std;
using namespace iter;


void runListExample() {
	{
		List<int> alexanderThibault = {1, 3, 4};
		auto it = ++alexanderThibault.begin(); // Faire ++ sur un itérateur le fait avancer, donc faire ++ sur le résultat de begin() (le premier élément) nous amène au deuxième.
		alexanderThibault.insert(it, 2);
		cout << alexanderThibault << "\n";

		it = alexanderThibault.erase(alexanderThibault.begin());
		cout << "1er elem : " << *it << "\n";
		alexanderThibault.pop_back();
		alexanderThibault.pop_front();
		cout << alexanderThibault << "\n";
	}

	cout << string(60, '-') << endl;

	{
		List<int> michelleCardinal = {69, 420};
		cout << michelleCardinal << "\n";
		michelleCardinal.erase(--michelleCardinal.end()); // On ne peut pas erase le end(), car cet élément n'existe pas (il est après la fin de la liste). C'est pour cela que je fait -- sur end() pour avoir un itérateur sur le dernier élément. C'est pas mal la convention en C++.
		cout << michelleCardinal << "\n";
		michelleCardinal.pop_back();
		cout << michelleCardinal << "\n";
	}
}


int main() {
	runListExample(); cout << "\n\n\n\n";
}

