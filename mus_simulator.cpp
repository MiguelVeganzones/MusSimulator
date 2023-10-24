#include "Precision_totalizer.h"
#include "mus.h"
#include "phmap.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <execution>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <unordered_map>
#include <utility>
#include <vector>

struct data_totalizer
{
    precision_totalizer big_stats;
    precision_totalizer small_stats;
    precision_totalizer pairs_stats_1;
    precision_totalizer pairs_stats_2;
    precision_totalizer game_stats_1;
    precision_totalizer game_stats_2;
    precision_totalizer point_stats;
    std::size_t         total_count;
    std::size_t         points_count;
    std::size_t         game_count_1;
    std::size_t         game_count_2;
    std::size_t         pairs_count_1;
    std::size_t         pairs_count_2;
};

struct sample
{
    std::size_t count{};
    std::size_t game_count_1{};
    std::size_t game_count_2{};
    std::size_t pairs_count_1{};
    std::size_t pairs_count_2{};
    std::size_t point_count{};
    double      won_big{};
    double      won_small{};
    double      won_pairs_1{};
    double      won_pairs_2{};
    double      won_game_1{};
    double      won_game_2{};
    double      won_point{};
};

// void full_stats(std::size_t samples, std::size_t sample_size)
// {
//     std::mutex m;

//     // std::unordered_map<Hand, data_totalizer, Hand::Hasher> record_data;
//     phmap::flat_hash_map<Hand, data_totalizer, Hand::Hasher> record_data;
//     record_data.reserve(Hand::s_Distinct_posssible_hands);

//     std::vector<int> iterable(samples, 0);
//     std::iota(iterable.begin(), iterable.end(), 0);

//     int i__ = 0;

//     // for (auto iter__ = 0uz; iter__ != samples; ++iter__)
//     // {
//     std::for_each(
//         std::execution::par_unseq,
//         std::begin(iterable),
//         std::end(iterable),
//         [&i__, &m, samples, sample_size, &record_data](int iter__) {
//             phmap::flat_hash_map<Hand, sample, Hand::Hasher> data_sample;
//             data_sample.reserve(Hand::s_Distinct_posssible_hands);
//             Deck deck;
//             for (auto iteration = 0u; iteration != sample_size; ++iteration)
//             {
//                 // deck_mutex.lock();
//                 // deck.shuffle();
//                 // auto&& hands = deck.deal_first();
//                 // deck_mutex.unlock();

//                 deck.shuffle();
//                 auto&& hands = deck.deal_first();

//                 std::array<float, 4> big{};
//                 std::array<float, 4> small{};
//                 std::array<float, 4> pairs{};
//                 std::array<float, 4> game{};

//                 for (int i = 0; i != 4; ++i)
//                 {
//                     big[i]   = hands[i].get_big_score();
//                     small[i] = hands[i].get_small_score();
//                     pairs[i] = hands[i].get_pair_score();
//                     game[i]  = hands[i].get_game_score();
//                 }

//                 const auto best_big_idx   = std::distance(big.begin(), std::max_element(big.begin(), big.end()));
//                 const auto best_small_idx = std::distance(small.begin(), std::min_element(small.begin(),
//                 small.end())); const auto best_pairs_idx = std::distance(pairs.begin(),
//                 std::max_element(pairs.begin(), pairs.end())); const auto best_game_idx =
//                     std::distance(game.begin(), std::max_element(game.begin(), game.end(), game_compare));

//                 const auto pairs_played =
//                     std::count_if(std::begin(pairs), std::end(pairs), [](auto a) { return a > 1; }) > 1;

//                 const auto game_played =
//                     std::count_if(std::begin(game), std::end(game), [](auto a) { return a > 31; }) > 1;

//                 // for (int i = 0; i != 4; ++i)
//                 // {
//                 //     if (i == best_game_idx)
//                 //     {
//                 //         std::cout << "(" << game[i] << ") ";
//                 //     }
//                 //     else
//                 //     {
//                 //         std::cout << game[i] << " ";
//                 //     }
//                 // }
//                 // std::cout << '\n';

