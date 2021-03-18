#pragma once


#include <cstddef>
#include <cstdint>

#include <string>
#include <iostream>

using namespace std;


class Animal {
public:
	Animal(int weight)
	: weight_(weight) {
		printf("Animal(%i)" "\n", weight);
	}

	virtual ~Animal() {
		cout << "~Animal()" << "\n";
	}

	int getWeight() const { return weight_; }
	void setWeight(int weight) { weight_ = weight; }

	virtual void live() = 0;

private:
	int weight_;
};


class Lion : virtual public Animal {
public:
	Lion(int weight)
	: Animal(weight) {
		printf("Lion(%i)" "\n", weight);
	}

	~Lion() override {
		cout << "~Lion()" << "\n";
	}

	void live() override {
		cout << "Everything the light touches is our kingdom." << "\n";
	}

private:
	int m1_;
};


class Tiger : virtual public Animal {
public:
	Tiger(int weight)
	: Animal(weight) {
		printf("Tiger(%i)" "\n", weight);
	}

	~Tiger() override {
		cout << "~Tiger()" << "\n";
	}

	void live() override {
		cout << "I am a champion, and you're gonna hear me rooooar!" << "\n";
	}

private:
	int m2_;
};


class Liger : public Lion, public Tiger {
public:
	Liger(int weight)
	: Animal(weight),
	  Lion(weight),
	  Tiger(weight) {
		printf("Liger(%i)" "\n", weight);
	}

	~Liger() override {
		cout << "~Liger()" << "\n";
	}

	void live() override {
		Lion::live();
	}
};

