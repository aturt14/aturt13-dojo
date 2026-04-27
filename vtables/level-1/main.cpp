#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Animal {
public:
    Animal(const std::string& name): m_name(name) {}
    Animal(std::string&& name): m_name(std::move(name)) {}
    virtual void make_sound() const {
        std::cout << m_name << ": eek" << std::endl;
    }
    const std::string& get_name() const {
        return m_name;
    }
    virtual ~Animal() = default;
protected:
    std::string m_name;
};

class Cat: public Animal {
public:
    Cat(const std::string& name): Animal(name) {}
    void make_sound() const override {
        std::cout << m_name << ": meow!" << std::endl;
    }
};

class Dog: public Animal {
public:
    Dog(const std::string& name): Animal(name) {}
    void make_sound() const override {
        std::cout << m_name << ": haf!" << std::endl;
    }
};

class Pig: public Animal {
public:
    Pig(const std::string& name): Animal(name) {}
    void make_sound() const override {
        std::cout << m_name << ": sqeak!" << std::endl;
    }
};

using Farm = std::vector<std::unique_ptr<Animal>>;

std::string get_name() {
    std::string name;
    std::cout << "Name:" << std::endl;
    std::getline(std::cin, name);
    return name;
}

std::string get_type() {
    std::string type;
    std::cout << "Animal type:" << std::endl;
    std::getline(std::cin, type);
    return type;
}

void do_add_animal(Farm& farm) {
    std::string name = get_name();
    std::string type = get_type();
    if (type == "cat") {
        farm.push_back(std::make_unique<Cat>(name));
    }
    else if (type == "dog") {
        farm.push_back(std::make_unique<Dog>(name));
    }
    else {
        farm.push_back(std::make_unique<Pig>(name));
    }
}

void die(const Animal& animal) {
    animal.make_sound();
    animal.make_sound();
    std::cout << animal.get_name() << " died in pain." << std::endl;
}

void do_kill_animal(Farm& farm) {
    std::string name = get_name();
    for (std::size_t i = 0; i < farm.size(); i++) {
        if (farm[i]->get_name() == name) {
            die(*farm[i]);
            farm.erase(farm.begin() + i);
            return;
        }
    }
}

void do_write_msg(std::string& message) {
    std::cout << "Write your message:" << std::endl;
    std::getline(std::cin, message);
}

void do_read_msg(std::string& message) {
    std::cout << message << std::endl;
}

bool menu(Farm& farm, std::string& message) {
    std::string option;
    std::cout << "What do you wish to undertake?\n"
              << "add_animal/kill_animal/write_msg/read_msg/exit\n";
    std::cout << ">> " << std::flush;
    std::getline(std::cin, option);
    if (option == "add_animal") {
        do_add_animal(farm);
    }
    else if (option == "kill_animal") {
        do_kill_animal(farm);
    }
    else if (option == "write_msg") {
        do_write_msg(message);
    }
    else if (option == "read_msg") {
        do_read_msg(message);
    }
    else {
        return false;
    }
    return true;
}

void do_farm_things(Farm& farm) {
    std::string message;
    while (true) {
        for (const auto& animal: farm) {
            animal->make_sound();
            if (!menu(farm, message)) {
                return;
            }
        }
    }
}


int main(int argc, char* argv[]) {
    Farm farm;

    farm.push_back(std::make_unique<Cat>("Prinzessin"));
    farm.push_back(std::make_unique<Dog>("Servac"));
    farm.push_back(std::make_unique<Pig>("Napoleon"));

    do_farm_things(farm);

    return 0;
}
