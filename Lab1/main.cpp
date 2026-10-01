import std;
import Polynomial;

int main() {
    try {
        std::println("Вариант 17, Многочлены (5)");
        std::println("");

        std::println("1: Конструкторы и вывод (operator<<)");
        Polynomial<double> p_empty;
        std::cout << "Конструктор по умолчанию: " << p_empty << "\n";
        Polynomial<double> p_degree(3);
        std::cout << "Заданная степень 3 (ожидается 0): " << p_degree << "\n";
        Polynomial<double> p1(std::vector<double>{7.0, -4.0, 1.0});
        std::cout << "Из вектора (ожидается x^2 + -4x + 7): " << p1 << "\n";
        std::println("");

        std::println("2: Информационные методы и доступ");
        std::println("Степень P(x) (ожидается 2): {}", p1.degree());
        std::println("Коэффициент при x^0 (ожидается 7): {}", p1[0]);
        std::println("Коэффициент при x^1 (ожидается -4): {}", p1[1]);
        std::println("Коэффициент при x^5 вне границ (ожидается 0): {}", p1[5]);
        std::println("");

        std::println("3: Модификация (expand, set, shrink_to_fit)");
        Polynomial<double> p_mod(std::vector<double>{3.0});
        p_mod.expand(2);
        std::cout << "После expand(2) (ожидается 3): " << p_mod << "\n";
        p_mod.set(2, 5.0);
        std::cout << "После set(2, 5) (ожидается 5x^2 + 3): " << p_mod << "\n";
        p_mod.set(2, 0.0);
        std::cout << "После зануления старшего члена через set (ожидается 3): " << p_mod << "\n";
        std::println("");

        std::println("4: Вычисление значения (Схема Горнера)");
        double val = p1.evaluate(2.0);
        std::println("Значение P(2) (ожидается 3): {}", val);
        std::println("");

        std::println("5: (+=, -=, *=, +, -, *)");
        Polynomial<double> p2(std::vector<double>{-3.0, 4.0, -1.0});
        std::cout << "Многочлен Q(x): " << p2 << "\n";
        auto p_sum = p1 + p2;
        std::cout << "Сумма P(x) + Q(x) (ожидается 4): " << p_sum << "\n";
        std::println("Новая степень суммы (shrink_to_fit убрал x^2): {}", p_sum.degree());
        auto p_diff = p1 - p2;
        std::cout << "Разность P(x) - Q(x) (ожидается 2x^2 + -8x + 10): " << p_diff << "\n";
        auto p_mul_right = p1 * 2.0;
        auto p_mul_left = 2.0 * p1;
        std::cout << "Умножение справа P(x) * 2: " << p_mul_right << "\n";
        std::cout << "Умножение слева 2 * P(x): " << p_mul_left << "\n";
        std::println("");

        std::println("6: Локальный экстремум степени 2");
        // Для x^2 - 4x + 7 экстремум в x = -b/(2a) = 4/2 = 2. y = P(2) = 3
        const auto [x_ext, y_ext] = find_extremum(p1);
        std::cout << "Экстремум P(x) найден в точке: (" << x_ext << ", " << y_ext << ")\n";
        std::println("Проверка find_extremum на ошибку степени: ");
        find_extremum(p_sum);
    }
    catch (const std::invalid_argument& e) {
        std::println("\nУспешно перехвачено ожидаемое исключение: {}", e.what());
    }
    catch (const std::exception& e) {
        std::println(std::cerr, "\nНепредвиденная ошибка: {}", e.what());
    }
    return 0;
}