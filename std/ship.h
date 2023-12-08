#pragma once

#include <ostream>

// https://habr.com/ru/articles/352374/

namespace prod_cons
{
    enum class Size
    {
        Small = 50,
        Medium = 100,
        Large = 150
    };
    enum class Type
    {
        Bread = 1,
        Bananas,
        Clothes
    };

    std::ostream& operator<< (std::ostream &out, const Type type);

    class Ship
    {
        int count_ = 0;
        Size size_;
        Type type_;
        Ship(const Ship&) = delete;
        Ship() = delete;
        Ship operator=(const Ship&) = delete;

    public:
        Ship(Size size, Type type) : size_(size), type_(type)
        {
        }

        void Add(int count)
        {
            count_ += count;
        }

        bool countCheck() const
        {
            if (count_ >= static_cast<int>(size_))
            {
                return false;
            }
            return true;
        }

        int getCount() const
        {
            return count_;
        }

        Type getType() const
        {
            return type_;
        }

        Size getSize() const
        {
            return size_;
        }
        bool operator==(Type type) {
            return type_ == type;
        }
    };

    Type getRandomType();
    Size getRandomSize();
}