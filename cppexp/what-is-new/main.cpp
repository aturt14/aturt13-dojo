#include <iostream>
#include <array>
#include <limits>
#include <stdexcept>

constexpr size_t SHAPES_SIZE = 100;

class Shape {
public:
    Shape() {
        std::cout << "Your shape was successfully constructed!" << std::endl;
    }
    ~Shape() {
        std::cout << "Your shape was successfully destroyed!" << std::endl;
    }
public:
    char name[64];
    char bowers_style_acronym[16];
    int rank;  
    std::uint32_t vertices;
};

template <typename num_t>
bool read_number(num_t& num) {
    if(!(std::cin >> num)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    return true;
}

size_t get_index() {
    size_t idx;
    std::cout << "Enter index: " << std::endl;
    if(!read_number(idx)) {
        throw std::runtime_error("Index is a number!");
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (idx >= SHAPES_SIZE) {
        throw std::out_of_range("Index out of range!");
    }
    return idx;
}

size_t get_shape_count(size_t idx) {
    size_t shape_count;
    std::cout << "Enter shape count:" << std::endl;
    if (!read_number(shape_count)) {
        throw std::runtime_error("Shape count is a number!");
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (idx >= SHAPES_SIZE) {
        throw std::out_of_range("Shape count out of range!");
    }
    return shape_count;
}

void fill_shape(Shape& s) {
    std::cout << "Enter shape name:" << std::endl;
    std::cin.getline(s.name, sizeof(s.name));
    std::cout << "Enter Bower's style acronym of the shape:" << std::endl;
    std::cin.getline(s.bowers_style_acronym, sizeof(s.bowers_style_acronym));
    std::cout << "Enter shape rank:" << std::endl;
    if (!read_number(s.rank)) {
        throw std::runtime_error("Shape rank is a number!");
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Isn't C++ wonderful?
    std::cout << "Enter shape vertices count:" << std::endl;
    if (!read_number(s.vertices)) {
        throw std::runtime_error("Vertices count is a number!");
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void do_add_shape(std::array<Shape*, SHAPES_SIZE>& shapes, std::array<bool, SHAPES_SIZE>& allocated) {
    size_t idx = get_index();
    if (allocated[idx]) {
        std::cerr << "Not going to leak memory!" << std::endl;
        return;
    }
    shapes[idx] = new Shape;
    fill_shape(*shapes[idx]);
    allocated[idx] = true;
}

void do_add_shapes(std::array<Shape*, SHAPES_SIZE>& shapes, std::array<bool, SHAPES_SIZE>& allocated) {
    size_t idx = get_index();
    if (allocated[idx]) {
        std::cerr << "Not going to leak memory!" << std::endl;
        return;
    }
    size_t sc = get_shape_count(idx);
    shapes[idx] =  new Shape[sc];
    allocated[idx] = true;
    for (size_t i = 0; i < sc; i++) {
        std::cout << "Shape no. " << i << std::endl;
        fill_shape(shapes[idx][i]);
    }
}

void do_remove_shape(std::array<Shape*, SHAPES_SIZE>& shapes, std::array<bool, SHAPES_SIZE>& allocated) {
    size_t idx = get_index();
    if (!allocated[idx]) {
        throw std::runtime_error("Attempted double-free detected!");
    }
    delete shapes[idx];
    shapes[idx] = nullptr;
    allocated[idx] = false;
}

void do_remove_shapes(std::array<Shape*, SHAPES_SIZE>& shapes, std::array<bool, SHAPES_SIZE>& allocated) {
    size_t idx = get_index();
    if (!allocated[idx]) {
        throw std::runtime_error("Attempted double-free detected!");
    }
    delete[] shapes[idx];
    shapes[idx] = nullptr;
    allocated[idx] = false;
}

void menu() {
    std::cout << "add_shape/add_shapes/remove_shape/remove_shapes\n";
    std::cout << ">> " << std::flush;
}

void challenge() {
    std::array<Shape*, SHAPES_SIZE> shapes{};
    std::array<bool, SHAPES_SIZE> allocated{};

    std::string cmd;
    while (true) {
        menu();
        std::getline(std::cin, cmd);
        if (cmd == "add_shape") {
            do_add_shape(shapes, allocated);
        }
        else if (cmd == "add_shapes") {
            do_add_shapes(shapes, allocated);
        }
        else if (cmd == "remove_shape") {
            do_remove_shape(shapes, allocated);
        }
        else if (cmd == "remove_shapes") {
            do_remove_shapes(shapes, allocated);
        }
    }
}


int main(int argc, char* argv[]) {
    challenge();

    return 0;
}
