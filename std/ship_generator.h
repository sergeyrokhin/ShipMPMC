#pragma once

#include <thread>
#include <chrono>

#include "tunnel.h"
namespace prod_cons
{
    class ShipGenerator
    {

        Tunnel &tunnel;
        /// @brief Сколько будет создано корабликов до окончания навигации
        int shipCount;

    public:
        ShipGenerator(Tunnel &tunnel, int shipCount) : tunnel(tunnel), shipCount(shipCount)
        {
        }

        void run()
        {
            using namespace std::chrono_literals;
            int count_ = 0;
            while (count_ < shipCount)
            {
                // Добавляем новый кораблик, пауза
                count_++;
                // и помещаем (как только будет свободное место) в тоннель
                try
                {
                    tunnel.Add(getRandomSize(), getRandomType());
                    std::this_thread::sleep_for(1000ms);
                }
                catch (const std::exception &ex)
                {
                    //при создании кораблика может и не хватить памяти, тогда нужно будет ловить.
                    // выброс в потоке ловить можно будет только в этом же потоке
                    std::cout << ex.what() << std::endl;
                }
            }
            // все кораблики созданы
            std::cout << "Generator finished" << std::endl;
            tunnel.SetFinish();
        }
    };
}