export module LinkedList;
import std;

template<typename T>
struct Node {
	T data;
	Node* next = nullptr;
	Node* prev = nullptr;

	explicit Node(const T& value) : data(value) {}
};

export template<typename T>
class LinkedList {
private:
	Node<T>* _head = nullptr;
	Node<T>* _tail = nullptr;
	size_t _size = 0;

	void clear() noexcept {
		Node<T>* current = _head;
		while (current != nullptr) {
			Node<T>* next_node = current->next;
			delete current;
			current = next_node;
		}
		_head = nullptr;
		_tail = nullptr;
		_size = 0;
	}
public:
	LinkedList() = default;
	LinkedList(const LinkedList& other) {
		Node<T>* current = other._head;
		while (current) {
			push_tail(current->data);
			current = current->next;
		}
	}
	LinkedList(size_t count, T min_val, T max_val, unsigned int seed) {
		std::mt19937 generator(seed);
		auto get_random_val = [&]() {
			if constexpr (std::is_integral_v<T>) {
				return std::uniform_int_distribution<T>(min_val, max_val)(generator);
			}
			else {
				return std::uniform_real_distribution<T>(min_val, max_val)(generator);
			}
		};
		for (size_t i = 0; i < count; ++i) {
			push_tail(get_random_val());
		}
	}
	~LinkedList() {
		clear();
	}
	LinkedList& operator=(const LinkedList& other) {
		if (this != &other) {
			LinkedList temp(other);
			std::swap(_head, temp._head);
			std::swap(_tail, temp._tail);
			std::swap(_size, temp._size);
		}
		return *this;
	}
	size_t get_size() const noexcept {
		return _size;
	}
	void push_tail(const T& value) {
		Node<T>* new_node = new Node<T>(value);
		if (!_tail) {
			_head = new_node;
			_tail = new_node;
		}
		else {
			_tail->next = new_node;
			new_node->prev = _tail;
			_tail = new_node;
		}
		++_size;
	}
	void push_tail(const LinkedList& other) {
		Node<T>* current = other._head;
		while (current) {
			push_tail(current->data);
			current = current->next;
		}
	}
	void push_head(const T& value) {
		Node<T>* new_node = new Node<T>(value);
		if (!_head) {
			_head = new_node;
			_tail = new_node;
		}
		else {
			_head->prev = new_node;
			new_node->next = _head;
			_head = new_node;
		}
		++_size;
	}
	void push_head(const LinkedList& other) {
		Node<T>* current = other._tail;
		while (current) {
			push_head(current->data);
			current = current->prev;
		}
	}
	void pop_head() {
		if (!_head) {
			throw std::underflow_error("Нельзя удалять из пустого LinkedList");
		}
		Node<T>* temp = _head;
		_head = _head->next;
		if (_head) {
			_head->prev = nullptr;
		}
		else {
			_tail = nullptr;
		}
		delete temp;
		--_size;
	}
	void pop_tail() {
		if (!_tail) {
			throw std::underflow_error("Нельзя удалять из пустого LinkedList");
		}
		Node<T>* temp = _tail;
		_tail = _tail->prev;
		if (_tail) {
			_tail->next = nullptr;
		}
		else {
			_head = nullptr;
		}
		delete temp;
		--_size;
	}
	void delete_node(const T& value) {
		Node<T>* current = _head;
		while (current) {
			Node<T>* next_node = current->next;
			if (current->data == value) {
				if (current->prev) {
					current->prev->next = next_node;
				}
				else {
					_head = next_node;
				}
				if (current->next) {
					current->next->prev = current->prev;
				}
				else {
					_tail = current->prev;
				}
				delete current;
				--_size;
			}
			current = next_node;
		}
	}
	T& operator[](size_t index) {
		if (index >= _size) {
			throw std::out_of_range("Индекс вне диапазона");
		}
		Node<T>* current = _head;
		for (size_t i = 0; i < index; ++i) {
			current = current->next;
		}
		return current->data;
	}
	const T& operator[](size_t index) const {
		if (index >= _size) {
			throw std::out_of_range("Индекс вне диапазона");
		}
		Node<T>* current = _head;
		for (size_t i = 0; i < index; ++i) {
			current = current->next;
		}
		return current->data;
	}
	friend std::ostream& operator<<(std::ostream& os, const LinkedList& list) {
		Node<T>* current = list._head;
		os << "[";
		while (current) {
			os << current->data;
			if (current->next) {
				os << ", ";
			}
			current = current->next;
		}
		os << "]";
		return os;
	}
	void reverse() noexcept {
		if (!_head || _head == _tail) {
			return;
		}
		Node<T>* current = _head;
		Node<T>* temp = nullptr;
		while (current) {
			temp = current->next;
			current->next = current->prev;
			current->prev = temp;
			current = current->prev;
		}
		temp = _head;
		_head = _tail;
		_tail = temp;
	}
};

