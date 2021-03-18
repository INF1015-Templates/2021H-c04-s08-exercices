#pragma once


#pragma region "Includes" //{

#include <ciso646>
#include <cstddef>
#include <cstdint>
#include <new>

#include <ostream>
#include <memory>
#include <iomanip>
#include <type_traits>
#include <stdexcept>
#include <initializer_list>

#include <cppitertools/itertools.hpp>
#include <cppitertools/zip.hpp>

using namespace std;
using namespace iter;

#pragma endregion //}


// Déclarations avancées. C'est un peu comme les prototypes de fonctions, mais pour les classes.
template <typename> class ListNode_impl;
template <typename, typename> class ListIterator_impl;
template <typename> class List;


// Un noeud est un élément de la liste qui contient une valeur (générique dans ce cas) et connait le noeud qui le précède et qui le suit.
// La classe de noeud est normalement transparente dans l'implémentation d'une liste, c-à-d qu'on ne s'en sert jamais directement, car c'est une classe utilisée à l'interne de la liste.
// Ici, un noeud possède le noeud qui le suit, et la liste possède son premier noeud. De cette façon, lorsque le noeud est détruit, il détruit son prochain.
template <typename T>
class ListNode_impl
{
	// On met la classe List du même type comme friend pour qu'elle puisse accéder aux 
	friend class List<T>;

public:
	// Un truc très courant dans la librairie standard, on fait un typedef public 'value_type' qui représente le type des valeurs qu'on contient (donc 'T' dans le cas actuel)
	using value_type = T;

	ListNode_impl() = default;

	ListNode_impl(const T& value, ListNode_impl<T>* previous = {}, unique_ptr<ListNode_impl<T>>&& next = {})
		: value_(make_unique<T>(value)),
		  previous_(previous),
		  next_(move(next)) { }

	// On n'a rien à faire dans le destructor, car lorsqu'on détruit le membre 'next_', son destructeur se fait appeler, détruisant son prochain, et ainsi de suite jusqu'au dernier élément de la liste (dont le suivant est nul).
	// Toutefois, les appels sont récursifs et causent un débordement de pile pour un nombre modéré d'éléments (dans le millier). On peut régler le problème avec une boucle qui détruit les noeuds suivants sans appels récursifs (pas important pour le cours).
	~ListNode_impl() = default;

	T& getValue() { return *value_; }
	const T& getValue() const { return *value_; }
	ListNode_impl<T>* getPrevious() const { return previous_; }
	ListNode_impl<T>* getNext() const { return next_.get(); }

private:
	unique_ptr<T>                value_;
	ListNode_impl<T>*            previous_ = nullptr;
	unique_ptr<ListNode_impl<T>> next_;
};


// Un itérateur est une objet qui pointe vers un élément d'un conteneur et dont l'interface ressemble à celle d'un pointeur (++ et -- pour avancer/reculer, * pour accéder à la valeur).
// Pour permettre des itérateurs qui pointent vers des listes constantes ou modifiable, on passe le type de liste et de noeud en paramètre
template <class ListT, class NodeT>
class ListIterator_impl
{
	// On met List comme ami.
	template <typename>
	friend class List;

public:
	// Petit bout de code un peu avancé pour déterminer si les valeurs pointées sont constantes ou pas selon le type de List pointée.
	using ListeValueType = typename ListT::value_type;
	// En gros, si la liste est constante, alors le type de valeur (value_type) est le value_type de la liste avec const.
	using value_type = typename conditional_t<is_const_v<ListT>, const ListeValueType, ListeValueType>;

	ListIterator_impl() = default;
	ListIterator_impl(const ListIterator_impl&) = default;
	// Un itérateur est constitué d'un noeud pointée et d'une liste parente (la liste d'où vient le noeud).
	ListIterator_impl(NodeT* pos, ListT* parent) : elem_(pos), parent_(parent) { }
	ListIterator_impl& operator=(const ListIterator_impl&) = default;

	// Interface ressemblant à un pointeur. * pour accéder à la valeur, ++ et -- pour avancer/reculer, puis == et != pour comparer.
	value_type& operator*() { return elem_->getValue(); }

	ListIterator_impl& operator++() { elem_ = elem_->getNext(); return *this; }

	ListIterator_impl& operator--() {
		// Un itérateur qui pointe vers un noeud nul et une liste valide pointe vers l'élément après le dernier élément. Donc reculer un tel itérateur nous amène au dernier élément de la liste parente.
		if (elem_ != nullptr)
			elem_ = elem_->getPrevious();
		else
			elem_ = parent_->last_;
		return *this;
	}

	bool operator==(const ListIterator_impl& rhs) const { return elem_ == rhs.elem_ and parent_ == rhs.parent_; }

private:
	NodeT* elem_ = nullptr;
	ListT* parent_ = nullptr;
};


// Une liste bidirectionnelle est une séquence de noeuds qui connaissent leur noeuds précédents et suivants.
template <typename T>
class List
{
	// On met Iterateur ami (pour accéder à 'last_').
	template <typename, typename>
	friend class ListIterator_impl;

public:
	// Un autre paquet de typedef qui reflète ce qui est fait dans la librairie standard.
	using value_type = T; // Le type des éléments
	using iterator = typename ListIterator_impl<List<T>, ListNode_impl<T>>; // Le type d'itérateur pour une liste modifiable
	using const_iterator = typename ListIterator_impl<const List<T>, const ListNode_impl<T>>; // Le type d'itérateur pour une liste non-modifiable.

	List() = default;

	List(const List& autre) {
		// On réutilise notre opérateur d'affectation
		*this = autre;
	}

