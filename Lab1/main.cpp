import std;
import Polynomial;

int main() {
    try {
        std::cout << "Вариант 17, Многочлены (5)\n\n";

        std::cout << "1: Конструкторы и вывод (operator<<)\n";
        Polynomial<double> p_empty;
        std::cout << "Конструктор по умолчанию: " << p_empty << "\n";
        Polynomial<double> p_degree(3);
        std::cout << "Заданная степень 3 (ожидается 0): " << p_degree << "\n";
        double coeffs1[] = { 7.0, -4.0, 1.0 };
        Polynomial<double> p1(coeffs1, 3);
        std::cout << "Из массива (ожидается x^2 + -4x + 7): " << p1 << "\n\n";

        std::cout << "2: Информационные методы и доступ\n";
        std::cout << "Степень P(x) (ожидается 2): " << p1.degree() << "\n";
        std::cout << "Коэффициент при x^0 (ожидается 7): " << p1[0] << "\n";
        std::cout << "Коэффициент при x^1 (ожидается -4): " << p1[1] << "\n";
        try {
            std::cout << "Попытка получить коэффициент при x^5: ";
            std::cout << p1[5] << "\n";
        }
        catch (const std::out_of_range& e) {
            std::cout << "[Ожидаемое исключение]: " << e.what() << "\n";
        }
        std::cout << "\n";

        std::cout << "3: Модификация (expand, set, shrink_to_fit)\n";
        double coeffs_mod[] = { 3.0 };
        Polynomial<double> p_mod(coeffs_mod, 1);
        std::cout << "Многочлен: " << p_mod << "\n";
        p_mod.expand(2);
        std::cout << "После expand(2) (ожидается 3): " << p_mod << "\n";
        p_mod.set(2, 5.0);
        std::cout << "После set(2, 5) (ожидается 5x^2 + 3): " << p_mod << "\n";
        p_mod.set(2, 0.0);
        std::cout << "После зануления старшего члена через set (ожидается 3): " << p_mod << "\n\n";

        std::cout << "4: Вычисление значения (Схема Горнера)\n";
        double val = p1.evaluate(2.0);
        std::cout << "Значение P(2) = " << p1 << " (ожидается 3): " << val << "\n\n";

        std::cout << "5: (+=, -=, *=, +, -, *)\n";
        double coeffs2[] = { -3.0, 4.0, -1.0 };
        Polynomial<double> p2(coeffs2, 3);
        std::cout << "Многочлен P(x): " << p1 << "\n";
        std::cout << "Многочлен Q(x): " << p2 << "\n";
        auto p_sum = p1 + p2;
        std::cout << "Сумма P(x) + Q(x) (ожидается 4): " << p_sum << "\n";
        std::cout << "Новая степень суммы (shrink_to_fit убрал x^2 и x): " << p_sum.degree() << "\n";

        auto p_diff = p1 - p2;
        std::cout << "Разность P(x) - Q(x) (ожидается 2x^2 + -8x + 10): " << p_diff << "\n";

        auto p_mul_right = p1 * 2.0;
        auto p_mul_left = 2.0 * p1;
        std::cout << "Умножение справа P(x) * 2: " << p_mul_right << "\n";
        std::cout << "Умножение слева 2 * P(x): " << p_mul_left << "\n\n";

        std::cout << "6: Локальный экстремум степени 2\n";
        // Для x^2 - 4x + 7 экстремум в x = -b/(2a) = 4/2 = 2. y = P(2) = 3
        const auto [x_ext, y_ext] = find_extremum(p1);
        std::cout << "Экстремум x^2 - 4x + 7 найден в точке: (" << x_ext << ", " << y_ext << ")\n\n";

        std::cout << "7: Случайный объект класса\n";
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(-5.0, 5.0);

        double random_data[3];
        random_data[0] = dis(gen);
        random_data[1] = dis(gen);
        double a_val = 0.0;
        while (std::abs(a_val) < 0.01) {
            a_val = dis(gen);
        }
        random_data[2] = a_val;
        Polynomial<double> p_rand(random_data, 3);
        std::cout << "   Случайный полином p_rand: " << p_rand << "\n";
        std::cout << "   Степень p_rand: " << p_rand.degree() << "\n";

        Polynomial<double> p_rand_copy(p_rand);
        Polynomial<double> p_rand_set;
        p_rand_set = p_rand;
        p_rand_set.set(1, 0.0);
        std::cout << "   p_rand == копия: " << (p_rand == p_rand_copy) << "\n";
        std::cout << "   p_rand != модифицированный: " << (p_rand != p_rand_set) << "\n";
        try {
            auto [rx, ry] = find_extremum(p_rand);
            std::cout << "   Экстремум p_rand найден в: (" << rx << ", " << ry << ")\n";
        }
        catch (const std::exception& e) {
            std::cout << "   [Ошибка экстремума случайного полинома]: " << e.what() << "\n";
        }
        std::cout << "\n";

        std::cout << "Проверка find_extremum на ошибку степени: \n";
        find_extremum(p_sum);
    }
    catch (const std::invalid_argument& e) {
        std::cout << "\nУспешно перехвачено ожидаемое исключение: " << e.what() << "\n";
    }
    catch (const std::exception& e) {
        std::cout << "\nНепредвиденная ошибка: " << e.what() << "\n";
    }
    return 0;
}
