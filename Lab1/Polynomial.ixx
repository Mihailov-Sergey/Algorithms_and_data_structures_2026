export module Polynomial;

export{
	template <typename T>
	class Polynomial {
	private:
		std::vector<T> _coeff;
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
		Polynomial() : _coeff({ static_cast<T>(0) }) {}
		Polynomial(size_t max_degree);
		Polynomial(const std::vector<T>& coefficients) : _coeff(coefficients) {
			if (_coeff.empty()) {
				_coeff.push_back(static_cast<T>(0));
			}
		}
		T operator[](size_t index) const;
		void set(size_t degree, const T& value);
		template <typename X>
		auto evaluate(const X& x) const;
		Polynomial operator+(const Polynomial& other) const;
		Polynomial operator-(const Polynomial& other) const;
		Polynomial operator*(const T& scalar) const;
		size_t degree() const {
			return _coeff.size() - 1;
		}
		shrink_to_fit();
		expand();
	};
	template <typename T>
	Polynomial<T> operator*(const T& scalar, const Polynomial& poli) {
		return poli * scalar;
	}
}