	// Ici on a un constructeur par paramètre qui prend un std::initializer_list. Présent dans tous les conteneurs de la std lib, c'est la classe qui encapsule la syntaxe où on fait par exemple std::vector<int> foo = {1, 2, 3};
	// Non ce n'est pas dans le cours, pas important à savoir, c'est juste pour ceux que ça intéresse.
	List(initializer_list<value_type> elements) {
		// On itère dessus avec un range-based for.
		for (auto&& e : elements)
			push_back(e);
	}

	List& operator=(const List& other) {
		if (&other == this)
			return *this;

		resize(other.size());
		// Cette syntaxe (le "range-based for", ou "foreach") est rendue possible par les itérateurs qui ont *, ++ et !=, ainsi que la liste qui a begin() et end().
		for (auto&& [e1, e2] : zip(*this, other))
			e1 = e2;
		return *this;
	}

	List& operator=(initializer_list<value_type> elements) {
		resize(elements.size());
		// Même concept qu'avec l'opérateur par copie : on redimensionne puis on copie.
		for (auto&& [e1, e2] : zip(*this, elements))
			e1 = e2;
		return *this;
	}

	// Interface de création d'itérateurs compatible avec la librairie standard et les range-based-for.
	iterator begin() { return iterator(first_.get(), this); }
	iterator end() { return iterator(nullptr, this); }
	const_iterator begin() const { return const_iterator(first_.get(), this); }
	const_iterator end() const { return const_iterator(nullptr, this); }

	size_t size () const { return nbElements_; }

	void resize(size_t size) {
		while (nbElements_ < size)
			push_back(T{});
		while (nbElements_ > size)
			effacerFin();
	}

	// L'insertion (comme pour le insert() des conteneurs standards) se fait à l'endroit spécifié. L'élément pointé par l'argument 'position' se retrouvera donc après l'élément inséré.
	void inserer (iterator position, const T& valeur)
	{
		// On vérifie que l'itérateur nous appartient...
		if (position.parent_ != this)
			throw runtime_error("Nah girl, I ain't yo daddy!");

		// Ici, le code n'est pas nécessairement le plus concis, mais il exprime conceptuellement bien ce qui se passe.
		if (position == end()) {
			push_back(valeur);
		} else if (position == begin()) {
			insererDebut(valeur);
		} else {
			auto apres = position.elem_;
			auto avant = apres->getPrevious();
			auto nouveau = make_unique<ListNode_impl<T>>(valeur);

			// Étant donné qu'un noeud possède son suivant, il faut faire attention à l'ordre des opérations.
			nouveau->previous_ = avant;
			nouveau->next_ = move(avant->next_);
			apres->previous_ = nouveau.get();
			avant->next_ = move(nouveau);

			nbElements_++;
		}
	}

	// Équivalent de push_front()
	void insererDebut (const T& valeur)
	{
		if (nbElements_ == 0) {
			insererPremierElem(valeur);
		} else {
			auto nouveau = make_unique<ListNode_impl<T>>(valeur);

			first_->previous_ = nouveau.get();
			nouveau->next_ = move(first_);
			first_ = move(nouveau);

			nbElements_++;
		}
	}

	// Équivalent de push_back()
	void push_back (const T& valeur)
	{
		if (nbElements_ == 0) {
			insererPremierElem(valeur);
		} else {
			auto nouveau = make_unique<ListNode_impl<T>>(valeur);
			auto ancienDernier = last_;

			nouveau->previous_ = ancienDernier;
			last_ = nouveau.get();
			ancienDernier->next_ = move(nouveau);

			nbElements_++;
		}
	}

	// Pour faire comme les erase() des différents conteneurs de la stdlib, effacer() retourne un itérateur
	iterator effacer (iterator position)
	{
		// On vérifie que l'itérateur nous appartient...
		if (position.parent_ != this)
			throw runtime_error("You are... NOT THE FATHER!");

		if (position == begin() or nbElements_ == 1) {
			effacerDebut();
			return begin();
		} else if (position.elem_ == last_) {
			effacerFin();
			return end();
		} else {
			auto effacement = position.elem_;
			auto avant = effacement->previous_;
			auto apres = effacement->next_.get();

			// Encore là, faire attention à l'ordre des opérations.
			apres->previous_ = avant;
			avant->next_ = move(effacement->next_);

			return iterator(apres, this);
		}
	}

	// Équivalent de pop_front()
	void effacerDebut ()
	{
		if (first_->next_ != nullptr)
			first_->next_->previous_ = nullptr;
		first_ = move(first_->next_);
		nbElements_--;
	}

	// Équivalent de pop_back()
	void effacerFin ()
	{
		if (last_->previous_ != nullptr) {
			last_ = last_->previous_;
			last_->next_.reset();
			nbElements_--;
		} else {
			// Si le dernier n'a pas de précédent, alors la liste a un seul élément et on réutilise effacerDebut() qui traite déjà ce cas.
			effacerDebut();
		}
	}

	void afficher (ostream& out, streamsize alignement = 0) const
	{
		for (auto&& e : *this)
			out << setw(alignement) << e << " ";
	}

private:
	void insererPremierElem (const T& valeur)
	{
		first_ = make_unique<ListNode_impl<T>>(valeur);
		last_ = first_.get();
		nbElements_ = 1;
	}

	unique_ptr<ListNode_impl<T>> first_;
	ListNode_impl<T>* last_ = nullptr;
	size_t               nbElements_ = 0;
};


template < typename T >
void swap (List<T>& l1, List<T>& l2)
{
	std::swap(l1.first_, l2.first_);
	std::swap(l1.last_, l2.last_);
	std::swap(l1.nbElements_, l2.nbElements_);
}

template < typename T >
ostream& operator<<(ostream& lhs, const List<T>& rhs)
{
	auto alignement = lhs.width();
	rhs.afficher(lhs, alignement);
	return lhs;
}

