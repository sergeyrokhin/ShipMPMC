#include <random>

#include "ship.h"
static std::random_device rd;
static std::mt19937 gen(rd());

using namespace prod_cons;

static std::vector<Type> ALL_TYPES({Type::Bananas, Type::Bread, Type::Clothes});
static std::vector<Size> ALL_SIZES({Size::Large, Size::Medium, Size::Small});

Type getRandomType()
{
    std::uniform_int_distribution<size_t> dis(0, ALL_TYPES.size() - 1);
    return ALL_TYPES[dis(gen)];
}

Size getRandomSize()
{
    std::uniform_int_distribution<size_t> dis(0, ALL_SIZES.size() - 1);
    return ALL_SIZES[dis(gen)];
}

std::ostream & prod_cons::operator<<(std::ostream &out, const Type type)
{
    switch (type)
    {
    case Type::Bananas:
        out << "Bananas";
        break;
    case Type::Bread:
        out << "Bread";
        break;
    case Type::Clothes:
        out << "Clothes";
        break;
    default:
        break;
    }
    return out;
}