//                 for (int i = 0; i != 4; ++i)
//                 {
//                     auto&& h  = hands[i]; // todo test performance vs auto&& or auto&
//                     auto   it = data_sample.find(h);
//                     if (it == data_sample.end())
//                     {
//                         it = data_sample.insert({ h, sample{} }).first;
//                     }
//                     ++(it->second.count);
//                     if (i == best_big_idx)
//                     {
//                         ++(it->second.won_big);
//                     }
//                     if (i == best_small_idx)
//                     {
//                         ++(it->second.won_small);
//                     }
//                     if (pairs[best_pairs_idx] > 1)
//                     {
//                         it->second.pairs_count_1++;
//                         if (i == best_pairs_idx)
//                         {
//                             ++(it->second.won_pairs_1);
//                             if (pairs_played)
//                             {
//                                 ++(it->second.won_pairs_2);
//                             }
//                         }
//                         if (pairs_played)
//                         {
//                             it->second.pairs_count_2++;
//                         }
//                     }
//                     if (game[best_game_idx] >= 31)
//                     {
//                         it->second.game_count_1++;
//                         if (i == best_game_idx)
//                         {
//                             ++(it->second.won_game_1);
//                             if (game_played)
//                             {
//                                 ++(it->second.won_game_2);
//                             }
//                         }
//                         if (game_played)
//                         {
//                             it->second.game_count_2++;
//                         }
//                     }
//                     else
//                     {
//                         it->second.point_count++;
//                         if (i == best_game_idx)
//                         {
//                             ++(it->second.won_point);
//                         }
//                     }
//                 }
//             }

//             std::lock_guard guard(m);
//             if (iter__ % (samples / 1000) == 0)
//                 std::cout << ++i__ << '\n';
//             for (auto& hand_data : data_sample)
//             {
//                 auto&& hand  = hand_data.first;
//                 auto&& stats = hand_data.second;

//                 auto it = record_data.find(hand);
//                 if (it == record_data.end())
//                 {
//                     it = record_data.insert({ auto(hand), data_totalizer{} }).first;
//                 }
//                 it->second.big_stats.add(stats.won_big / (double)stats.count);
//                 it->second.small_stats.add(stats.won_small / (double)stats.count);
//                 if (stats.pairs_count_1 > 0)
//                     it->second.pairs_stats_1.add(stats.won_pairs_1 / (double)stats.pairs_count_1);
//                 if (stats.pairs_count_2 > 0)
//                     it->second.pairs_stats_2.add(stats.won_pairs_2 / (double)stats.pairs_count_2);
//                 if (stats.game_count_1 > 0)
//                     it->second.game_stats_1.add(stats.won_game_1 / (double)stats.game_count_1);
//                 if (stats.game_count_2 > 0)
//                     it->second.game_stats_2.add(stats.won_game_2 / (double)stats.game_count_2);
//                 if (stats.point_count > 0)
//                     it->second.point_stats.add(stats.won_point / (double)stats.point_count);
//                 it->second.total_count += stats.count;
//                 it->second.pairs_count_1 += stats.pairs_count_1;
//                 it->second.pairs_count_2 += stats.pairs_count_2;
//                 it->second.game_count_1 += stats.game_count_1;
//                 it->second.game_count_2 += stats.game_count_2;
//                 it->second.points_count += stats.point_count;
//             }
//         }
//     );

//     std::ofstream outfile(
//         samples == sample_size
//             ? "data_" + std::to_string(samples) + "_samples.csv"
//             : "data_Samples_" + std::to_string(samples) + "_Samples_size_" + std::to_string(sample_size) + ".csv"
//     );

