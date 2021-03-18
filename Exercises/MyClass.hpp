#pragma once


#pragma region "Includes" //{

#include <cstddef>
#include <cstdint>

#include <iostream>

#include "print.hpp"

using namespace std;

#pragma endregion //}


/*
    MyClass (dtor virtuel)
       ^
       |
  MyNiceClass

  MyOtherClass (dtor régulier)
       ^
       |
MyOtherNiceClass
*/


class MyClass {
public:
	MyClass() { printDefCtor("MyClass"); }
	MyClass(const MyClass&) { printCpyCtor("MyClass"); }
	MyClass(MyClass&&) { printMovCtor("MyClass"); }
	explicit MyClass(int) { printIntParamCtor("MyClass"); }

	virtual ~MyClass() { printDtor("MyClass"); }

	MyClass& operator=(const MyClass&) { printCpyAsgn("MyClass"); return *this; }
	MyClass& operator=(MyClass&&) { printMovCtor("MyClass"); return *this; }

	virtual void fn1() {
		cout << "MyClass::fn1()" << "\n";
	}
};

class MyOtherClass {
public:
	MyOtherClass() { printDefCtor("MyOtherClass"); }
	MyOtherClass(const MyOtherClass&) { printCpyCtor("MyOtherClass"); }
	MyOtherClass(MyOtherClass&&) { printMovCtor("MyOtherClass"); }
	MyOtherClass(int) { printIntParamCtor("MyOtherClass"); }

	// Ici, on a mis le destructeur non-virtuel intentionnellement (voir exemple de l'effet dans le main).
	~MyOtherClass() { printDtor("MyOtherClass"); }

	MyOtherClass& operator=(const MyOtherClass&) { printCpyAsgn("MyOtherClass"); return *this; }
	MyOtherClass& operator=(MyOtherClass&&) { printMovCtor("MyOtherClass"); return *this; }

	void fn2() {
		cout << "MyOtherClass::fn2()" << "\n";
	}
};

class MyNiceClass : public MyClass {
public:
	MyNiceClass() { printDefCtor("MyNiceClass"); }
	MyNiceClass(const MyNiceClass& autre) : MyClass(autre) { printCpyCtor("MyNiceClass"); }
	MyNiceClass(MyNiceClass&& autre) : MyClass(autre) { printMovCtor("MyNiceClass"); }
	MyNiceClass(int arg) : MyClass(arg) { printIntParamCtor("MyNiceClass"); }

	~MyNiceClass() override { printDtor("MyNiceClass"); }

	MyNiceClass& operator=(const MyNiceClass&) { printCpyAsgn("MyNiceClass"); return *this; }
	MyNiceClass& operator=(MyNiceClass&&) { printMovCtor("MyNiceClass"); return *this; }

	void fn1() override {
		cout << "MyNiceClass::fn1()" << "\n";
	}
};

class MyOtherNiceClass : public MyOtherClass {
public:
	MyOtherNiceClass() { printDefCtor("MyOtherNiceClass"); }
	MyOtherNiceClass(const MyOtherNiceClass&) { printCpyCtor("MyOtherNiceClass"); }
	MyOtherNiceClass(MyOtherNiceClass&&) { printMovCtor("MyOtherNiceClass"); }
	MyOtherNiceClass(int) { printIntParamCtor("MyOtherNiceClass"); }

	~MyOtherNiceClass() { printDtor("MyOtherNiceClass"); }

	MyOtherNiceClass& operator=(const MyOtherNiceClass&) { printCpyAsgn("MyOtherNiceClass"); return *this; }
	MyOtherNiceClass& operator=(MyOtherNiceClass&&) { printMovCtor("MyOtherNiceClass"); return *this; }

	void fn2() {
		cout << "MyOtherNiceClass::fn2()" << "\n";
	}

private:
	MyClass m1_;
};

