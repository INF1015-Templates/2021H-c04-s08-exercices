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
#include "School.hpp"
#include "Animal.hpp"

using namespace std;
using namespace iter;


void runSimpleVtableExample() {
	class Spam {
	public:
		virtual void fn1() const {
			cout << "Hello, world!" << "\n";
		}

		virtual int fn2() const {
			return m1_;
		}

		int m1_ = 0;
	};

	class Eggs : public Spam {
	public:
		void fn1() const override {
			cout << "Howdy there!" << "\n";
		}

		int fn2() const override {
			return m1_ + m2_;
		}

		int m2_ = 0;
	};

	class Parrot : public Eggs {
	public:
		void fn1() const override {
			cout << "Well hey there little fella" << "\n";
		}

		int fn2() const override {
			return m1_ + m2_ + m3_;
		}

		int m3_ = 0;
	};


	Parrot foo;
	foo.m1_ = 0x42;
	foo.m2_ = 0x69;
	foo.m3_ = 0x1337;
	cout << sizeof(foo) << "\n";

	Spam& ref = foo;
	ref.fn1();
	auto res = ref.fn2();
	cout << res;
}

void runVirtualInheritanceVtableExample() {
	struct Spam {
		int m1_ = 0;
	};

	struct Eggs : virtual public Spam {
		int m2_ = 0;
	};

	struct Parrot : virtual public Spam {
		int m3_ = 0;
	};

	struct ExParrot : public Eggs, public Parrot {
		int m4_ = 0;
	};


	ExParrot foo;
	foo.m1_ = 0x42;
	foo.m2_ = 0x69;
	foo.m3_ = 0x1337;
	foo.m4_ = 0xDEAD;
	cout << sizeof(foo) << "\n";
}

void runSchoolExample() {
	// Exemple prof/étudiant (School.hpp)
	Professor nancySirois(0xDEADBEEF);
	GraduateStudent lukasLehoux(69420, "Maitrise en nenie fomatique", &nancySirois);
		
	Professor karlBachand(0xBADCAFE);
	ChaoticEvilStudent alexDesrosiers(0xFEEDBABE, "Genie chimique AKA 'comment optimiser la production de meth'", &karlBachand);
	Student* ptr = &alexDesrosiers;
	// 'ptr' est un pointeur de Etudiant (up-casté de MauvaisEtudiantBanditPasCorrect)
	// Si on le dynamic_cast en GradStudent*, ça marche puisque MauvaisEtudiantBanditPasCorrect est dérivé
	// de GradStudent, et donc en est un.
	if ( dynamic_cast<GraduateStudent*>(ptr) != nullptr )
		cout << "Sup" << endl;
	else
		cout << "Pas sup" << endl;

	// Destructeur de MauvaisEtudiantBanditPasCorrect se fait appelé à la fin du scope : I'll be back!
}

void runAnimalExample() {
	Liger unholyAbomination(1337);
	unholyAbomination.live();
	Lion& simba = unholyAbomination;
	Tiger& tony = unholyAbomination;
	Animal& animal = unholyAbomination;
	simba.live();
	tony.live();
	animal.live();
}

void runAmbiguousCallExample() {
	class LegalEntity {
	public:
		LegalEntity(int id) : id_(id) { }

		int getId() const {
			return id_;
		}

	private:
		int id_;
	};

	class StockExchangeEntity {
	public:
		StockExchangeEntity(int id) : id_(id) { }

		int getId() const {
			return id_;
		}

	private:
		int id_;
	};

	class PublicCompany : public LegalEntity, public StockExchangeEntity {
	public:
		PublicCompany(int legalId, int xchgId)
		: LegalEntity(legalId),
		  StockExchangeEntity(xchgId) {
			
		}

		//using LegalEntity::getId;

		int getReq() const {
			return LegalEntity::getId();
		}

		int getStonksId() const {
			return StockExchangeEntity::getId();
		}

		pair<int, int> getId() const {
			return {LegalEntity::getId(), StockExchangeEntity::getId()};
		}
	};

	PublicCompany foo(0xDEAD, 0xBEEF);
	auto&& [req, xchg] = foo.getId();
	cout << hex << req << "\n";
	cout << hex << xchg << "\n";
	//cout << hex << foo.getId() << "\n";
	//cout << hex << foo.getReq() << "\n";
	//cout << hex << foo.getStonksId() << "\n";
}


int main() {
	//runAmbiguousCallExample(); cout << "\n\n\n\n";
	//runSimpleVtableExample(); cout << "\n\n\n\n";
	runVirtualInheritanceVtableExample(); cout << "\n\n\n\n";
	//runSchoolExample(); cout << "\n\n\n\n";
	//runAnimalExample(); cout << "\n\n\n\n";
}

