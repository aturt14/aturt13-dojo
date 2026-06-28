#include <cmath>
#include <cstring>
#include <iostream>
#include <array>
#include <limits>
#include <stdexcept>
#include <vector>

constexpr size_t SHAPES_SIZE = 100;

struct Point7D {
    double coords[7];
};

class Polytope {
public:
    Polytope(const char* n, const char* acronym, int r, std::uint32_t v): rank(r), vertices(v) {
        std::strncpy(name, n, 64);
        std::strncpy(bowers_style_acronym, acronym, 16);
    }
    
    Polytope(): rank(0), vertices(0) {
        std::memset(name, 0, sizeof(name));
        std::memset(bowers_style_acronym, 0, sizeof(bowers_style_acronym));
    }
    
    virtual ~Polytope() = 0; 

public:
    char name[64];
    char bowers_style_acronym[16];
    int rank;  
    std::uint32_t vertices;
};

Polytope::~Polytope() {}

// We need a non-abstract class to be able to allocate such object...
class Shape: public Polytope {
public:
    Shape(): Polytope() {}
    ~Shape() override {}
};

class Pentacosihexacontahexadecahecatondodecaexon: public Polytope {
public:
    Pentacosihexacontahexadecahecatondodecaexon() 
        : Polytope("Pentacosihexacontahexadecahecatondodecaexon", "Shit", 7, 56) {
        m_vertex_coords = new Point7D[vertices]; // Allocate memory for vertices
        
        for (std::uint32_t i = 0; i < vertices; ++i) {
            double angle = i * (2.0 * M_PI / vertices);
            m_vertex_coords[i].coords[0] = std::cos(angle);
            m_vertex_coords[i].coords[1] = std::sin(angle);
            m_vertex_coords[i].coords[2] = std::cos(angle * 2);
            m_vertex_coords[i].coords[3] = std::sin(angle * 2);
            m_vertex_coords[i].coords[4] = std::cos(angle * 3);
            m_vertex_coords[i].coords[5] = std::sin(angle * 3);
            m_vertex_coords[i].coords[6] = 1.0; 
        }
    }

    ~Pentacosihexacontahexadecahecatondodecaexon() override {
        // Don't forget to free vertices
        delete[] m_vertex_coords;
    }

    void render_to_terminal(int width = 60, int height = 20) const {
        std::vector<std::string> grid(height, std::string(width, ' '));

        int x = 0; 
        int y = 1;

        for (std::uint32_t i = 0; i < vertices; ++i) {
            double x7d = m_vertex_coords[i].coords[x];
            double y7d = m_vertex_coords[i].coords[y];

            int sx = static_cast<int>((x7d + 1.0) * 0.5 * (width - 1));
            int sy = static_cast<int>((y7d + 1.0) * 0.5 * (height - 1));

            if (sx >= 0 && sx < width && sy >= 0 && sy < height) {
                grid[sy][sx] = '*';
            }
        }

        std::cout << "\n--- Terminal Render of " << name << " ---\n";
        for (const auto& row: grid) {
            std::cout << row << "\n";
        }
        std::cout << "--------------------------------------------------------\n";
    }

private:
    Point7D* m_vertex_coords;
};

template <typename num_t>
bool read_number(num_t& num) {
    if(!(std::cin >> num)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Isn't C++ wonderful?
        return false;
    }
    return true;
}

size_t get_index() {
    size_t idx;
    std::cout << "Enter index:" << std::endl;
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
        throw std::runtime_error("Polytope count is a number!");
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (idx >= SHAPES_SIZE) {
        throw std::out_of_range("Polytope count out of range!");
    }
    return shape_count;
}

void fill_shape(Polytope& s) {
    std::cout << "Enter shape name:" << std::endl;
    std::cin.getline(s.name, sizeof(s.name));
    std::cout << "Enter Bower's style acronym of the shape:" << std::endl;
    std::cin.getline(s.bowers_style_acronym, sizeof(s.bowers_style_acronym));
    std::cout << "Enter shape rank:" << std::endl;
    if (!read_number(s.rank)) {
        throw std::runtime_error("Polytope rank is a number!");
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    std::cout << "Enter shape vertices count:" << std::endl;
    if (!read_number(s.vertices)) {
        throw std::runtime_error("Vertices count is a number!");
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void do_add_shape(std::array<Polytope*, SHAPES_SIZE>& shapes, std::array<bool, SHAPES_SIZE>& allocated) {
    size_t idx = get_index();
    if (allocated[idx]) {
        std::cerr << "Not going to leak memory!" << std::endl;
        return;
    }
    shapes[idx] = new Shape();
    fill_shape(*shapes[idx]);
    allocated[idx] = true;
}

void do_add_shapes(std::array<Polytope*, SHAPES_SIZE>& shapes, std::array<bool, SHAPES_SIZE>& allocated) {
    size_t idx = get_index();
    if (allocated[idx]) {
        std::cerr << "Not going to leak memory!" << std::endl;
        return;
    }
    size_t sc = get_shape_count(idx);
    shapes[idx] =  new Shape[sc];
    allocated[idx] = true;
    for (size_t i = 0; i < sc; i++) {
        std::cout << "Polytope no. " << i << std::endl;
        fill_shape(shapes[idx][i]);
    }
}

void do_add_complex_shape(std::array<Polytope*, SHAPES_SIZE>& shapes, std::array<bool, SHAPES_SIZE>& allocated) {
    size_t idx = get_index();
    if (allocated[idx]) {
        std::cerr << "Not going to leak memory!" << std::endl;
        return;
    }
    shapes[idx] = new Pentacosihexacontahexadecahecatondodecaexon();
    allocated[idx] = true;
}

void do_render_complex_shape(std::array<Polytope*, SHAPES_SIZE>& shapes, std::array<bool, SHAPES_SIZE>& allocated) {
    size_t idx = get_index();
    if (!allocated[idx] || shapes[idx] == nullptr) {
        std::cerr << "I have nothing!" << std::endl;
        return;
    }
    auto* complex_shape = dynamic_cast<Pentacosihexacontahexadecahecatondodecaexon*>(shapes[idx]);
    if (complex_shape) {
        complex_shape->render_to_terminal();
    }
    else {
        std::cerr << shapes[idx]->name << " is not a Pentacosihexacontahexadecahecatondodecaexon!" << std::endl;
    }
}

void do_remove_shape(std::array<Polytope*, SHAPES_SIZE>& shapes, std::array<bool, SHAPES_SIZE>& allocated) {
    size_t idx = get_index();
    if (!allocated[idx]) {
        throw std::runtime_error("Attempted double-free detected!");
    }
    delete shapes[idx];
    shapes[idx] = nullptr;
    allocated[idx] = false;
}

void do_remove_shapes(std::array<Polytope*, SHAPES_SIZE>& shapes, std::array<bool, SHAPES_SIZE>& allocated) {
    size_t idx = get_index();
    if (!allocated[idx]) {
        throw std::runtime_error("Attempted double-free detected!");
    }
    delete[] shapes[idx];
    shapes[idx] = nullptr;
    allocated[idx] = false;
}

void menu() {
    std::cout << "add_shape/add_shapes/add_complex/render_complex/remove_shape/remove_shapes\n";
    std::cout << ">> " << std::flush;
}

void challenge() {
    std::array<Polytope*, SHAPES_SIZE> shapes{};
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
        else if (cmd == "add_complex") {
            do_add_complex_shape(shapes, allocated);
        }
        else if (cmd == "render_complex") {
            do_render_complex_shape(shapes, allocated);
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
