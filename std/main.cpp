#include <random>
#include <string>
#include <utility>

#include "tunnel.h"
#include "ship_generator.h"
#include "preloader.h"

namespace prod_cons
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    // добавил имя для потока, но никак это имя не использовал.
    // просто поупражнялся с шаблоном
    class ThreadWithName : public std::thread
    {
    public:
        std::string name_;

        template <class... Args>
        ThreadWithName(const std::string &name, Args... args) : std::thread(args...), name_(name) {}
    };

    // static std::vector<Type> ALL_TYPES({Type::Bananas, Type::Bread, Type::Clothes});
    static std::vector<Size> ALL_SIZES({Size::Large, Size::Medium, Size::Small});

    Type getRandomType()
    {
        std::uniform_int_distribution<size_t> dis(1, 3);
        return static_cast<Type>(dis(gen));
    }

    Size getRandomSize()
    {
        std::uniform_int_distribution<size_t> dis(0, ALL_SIZES.size() - 1);
        return ALL_SIZES[dis(gen)];
    }
}
int main()
{
    using namespace prod_cons;
    using namespace std::literals;

    Tunnel tunnel;
    ShipGenerator shipGenerator(tunnel, 10);
    PierLoader pierLoader1(tunnel, Type::Clothes);
    PierLoader pierLoader2(tunnel, Type::Bananas);
    PierLoader pierLoader3(tunnel, Type::Bread);

    ThreadWithName t0("shipGenerator", &ShipGenerator::run, std::ref(shipGenerator));
    //std::thread t0(&ShipGenerator::run, std::ref(shipGenerator));

    ThreadWithName t1("Clothes"s, &PierLoader::run, std::ref(pierLoader1));
    ThreadWithName t2("Bananas"s, &PierLoader::run, std::ref(pierLoader2));
    ThreadWithName t3("Bread"s, &PierLoader::run, std::ref(pierLoader3));

    t0.join();
    t1.join();
    t2.join();
    t3.join();
}