//     outfile << "Mano,Grande,Stddev_grande,Chica,Stddev_chica,Pares_1,Stddev_pares_1,Pares_2,Stddev_pares_2,Juego_1,"
//                "Stddev_juego_1,Juego_2,Stddev_juego_2,"
//                "Punto,Stddev_punto,Grande_value,Chica_value,Pares_value,Juego_value,Pares_repr,Juego_repr,"
//                "SampleAppearances,Muestras_total,Muestras_pares_1,Muestras_pares_2,Muestras_juego_1,Muestras_juego_2,"
//                "Muestras_punto,Samples,Sample_size\n";
//     for (auto& hand_stats : record_data)
//     {
//         outfile << hand_stats.first << ", " << hand_stats.second.big_stats.get_average() * 100 << ", "
//                 << hand_stats.second.big_stats.get_stddev() * 100 << ", "
//                 << hand_stats.second.small_stats.get_average() * 100 << ", "
//                 << hand_stats.second.small_stats.get_stddev() * 100 << ", "
//                 << hand_stats.second.pairs_stats_1.get_average() * 100 << ", "
//                 << hand_stats.second.pairs_stats_1.get_stddev() * 100 << ", "
//                 << hand_stats.second.pairs_stats_2.get_average() * 100 << ", "
//                 << hand_stats.second.pairs_stats_2.get_stddev() * 100 << ", "
//                 << hand_stats.second.game_stats_1.get_average() * 100 << ", "
//                 << hand_stats.second.game_stats_1.get_stddev() * 100 << ", "
//                 << hand_stats.second.game_stats_2.get_average() * 100 << ", "
//                 << hand_stats.second.game_stats_2.get_stddev() * 100 << ", "
//                 << hand_stats.second.point_stats.get_average() * 100 << ", "
//                 << hand_stats.second.point_stats.get_stddev() * 100 << ", " << hand_stats.first.get_big_score() << ",
//                 "
//                 << hand_stats.first.get_small_score() << ", " << hand_stats.first.get_pair_score() << ", "
//                 << hand_stats.first.get_game_score() << ", " << hand_stats.first.get_pair_repr() << ", "
//                 << hand_stats.first.get_game_repr() << ", " << hand_stats.second.big_stats.samples() << ", "
//                 << hand_stats.second.total_count << ", " << hand_stats.second.pairs_count_1 << ", "
//                 << hand_stats.second.pairs_count_2 << ", " << hand_stats.second.game_count_1 << ", "
//                 << hand_stats.second.game_count_2 << ", " << hand_stats.second.points_count << ", " << samples << ",
//                 "
//                 << sample_size << "\n";
//     }
//     outfile.close();
// }

