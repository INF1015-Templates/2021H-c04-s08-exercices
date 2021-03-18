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

#include <cppitertools/itertools.hpp>
#include <cppitertools/zip.hpp>

#include "MyClass.hpp"
#include "List.hpp"

using namespace std;
using namespace iter;


int main() {
	List<int> foo = {1, 2, 3};
	List<int> bar = {1, 2};
	cout << foo << "\n";
	foo = bar;
	cout << foo << "\n";
	auto qux = foo;
	cout << qux << "\n";
	cout << "\n\n\n\n";
}

