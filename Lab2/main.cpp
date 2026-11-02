import std;
import LinkedList;

int main() {
    try {
        unsigned int random_seed = std::random_device{}();
        LinkedList<int> list(5, 10, 99, random_seed);
        std::cout << "Оригинальный список: " << list << " (Размер: " << list.get_size() << ")\n";

        list.push_head(7);
        list.push_tail(7);
        std::cout << "После push_head(7)/tail(7):  " << list << "\n";

        list.reverse();
        std::cout << "Развернутый список:   " << list << "\n";

        list.delete_node(7);
        std::cout << "После удаления всех 7: " << list << "\n";

        std::cout << "Элемент под индексом 2: " << list[2] << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка " << e.what() << "\n";
    }
    return 0;
}
