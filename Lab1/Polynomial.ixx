export module Polynomial;

export{
	template <typename T>
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
		void set(size_t degree, const T& value) {
			if (degree > degree()) {
				if (is_zero(value)) {
					return;
				}
				expand(degree);
			}
			_coeffs[degree] = value;
			shrink_to_fit();
		}
		template <typename X>
		auto evaluate(const X& x) const;
		Polynomial operator+(const Polynomial& other) const;
		Polynomial operator-(const Polynomial& other) const;
		Polynomial operator*(const T& scalar) const;
		void shrink_to_fit() {
			while (_coeffs.size() > 1 && is_zero(_coeffs.back())) {
				_coeffs.pop_back();
			}
			_coeffs.shrink_to_fit();
		}
	};
	template <typename T>
	Polynomial<T> operator*(const T& scalar, const Polynomial<T>& poli) {
		return poli * scalar;
	}
}