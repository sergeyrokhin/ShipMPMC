#pragma once

#include <thread>
#include <chrono>

#include "tunnel.h"
namespace prod_cons
{
    class PierLoader
    {
        Tunnel &tunnel;
        Type shipType;

    public:
        PierLoader(Tunnel &tunnel, Type shipType) : tunnel(tunnel), shipType(shipType) {}

        void run()
        {
            using namespace std::chrono_literals;

            while (true)
            {
                try
                {
                    // Thread.currentThread().setName("Loader "+shipType);

                    // Time to load the goods
                    std::this_thread::sleep_for(500ms);
                    auto ship_opt = tunnel.get(shipType);
                    if (ship_opt)
                    {
                        Ship& ship = *(ship_opt.value().get());
                        // грузим частиями, по 10, пока не заполним
                        while (ship.countCheck())
                        {
                            std::this_thread::sleep_for(100ms);
                            ship.Add(10);
                            std::cout << ship.getCount() << " Loaded ship. " << shipType << " " << std::this_thread::get_id() << std::endl;
                        }
                        std::cout << "Finished load ship. " << shipType << " " << std::this_thread::get_id() << std::endl;
                    }
                    else if (tunnel.GetFinish())
                    {
                        std::cout << "Preloader finished . " << shipType << " " << std::this_thread::get_id() << std::endl;
                        return;
                    }
                }
                catch (const std::exception &ex)
                {
                    std::cout << ex.what() << std::endl;
                }
            }
        }
    };
}