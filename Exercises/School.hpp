#pragma once


#include <cstddef>
#include <cstdint>

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <algorithm>

#include <cppitertools/itertools.hpp>

using namespace std;
using namespace iter;


// Déclarations avancées
class UndergradStudent;
class GraduateStudent;
class Employee;
class Professor;
class School;


// Classe abstraite représentant un étudiant. Contient un matricule, programme d'étude (ex: Génie informatique)
// et une moyenne globale, ainsi que les accesseurs/modificateurs associés.
class Student {
public:
	Student(int id, const string& program)
	: studentId_(id),
	  program_(program) { }

	// On met le destructeur virtuel car on sait que la classe va être dérivée.
	virtual ~Student() = default;

	void updateGpa(double gpa) { gpa_ = gpa; }
	int getStudentId() const { return studentId_; }
	double getGpa() const { return gpa_; }

	void changeProgram(const string& program) {
		haveExistentialCrisis();
		applyProgramChange(program);
	}

protected:
	// Méthode virtuelle pure qui doit être implémentée dans les classes dérivées.
	virtual void applyProgramChange(const string& programme) = 0;

	string program_;

private:
	void haveExistentialCrisis() {
		cout << "Why am I here, just to suffer?" << "\n";
	}

	int    studentId_;
	double gpa_ = 0.0;
};


// Classe abstraite, plus spécifiquement une classe interface. C'est une interface car c'est une classe abstraite
// qui n'a aucune données membres. Cette définition est purement une convention et le concept de classe interface
// n'existe pas direcement dans le langage C++.
class IllegalStudent {
public:
	virtual ~IllegalStudent() = default;

	virtual void showUpWithoutPaying() = 0;
};


// On hérite de Student et on implémente la méthode changeProgram(), on n'a donc plus aucune méthode virtuelle pure
// et UndergradStudent est donc une classe concrète qui peut être instanciée.
class UndergradStudent : public Student {
public:
	// Petit truc pour hériter des constructeurs de sa classe parent sans avoir à les répéter.
	using Student::Student;

protected:
	void applyProgramChange(const string& program) override {
		cout << "Welp let's hope I don't have to retake math classes... ";
		// On peu s'imaginer un accès à la bonne base de donnée, bla bla...
		program_ = program;
		cout << "It is done. NOW STUDY WEAKLING!" << endl;
	}
};


// Même concept qu'avec UndergradStudent, avec implémentation différente de changeProgram(). En plus, on ajoute un superviseur
// comme données avec accesseur/modificateur. Ceci est donc spécifique à GraduateStudent.
class GraduateStudent : public Student
{
public:
	GraduateStudent(int id, string program, Professor* supervisor)
	: Student(id, program),
	  supervisor_(supervisor) { }

	Professor* getSupervisor() const { return supervisor_; }
	void changeSupervisor(Professor* supervisor) { supervisor_ = supervisor; }

protected:
	void applyProgramChange(const string& program) override {
		cout << "This is probably a waste of time, but so am I... ";
		// On peu s'imaginer un accès à la bonne base de donnée, changement au plan d'étude, etc.
		program_ = program;
		cout << "Aight cool." << endl;
	}

private:
	Professor* supervisor_;
};


// Classe qui hérite à la fois de EtudiantCycleSup et de l'interface EtudiantIllegal, en implémentant
// la méthode showUpWithoutPaying().
class ChaoticEvilStudent : public GraduateStudent, public IllegalStudent {
public:
	using GraduateStudent::GraduateStudent;

	// Comme pour les méthodes, on met 'override' à notre destructeur virtuel réimplémenté.
	~ChaoticEvilStudent() override {
		cout << "I'll be back!" << "\n";
	}

	void showUpWithoutPaying() override {
		cout << "chu dans le cours lol" << "\n";
	}
};


// Classe d'employé qui représente un employé générique. Ce n'est pas une classe abstraite, mais on va quand même en hériter.
class Employee {
public:
	Employee() = delete;
	Employee(int id) : employeeId_(id) { }

	// Encore là, important de le mettre virtuel si on sait qu'on va en hériter.
	virtual ~Employee() = default;

	int getEmployeeId() const { return employeeId_; }

private:
	int employeeId_;
};

// En plus des trucs de Employee, on a des méthodes pour ajouter et retirer des étudiants supervisés.
class Professor : public Employee {
public:
	using Employee::Employee;

	void addStudent(GraduateStudent* student) {
		students_.push_back(student);
	}

	void removeStudent(int id) {
		size_t pos = findStudent(id);
		if (pos != -1)
			students_.erase(students_.begin() + pos);
	}

protected:
	size_t findStudent(int id) {
		size_t pos = (size_t)-1;
		for (auto i : range(students_.size())) {
			if (students_[i]->getStudentId() == id) {
				pos = i;
				break;
			}
		}
		return pos;
	}

private:
	vector<GraduateStudent*> students_;
};


class School {
public:
	void addNewUndergrad(int studentId, const string& program) {
		auto uptr = make_unique<UndergradStudent>(studentId, program);
		undergrads_.push_back(uptr.get());
		students_.push_back(move(uptr));
	}

	void addNewGrad(int studentId, const string& program, Professor* supervisor = nullptr) {
		auto uptr = make_unique<GraduateStudent>(studentId, program, supervisor);
		gradStudents_.push_back(uptr.get());
		students_.push_back(move(uptr));
	}

	Student* findStudent(int studentId) {
		for (auto& e : students_)
			if (e->getStudentId() == studentId)
				return e.get();
		return nullptr;
	}

	UndergradStudent* findUndergrad(int studentId) {
		for (auto& e : undergrads_)
			if (e->getStudentId() == studentId)
				return e;
		return nullptr;
	}

	GraduateStudent* findGrad(int studentId) {
		for (auto& e : gradStudents_)
			if (e->getStudentId() == studentId)
				return e;
		return nullptr;
	}

private:
	vector<unique_ptr<Student>> students_; // Contient les ressources
	vector<UndergradStudent*>   undergrads_; // Conteneur redondant pour séparer les étudiants au bac.
	vector<GraduateStudent*>    gradStudents_;
};