void full_stats_manual_threading(std::size_t samples, std::size_t sample_size)
{
    std::mutex m;

    phmap::flat_hash_map<Hand, data_totalizer, Hand::Hasher> record_data;
    record_data.reserve(Hand::s_Distinct_posssible_hands);

    std::vector<int> iterable(samples, 0);
    std::iota(iterable.begin(), iterable.end(), 0);

    int            i__          = 0;
    constexpr auto thread_count = 12;

    // for (auto iter__ = 0uz; iter__ != samples; ++iter__)
    // {
    auto f = [samples, sample_size, &m, &i__, &record_data](
                 Deck deck, phmap::flat_hash_map<Hand, sample, Hand::Hasher> data_sample
             ) {
        for (auto thread_samples = 0uz; thread_samples != (samples / thread_count); ++thread_samples)
        {
            data_sample.clear();
            data_sample.reserve(Hand::s_Distinct_posssible_hands);
            for (auto iteration = 0u; iteration != sample_size; ++iteration)
            {
                deck.shuffle();
                auto&& hands = deck.deal_first();

                std::array<float, 4> big{};
                std::array<float, 4> small{};
                std::array<float, 4> pairs{};
                std::array<float, 4> game{};

                // Game and pairs played is independant of board positions
                for (int i = 0; i != 4; ++i)
                {
                    pairs[i] = hands[i].get_pair_score();
                    game[i]  = hands[i].get_game_score();
                }
                const auto pairs_count =
                    std::count_if(std::begin(pairs), std::end(pairs), [](auto a) { return a > 1; });

                const auto game_count = std::count_if(std::begin(game), std::end(game), [](auto a) { return a > 31; });

                for (auto permutation : GameInstance::s_board_positions_permutations)
                {

                    for (int i = 0; i != 4; ++i)
                    {
                        hands[i].set_board_position(permutation[i]);
                        big[i]   = hands[i].get_big_score();
                        small[i] = hands[i].get_small_score();
                        pairs[i] = hands[i].get_pair_score();
                        game[i]  = hands[i].get_game_score();
                    }
                    const auto best_big_idx = std::distance(big.begin(), std::max_element(big.begin(), big.end()));
                    const auto best_small_idx =
                        std::distance(small.begin(), std::min_element(small.begin(), small.end()));
                    const auto best_pairs_idx =
                        std::distance(pairs.begin(), std::max_element(pairs.begin(), pairs.end()));
                    const auto best_game_idx =
                        std::distance(game.begin(), std::max_element(game.begin(), game.end(), game_compare));

                    for (int i = 0; i != 4; ++i)
                    {
                        auto&& h  = hands[i]; // todo test performance vs auto&& or auto&
                        auto   it = data_sample.find(h);
                        if (it == data_sample.end())
                        {
                            it = data_sample.insert({ h, sample{} }).first;
                        }
                        ++(it->second.count);
                        if (i == best_big_idx)
                        {
                            ++(it->second.won_big);
                        }
                        if (i == best_small_idx)
                        {
                            ++(it->second.won_small);
                        }
                        if (pairs_count > 1)
                        {
                            it->second.pairs_count_1++;
                            if (i == best_pairs_idx)
                            {
                                ++(it->second.won_pairs_1);
                                if (pairs_count > 2)
                                {
                                    ++(it->second.won_pairs_2);
                                }
                            }
                            if (pairs_count > 2)
                            {
                                it->second.pairs_count_2++;
                            }
                        }
                        if (game_count > 1)
                        {
                            it->second.game_count_1++;
                            if (i == best_game_idx)
                            {
                                ++(it->second.won_game_1);
                                if (game_count > 2)
                                {
                                    ++(it->second.won_game_2);
                                }
                            }
                            if (game_count > 2)
                            {
                                it->second.game_count_2++;
                            }
                        }
                        else
                        {
                            it->second.point_count++;
                            if (i == best_game_idx)
                            {
                                ++(it->second.won_point);
                            }
                        }
                    }
                }
            }

            std::lock_guard guard(m);
            if (thread_samples % (samples / 1000) == 0)
                std::cout << ++i__ << '\n';
            for (const auto& hand_data : data_sample)
            {
                auto&& hand  = hand_data.first;
                auto&& stats = hand_data.second;

                auto it = record_data.find(hand);
                if (it == record_data.end())
                {
                    it = record_data.insert({ hand, {} }).first;
                }
                it->second.big_stats.add(stats.won_big / (double)stats.count);
                it->second.small_stats.add(stats.won_small / (double)stats.count);
                if (stats.pairs_count_1 > 0)
                    it->second.pairs_stats_1.add(stats.won_pairs_1 / (double)stats.pairs_count_1);
                if (stats.pairs_count_2 > 0)
                    it->second.pairs_stats_2.add(stats.won_pairs_2 / (double)stats.pairs_count_2);
                if (stats.game_count_1 > 0)
                    it->second.game_stats_1.add(stats.won_game_1 / (double)stats.game_count_1);
                if (stats.game_count_2 > 0)
                    it->second.game_stats_2.add(stats.won_game_2 / (double)stats.game_count_2);
                if (stats.point_count > 0)
                    it->second.point_stats.add(stats.won_point / (double)stats.point_count);
                it->second.total_count += stats.count;
                it->second.pairs_count_1 += stats.pairs_count_1;
                it->second.pairs_count_2 += stats.pairs_count_2;
                it->second.game_count_1 += stats.game_count_1;
                it->second.game_count_2 += stats.game_count_2;
                it->second.points_count += stats.point_count;
            }
        }
    };


    std::jthread j1(f, Deck(), phmap::flat_hash_map<Hand, sample, Hand::Hasher>());
    std::jthread j2(f, Deck(), phmap::flat_hash_map<Hand, sample, Hand::Hasher>());
    std::jthread j3(f, Deck(), phmap::flat_hash_map<Hand, sample, Hand::Hasher>());
    std::jthread j4(f, Deck(), phmap::flat_hash_map<Hand, sample, Hand::Hasher>());
    std::jthread j5(f, Deck(), phmap::flat_hash_map<Hand, sample, Hand::Hasher>());
    std::jthread j6(f, Deck(), phmap::flat_hash_map<Hand, sample, Hand::Hasher>());
    std::jthread j7(f, Deck(), phmap::flat_hash_map<Hand, sample, Hand::Hasher>());
    std::jthread j8(f, Deck(), phmap::flat_hash_map<Hand, sample, Hand::Hasher>());
    std::jthread j9(f, Deck(), phmap::flat_hash_map<Hand, sample, Hand::Hasher>());
    std::jthread j10(f, Deck(), phmap::flat_hash_map<Hand, sample, Hand::Hasher>());
    std::jthread j11(f, Deck(), phmap::flat_hash_map<Hand, sample, Hand::Hasher>());
    std::jthread j12(f, Deck(), phmap::flat_hash_map<Hand, sample, Hand::Hasher>());

    j1.join();
    j2.join();
    j3.join();
    j4.join();
    j5.join();
    j6.join();
    j7.join();
    j8.join();
    j9.join();
    j10.join();
    j11.join();
    j12.join();

    std::ofstream outfile(
        samples == sample_size
            ? "data_" + std::to_string(samples) + "_samples.csv"
            : "data_Samples_" + std::to_string(samples) + "_Samples_size_" + std::to_string(sample_size) + ".csv"
    );

    outfile << "Mano,Grande,Stddev_grande,Chica,Stddev_chica,Pares_1,Stddev_pares_1,Pares_2,Stddev_pares_2,Juego_1,"
               "Stddev_juego_1,Juego_2,Stddev_juego_2,"
               "Punto,Stddev_punto,Grande_value,Chica_value,Pares_value,Juego_value,Pares_repr,Juego_repr,"
               "SampleAppearances,Muestras_total,Muestras_pares_1,Muestras_pares_2,Muestras_juego_1,Muestras_juego_2,"
               "Muestras_punto,Samples,Sample_size\n";
    for (const auto& hand_stats : record_data)
    {
        outfile << hand_stats.first << ", " << hand_stats.second.big_stats.get_average() * 100 << ", "
                << hand_stats.second.big_stats.get_stddev() * 100 << ", "
                << hand_stats.second.small_stats.get_average() * 100 << ", "
                << hand_stats.second.small_stats.get_stddev() * 100 << ", "
                << hand_stats.second.pairs_stats_1.get_average() * 100 << ", "
                << hand_stats.second.pairs_stats_1.get_stddev() * 100 << ", "
                << hand_stats.second.pairs_stats_2.get_average() * 100 << ", "
                << hand_stats.second.pairs_stats_2.get_stddev() * 100 << ", "
                << hand_stats.second.game_stats_1.get_average() * 100 << ", "
                << hand_stats.second.game_stats_1.get_stddev() * 100 << ", "
                << hand_stats.second.game_stats_2.get_average() * 100 << ", "
                << hand_stats.second.game_stats_2.get_stddev() * 100 << ", "
                << hand_stats.second.point_stats.get_average() * 100 << ", "
                << hand_stats.second.point_stats.get_stddev() * 100 << ", " << hand_stats.first.get_big_score() << ", "
                << hand_stats.first.get_small_score() << ", " << hand_stats.first.get_pair_score() << ", "
                << hand_stats.first.get_game_score() << ", " << hand_stats.first.get_pair_repr() << ", "
                << hand_stats.first.get_game_repr() << ", " << hand_stats.second.big_stats.samples() << ", "
                << hand_stats.second.total_count << ", " << hand_stats.second.pairs_count_1 << ", "
                << hand_stats.second.pairs_count_2 << ", " << hand_stats.second.game_count_1 << ", "
                << hand_stats.second.game_count_2 << ", " << hand_stats.second.points_count << ", " << samples << ", "
                << sample_size << "\n";
    }
    outfile.close();
}

int main(int argc, char* argv[])
{
    std::size_t samples{ 3000 };
    std::size_t sample_size{ 3000 };

    if (argc == 2)
    {
        samples     = std::stoi(argv[1]);
        sample_size = samples;
    }
    else if (argc == 3)
    {
        samples     = std::stoi(argv[1]);
        sample_size = std::stoi(argv[2]);
    }

    full_stats_manual_threading(samples, sample_size);
    return EXIT_SUCCESS;
}