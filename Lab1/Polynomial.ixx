export module Polynomial;
import std;

export template <typename T>
class Polynomial {
private:
	std::vector<T> _coeffs;
	bool is_zero(const T& val) const {
		if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>
			|| std::is_same_v<T, std::complex<float>> || std::is_same_v<T, std::complex<double>>) {
			return std::abs(val) < 1e-9;
		}
		else {
			return val == static_cast<T>(0);
		}
	}
public:
	Polynomial() : _coeffs({ static_cast<T>(0) }) {}
	Polynomial(size_t max_degree) : _coeffs(max_degree + 1, static_cast<T>(0)) {}
	Polynomial(const std::vector<T>& coefficients) : _coeffs(coefficients) {
		if (_coeffs.empty()) {
			_coeffs.push_back(static_cast<T>(0));
		}
		shrink_to_fit();
	}
	size_t degree() const {
		return _coeffs.size() - 1;
	}
	T operator[](size_t index) const {
		if (index > degree()) {
			return static_cast<T>(0);
		}
		return _coeffs[index];
	}
	void expand(size_t other_degree) {
		if (other_degree > degree()) {
			_coeffs.resize(other_degree + 1, static_cast<T>(0));
		}
	}
	void set(size_t other_degree, const T& value) {
		if (other_degree > degree()) {
			if (is_zero(value)) {
				return;
			}
			expand(other_degree);
		}
		_coeffs[other_degree] = value;
		shrink_to_fit();
	}
	template <typename X>
	auto evaluate(const X& x) const {
		using ResultType = decltype(T()* X());
		size_t d = degree();
		ResultType result = static_cast<ResultType>((*this)[d]);
		while (d > 0) {
			--d;
			result = result * x + static_cast<ResultType>((*this)[d]);
		}
		return result;
	}
	Polynomial& operator+=(const Polynomial& other) {
		if (other._coeffs.size() > _coeffs.size()) {
			_coeffs.resize(other._coeffs.size(), static_cast<T>(0));
		}
		for (size_t i = 0; i < other._coeffs.size(); ++i) {
			_coeffs[i] += other._coeffs[i];
		}
		shrink_to_fit();
		return *this;
	}
	Polynomial& operator-=(const Polynomial& other) {
		if (other._coeffs.size() > _coeffs.size()) {
			_coeffs.resize(other._coeffs.size(), static_cast<T>(0));
		}
		for (size_t i = 0; i < other._coeffs.size(); ++i) {
			_coeffs[i] -= other._coeffs[i];
		}
		shrink_to_fit();
		return *this;
	}
	Polynomial& operator*=(const T& scalar) {
		for (size_t i = 0; i < _coeffs.size(); ++i) {
			_coeffs[i] *= scalar;
		}
		shrink_to_fit();
		return *this;
	}
	void shrink_to_fit() {
		while (_coeffs.size() > 1 && is_zero(_coeffs.back())) {
			_coeffs.pop_back();
		}
		_coeffs.shrink_to_fit();
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
		if (val == static_cast<T>(0)) continue;
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

