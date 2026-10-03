export module Polynomial;
import std;

export template <typename T>
class Polynomial {
private:
	T* _coeffs;
	size_t _capacity;
	size_t _degree;
	void reallocate(size_t new_capacity) {
		T* new_coeffs = new T[new_capacity]();
		size_t n = (new_capacity < _capacity) ? new_capacity : _capacity;
		for (size_t i = 0; i < n; ++i) {
			new_coeffs[i] = _coeffs[i];
		}
		delete[] _coeffs;
		_coeffs = new_coeffs;
		_capacity = new_capacity;
	}
public:
	Polynomial() : _coeffs(new T[1]()), _capacity(1), _degree(0) {}
	Polynomial(size_t max_degree) : _coeffs(new T[max_degree + 1]()), _capacity(max_degree + 1), _degree(max_degree) {}
	Polynomial(const T* coefficients, size_t size) {
		if (size == 0) {
			_coeffs = new T[1]();
			_capacity = 1;
			_degree = 0;
		}
		else {
			_coeffs = new T[size]();
			for (size_t i = 0; i < size; ++i) {
				_coeffs[i] = coefficients[i];
			}
			_capacity = size;
			_degree = size - 1;
			shrink_to_fit();
		}
	}
	Polynomial(const Polynomial& other) : _coeffs(new T[other._capacity]()), _capacity(other._capacity), _degree(other._degree) {
		for (size_t i = 0; i < _capacity; ++i) {
			_coeffs[i] = other._coeffs[i];
		}
	}
	Polynomial& operator=(const Polynomial& other) {
		if (this != &other) {
			T* new_coeffs = new T[other._capacity]();
			for (size_t i = 0; i < other._capacity; ++i) {
				new_coeffs[i] = other._coeffs[i];
			}
			delete[] _coeffs;
			_coeffs = new_coeffs;
			_capacity = other._capacity;
			_degree = other._degree;
		}
		return *this;
	}
	~Polynomial() {
		delete[] _coeffs;
	}
	bool is_zero(const T& val) const {
		if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>
			|| std::is_same_v<T, std::complex<float>> || std::is_same_v<T, std::complex<double>>) {
			return std::abs(val) < 1e-9;
		}
		else {
			return val == static_cast<T>(0);
		}
	}
	size_t degree() const {
		return _degree;
	}
	T operator[](size_t index) const {
		if (index > _degree) {
			throw std::out_of_range("Индекс за пределами степени многочлена!");
		}
		return _coeffs[index];
	}
	void expand(size_t other_degree) {
		if (other_degree >= _capacity) {
			reallocate(other_degree + 1);
		}
		_degree = other_degree;
	}
	void set(size_t index, const T& value) {
		if (index > _degree) {
			if (is_zero(value)) return;
			expand(index);
		}
		_coeffs[index] = value;
		shrink_to_fit();
	}
	template <typename X>
	auto evaluate(const X& x) const {
		using ResultType = decltype(T()* X());
		size_t d = degree();
		ResultType result = static_cast<ResultType>(_coeffs[d]);
		while (d > 0) {
			--d;
			result = result * x + static_cast<ResultType>(_coeffs[d]);
		}
		return result;
	}
	Polynomial& operator+=(const Polynomial& other) {
		if (other._degree > _degree) {
			expand(other._degree);
		}
		for (size_t i = 0; i < other._degree + 1; ++i) {
			_coeffs[i] += other._coeffs[i];
		}
		shrink_to_fit();
		return *this;
	}
	Polynomial& operator-=(const Polynomial& other) {
		if (other._degree > _degree) {
			expand(other._degree);
		}
		for (size_t i = 0; i < other._degree + 1; ++i) {
			_coeffs[i] -= other._coeffs[i];
		}
		shrink_to_fit();
		return *this;
	}
	Polynomial& operator*=(const T& scalar) {
		for (size_t i = 0; i < _degree + 1; ++i) {
			_coeffs[i] *= scalar;
		}
		shrink_to_fit();
		return *this;
	}
	bool operator==(const Polynomial& other) const {
		if (_degree != other._degree) return false;
		for (size_t i = 0; i < _degree + 1; ++i) {
			T diff = _coeffs[i] - other._coeffs[i];
			if (!is_zero(diff)) {
				return false;
			}
		}
		return true;
	}
	bool operator!=(const Polynomial& other) const {
		return !(*this == other);
	}
	void shrink_to_fit() {
		while (_degree > 0 && is_zero(_coeffs[_degree])) {
			--_degree;
		}
		if (_capacity > _degree + 1) {
			reallocate(_degree + 1);
		}
	}
};

export template <typename T>
Polynomial<T> operator+(Polynomial<T> left, const Polynomial<T>& right) {
	left += right;
	return left;
}
export template <typename T>
Polynomial<T> operator-(Polynomial<T> left, const Polynomial<T>& right) {
	left -= right;
	return left;
}
export template <typename T>
Polynomial<T> operator*(Polynomial<T> poli, const T& scalar) {
	return poli *= scalar;
}
export template <typename T>
Polynomial<T> operator*(const T& scalar, Polynomial<T> poli) {
	return poli *= scalar;
}

export template <typename T>
auto find_extremum(const Polynomial<T>& poly) {
	if (poly.degree() != 2) {
		throw std::invalid_argument("Степень многочлена должна быть 2");
	}
	T c = poly[0];
	T b = poly[1];
	T a = poly[2];
	if (a == static_cast<T>(0)) {
		throw std::invalid_argument("Коэффициент при x^2 не может быть нулем");
	}
	auto x_extrem = -b / (static_cast<T>(2) * a);
	auto y_extrem = poly.evaluate(x_extrem);
	return std::pair{ x_extrem, y_extrem };
}
export template <typename T>
std::ostream& operator<<(std::ostream& os, const Polynomial<T>& poly) {
	bool first = true;
	for (size_t i = poly.degree() + 1; i > 0; --i) {
		size_t idx = i - 1;
		T val = poly[idx];
		if (poly.is_zero(val)) continue;
		if (!first) {
			os << " + ";
		}
		if (idx == 0 || val != static_cast<T>(1)) {
			os << val;
		}
		if (idx > 0) {
			os << "x";
			if (idx > 1) os << "^" << idx;
		}
		first = false;
	}
	return first ? (os << "0") : os;
}

