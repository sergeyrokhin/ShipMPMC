#pragma once
// https://habr.com/ru/articles/352374/

#include <list>
#include <string>
#include <sstream>
#include <iostream>
#include <mutex>
#include <memory>
#include <thread>
#include <optional>
#include <condition_variable>
#include <algorithm>
#include <atomic>

#include "ship.h"
namespace prod_cons
{
    class Tunnel
    {

        // у нас кораблики не тяжелые, но все равно, копировать их не хорошо.
        // организовывать самописный RAII - не в этом примере, используем shared_ptr
        std::list<std::shared_ptr<Ship>> store_;
        std::mutex store_guard_;
        std::condition_variable not_empty_not_full_;
        static const int maxShipsInTunel = 5;
        static const int minShipsInTunel = 0;
        bool generator_finished_ = false;

    public:
        Tunnel() = default;
        Tunnel(const Tunnel&) = delete;
        Tunnel(const Tunnel&&) = delete;
        Tunnel operator=(const Tunnel&) = delete;
        Tunnel operator=(const Tunnel&&) = delete;
        void SetFinish() { generator_finished_ = true; }
        bool GetFinish() { return generator_finished_; }

    public:
        bool Add(Size size, Type type)
        {

            try
            {
                std::unique_lock<std::mutex> lock(store_guard_);

                while(store_.size() >= maxShipsInTunel) {
                    std::cout << store_.size() << "> There is no place for a ship in the tunnel: " << std::this_thread::get_id() << std::endl;
                    not_empty_not_full_.wait(lock);
                }
                if (store_.size() < maxShipsInTunel)
                {
                    store_.emplace_back(std::make_shared<Ship>(size, type));
                    auto element = store_.back();
                    std::cout << store_.size() << " + The ship arrived in the tunnel: " << element->getType() << ' ' << static_cast<int>(element->getSize()) << ' ' << std::this_thread::get_id() << std::endl;
                    not_empty_not_full_.notify_all();
                }
            }
            catch (const std::exception &ex)
            {
                std::cout << ex.what() << std::endl;
            }
            return true;
        }

        std::optional<std::shared_ptr<Ship>> get(Type shipType)
        {
            using namespace std::chrono_literals;

            try
            {
                std::unique_lock<std::mutex> lock(store_guard_);
                while(store_.size()  <= minShipsInTunel)
                {
                    if (GetFinish())
                    {
                        break;
                    }
                    
                    std::cout << "0 < There are no ships in the tunnel " << shipType << std::endl;
                    not_empty_not_full_.wait_for(lock, 1s);
                }

                    auto ship_it = std::find_if(store_.begin(), store_.end(), [&shipType](auto ship_ptr) {return *(ship_ptr) == shipType;});
                    if(ship_it != store_.end())
                    {
                            std::cout << store_.size() << "- The ship is taken from the tunnel: " << std::this_thread::get_id() << " " << shipType << std::endl;
                            auto ship_ptr(*ship_it);
                            store_.erase(ship_it);
                            not_empty_not_full_.notify_all();
                            return ship_ptr;
                    }
            }
            catch (const std::exception &ex)
            {
                std::cout << ex.what() << std::endl;
            }
            return std::nullopt;
        }
    };
}