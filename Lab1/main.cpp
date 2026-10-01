import std;
import Polynomial;

int main() {
    try {
        Polynomial<double> p_empty;
        std::cout << "Конструктор по умолчанию: " << p_empty << "\n";

        Polynomial<double> p(std::vector<double>{7.0, -4.0, 1.0});
        const auto [x, y] = find_extremum(p);
        std::cout << "Точка локального экстремума: (" << x << ", " << y << ")\n";
    }
    catch (const std::invalid_argument& e) {
        std::println("\nУспешно перехвачено ожидаемое исключение: {}", e.what());
    }
    catch (const std::exception& e) {
        std::println(std::cerr, "\nНепредвиденная ошибка: {}", e.what());
    }

    return 0;
}