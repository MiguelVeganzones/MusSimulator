#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <random>
#include <string>

enum struct CardType : std::int_fast8_t
{
    Rey,
    Caballo,
    Sota,
    Siete,
    Seis,
    Cinco,
    Cuatro,
    As,
    NULL_CARD
};

class Card
{
public:
    using data_type = float;

    inline static constexpr int s_Nominal_size  = 8;
    inline static constexpr int s_Extended_size = 9;

    template <typename T>
    using container_type = std::array<T, s_Nominal_size>;
    template <typename T>
    using extended_container_type = std::array<T, s_Extended_size>;

    inline static constexpr container_type<data_type> s_big_value{ 8.f, 7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f };

    inline static constexpr container_type<data_type> s_small_value = s_big_value;

    inline static constexpr container_type<data_type> s_pair_value = s_big_value;

    inline static constexpr container_type<data_type> s_game_value = { 10, 10, 10, 7, 6, 5, 4, 1 };

    inline static constexpr extended_container_type<char> s_repr = { 'R', 'C', 'S', '7', '6', '5', '4', 'A', 'X' };

    inline bool operator<(Card const& other) const
    {
        return get_idx() < other.get_idx();
    }

    inline bool operator==(Card const& other) const
    {
        return type == other.type;
    }

    CardType type;

    constexpr Card(CardType t = CardType::NULL_CARD) :
        type{ t }
    {
    }

    constexpr Card(char c) :
        type{ get_type(c) }
    {
    }

    constexpr Card(const Card&) noexcept            = default;
    constexpr Card(Card&&) noexcept                 = default;
    constexpr Card& operator=(const Card&) noexcept = default;
    constexpr Card& operator=(Card&&) noexcept      = default;
    ~Card() noexcept                                = default;

private:
    inline auto get_idx() const noexcept -> std::underlying_type_t<CardType>
    {
        return static_cast<std::underlying_type_t<CardType>>(type);
    }

    inline static constexpr auto get_type(char c) noexcept -> CardType
    {
        switch (c)
        {
        case '8':
            return CardType::Rey;
        case '7':
            return CardType::Caballo;
        case '6':
            return CardType::Sota;
        case '5':
            return CardType::Siete;
        case '4':
            return CardType::Seis;
        case '3':
            return CardType::Cinco;
        case '2':
            return CardType::Cuatro;
        case '1':
            return CardType::As;
        default:
            return CardType::NULL_CARD;
        }
    }

public:
    inline auto get_big_value() const noexcept -> data_type
    {
        assert(get_idx() < (int)s_big_value.size());
        return s_big_value[get_idx()];
    }

    inline auto get_small_value() const noexcept -> data_type
    {
        assert(get_idx() < (int)s_small_value.size());
        return s_small_value[get_idx()];
    }

    inline auto get_pair_value() const noexcept -> data_type
    {
        assert(get_idx() < (int)s_pair_value.size());
        return s_pair_value[get_idx()];
    }

    inline auto get_game_value() const noexcept -> data_type
    {
        assert(get_idx() < (int)s_game_value.size());
        return s_game_value[get_idx()];
    }

    inline auto repr() const noexcept -> char
    {
        assert(get_idx() < (int)s_repr.size());
        return s_repr[get_idx()];
    }
};

struct Index
{
    std::int_fast8_t hand_idx;
    std::int_fast8_t card_idx;
};

class Hand
{
public:
    inline static constexpr int s_Distinct_posssible_hands = 1320;
    using score_type                                       = float;
    using cards_container                                  = std::array<Card, 4>;

public:
    Hand() noexcept = default;

    Hand(cards_container _cards) noexcept :
        cards{ _cards }
    {
        sort();
    }

    Hand(const Hand&) noexcept            = default;
    Hand(Hand&&) noexcept                 = default;
    Hand& operator=(const Hand&) noexcept = default;
    Hand& operator=(Hand&&) noexcept      = default;
    ~Hand() noexcept                      = default;

    Hand(int _repr) noexcept :
        cards{}
    {
        parse_repr_str(std::to_string(_repr));
    }

    Hand(float _repr) noexcept :
        cards{}
    {
        auto  str = std::to_string(static_cast<int>(_repr));
        float f;
        board_position = static_cast<std::int_fast8_t>(std::modf(_repr, &f) * 11);
        parse_repr_str(str);
    }

private:
    cards_container  cards;
    std::int_fast8_t board_position{};
    std::int_fast8_t cards_dealt{};
    score_type       m_base_big_score{};
    score_type       m_base_small_score{};
    score_type       m_base_pair_score{};
    score_type       m_base_game_score{};

    inline auto sort() noexcept -> void
    {
        std::sort(std::begin(cards), std::end(cards));
        calc_big_score();
        calc_small_score();
        calc_pair_score();
        calc_game_score();
    }

    inline void parse_repr_str(std::string_view repr)
    {
        for (auto i = 0u; i != 4; ++i)
        {
            if (i < repr.length())
                cards[i] = Card(repr[i]);
            else
                cards[i] = Card(CardType::NULL_CARD);
        }
        sort();
    }

public:
    struct Hasher
    {
        size_t operator()(const Hand& h) const
        {
            // Compute individual hash values for two data members and combine them using XOR and bit shifting
            return static_cast<size_t>(h.get_big_score() * 100.f);
        }
    };

    inline bool operator<(Hand const& other) const
    {
        return get_big_score() < other.get_big_score();
    }

    inline bool operator==(Hand const& other) const
    {
        return get_big_score() == other.get_big_score();
    }

    inline auto set_board_position(std::int_fast8_t pos) noexcept -> void
    {
        board_position = pos;
    }

    inline auto set(std::int_fast8_t idx, Card const& card) noexcept -> void
    {
        cards[idx] = card;
        if (++cards_dealt >= 4)
            sort();
    }

    inline auto get(int idx) const noexcept -> Card const&
    {
        return cards[idx];
    }

    inline auto get_big_score() const noexcept -> score_type
    {
        return m_base_big_score + get_table_position_score_delta();
    }

    inline auto calc_big_score() noexcept -> void
    {
        score_type score{};
        float      offset = 1000.f;
        for (auto c : cards)
        {
            score += c.get_big_value() * offset;
            offset /= 10.f;
        }
        // std::cout << repr() << "\n" << score + (float)board_position / 10 << "\n";
        m_base_big_score = score;
        // assert(m_base_big_score > 1000);
    }

    inline auto get_small_score() const noexcept -> score_type
    {
        return m_base_small_score - get_table_position_score_delta();
    }

    inline auto calc_small_score() noexcept -> void
    {
        score_type score{};
        float      offset = 1.f;
        for (auto c : cards)
        {
            score += c.get_big_value() * offset;
            offset *= 10.f;
        }
        m_base_small_score = score;
        // assert(m_base_small_score > 1000);
    }

    inline auto get_pair_score() const noexcept -> score_type
    {
        return m_base_pair_score + get_table_position_score_delta();
    }

    inline auto calc_pair_score() noexcept -> void
    {
        score_type score{};
        for (int i = 0; i < 3; ++i)
        {
            int count = 1;
            for (int j = i + 1; j != 4; ++j)
            {
                if (cards[j] == cards[i])
                {
                    ++count;
                }
                else
                {
                    break;
                }
            }
            if (count == 2)
            {
                if (score > 0)
                    score *= score_type(100);
                score += cards[i].get_pair_value() * score_type(11);
            }
            else if (count == 3)
            {
                score += cards[i].get_pair_value() * score_type(111);
            }
            else if (count == 4)
            {
                score += cards[i].get_pair_value() * score_type(1111);
            }
            i += (count - 1);
        }

        m_base_pair_score = score;
    }

    inline auto get_game_score() const noexcept -> score_type
    {
        return m_base_game_score + get_table_position_score_delta();
    }

    inline auto get_base_game_score() const noexcept -> score_type
    {
        score_type score{};
        for (auto c : cards)
        {
            score += c.get_game_value();
        }
        return score;
    }

    inline auto calc_game_score() noexcept -> void
    {
        auto score = get_base_game_score();
        if ((int)score == 31)
            score *= 100;
        else if ((int)score == 32)
            score *= 10;
        m_base_game_score = score;
        assert(m_base_game_score > 3);
    }

    inline auto get_table_position_score_delta() const noexcept -> score_type
    {
        return (score_type)board_position / score_type(10);
    }

    inline auto repr() const noexcept -> std::string
    {
        std::string ret(8, ' ');
        for (int i = 0; i != 4; ++i)
        {
            ret[i * 2] = cards[i].repr();
        }

        ret += get_board_position_repr();

        return ret;
    }

    inline auto get_board_position_repr() const -> std::string
    {
        int repr = -1;
        switch (board_position)
        {
        case 1:
            repr = 4;
            break;
        case 2:
            repr = 3;
            break;
        case 3:
            repr = 2;
            break;
        case 4:
            repr = 1;
            break;
        }
        return '(' + (std::to_string(repr)) + ')';
    }

    inline auto get_game_repr() const -> std::string
    {
        return std::to_string((int)get_base_game_score()) + ' ' + get_board_position_repr();
    }

    inline auto get_pair_repr() const -> std::string
    {
        std::string ret(8, ' ');
        auto        offset = 1000;
        for (int i = 0; i != 4; ++i)
        {
            if ((int)m_base_pair_score % offset > 0)
            {
                ret[i * 2] = cards[i].repr();
            }
            else
            {
                ret[i * 2] = Card(CardType::NULL_CARD).repr();
            }
            offset /= 10;
        }
        return ret + get_board_position_repr();
    }

    inline friend auto operator<<(std::ostream& os, Hand const& h) noexcept -> std::ostream&
    {
        os << h.repr();
        return os;
    }
};

class Deck
{
    using cards_container                                  = std::array<Card, 40>;
    inline static constexpr cards_container s_Default_deck = {
        Card(CardType::Rey),     Card(CardType::Rey),     Card(CardType::Rey),     Card(CardType::Rey),

        Card(CardType::Rey),     Card(CardType::Rey),     Card(CardType::Rey),     Card(CardType::Rey),

        Card(CardType::Caballo), Card(CardType::Caballo), Card(CardType::Caballo), Card(CardType::Caballo),

        Card(CardType::Sota),    Card(CardType::Sota),    Card(CardType::Sota),    Card(CardType::Sota),

        Card(CardType::Siete),   Card(CardType::Siete),   Card(CardType::Siete),   Card(CardType::Siete),

        Card(CardType::Seis),    Card(CardType::Seis),    Card(CardType::Seis),    Card(CardType::Seis),

        Card(CardType::Cinco),   Card(CardType::Cinco),   Card(CardType::Cinco),   Card(CardType::Cinco),

        Card(CardType::Cuatro),  Card(CardType::Cuatro),  Card(CardType::Cuatro),  Card(CardType::Cuatro),

        Card(CardType::As),      Card(CardType::As),      Card(CardType::As),      Card(CardType::As),

        Card(CardType::As),      Card(CardType::As),      Card(CardType::As),      Card(CardType::As),
    };

    std::mt19937 m_random_engine = std::mt19937(std::random_device()());

    cards_container  Cards;
    std::int_fast8_t dealt_cards = 0;

public:
    Deck() :
        Cards{ s_Default_deck }
    {
    }

    auto shuffle() noexcept -> void
    {
        if (dealt_cards < 32)
            return;
        std::shuffle(Cards.begin(), Cards.end(), m_random_engine);
        dealt_cards = 0;
    }

    auto deal_one() noexcept -> Card
    {
        return Cards[dealt_cards++];
    }

    auto deal_first() noexcept -> std::array<Hand, 4>
    {
        std::array<Hand, 4> ret;
        for (std::int_fast8_t i = 0; i != 4; ++i)
        {
            ret[i].set_board_position(i + 1);
            for (auto j = 0; j != 4; ++j)
            {
                ret[j].set(i, deal_one());
            }
        }
        return ret;
    }

    auto deal_mus(std::array<Hand, 4> current_hand, std::vector<Index> const& indeces) noexcept -> std::array<Hand, 4>
    {
        for (auto index : indeces)
        {
            current_hand[index.hand_idx].set(index.card_idx, deal_one());
        }
        return current_hand;
    }

    auto repr() const noexcept -> std::string
    {
        std::string ret((40 - dealt_cards) * 2, ' ');
        for (int i = dealt_cards, str_idx = 0; i != 40; ++i, str_idx += 2)
        {
            ret[str_idx] = Cards[i].repr();
        }
        return ret;
    }
};

[[nodiscard]] inline bool game_compare(float game1, float game2) noexcept
{
    return game1 < game2;
}

class GameInstance
{
public:
    inline static constexpr std::array<std::array<std::int_fast8_t, 4>, 24> s_board_positions_permutations =
        []() constexpr {
            std::array<std::int_fast8_t, 4>                 board_positions = { 1, 2, 3, 4 };
            std::array<std::array<std::int_fast8_t, 4>, 24> ret;
            for (int j = 0; j != 24; ++j)
            {
                for (int i = 0; i != 4; ++i)
                {
                    ret[j][i] = board_positions[i];
                }
                std::next_permutation(std::begin(board_positions), std::end(board_positions));
            }
            return ret;
        }();
};

// [[nodiscard]] inline bool game_compare(float game1, float game2) noexcept
// {
//     if (game1 >= 31 && game2 < 31)
//     {
//         return false;
//     }
//     if (game1 < 31 && game2 >= 31)
//     {
//         return true;
//     }
//     if (game1 < 31 && game2 < 31)
//     {
//         return game1 < game2;
//     }
//     if (game1 >= 31 && game2 >= 31)
//     {
//         if (game1 < 33 && game2 >= 33)
//         {
//             return false;
//         }
//         if (game1 >= 33 && game2 < 33)
//         {
//             return true;
//         }
//         if (game1 < 33 && game2 < 33)
//         {
//             if ((int)game1 == 32)
//             {
//                 if ((int)game2 == 31)
//                 {
//                     return true;
//                 }
//                 if ((int)game2 == 32)
//                 {
//                     return game1 < game2;
//                 }
//             }
//             if ((int)game1 == 31)
//             {
//                 if ((int)game2 == 31)
//                 {
//                     return game1 < game2;
//                 }
//                 if ((int)game2 == 32)
//                 {
//                     return false;
//                 }
//             }
//         }
//         if (game1 >= 33 && game2 >= 33)
//         {
//             return game1 < game2;
//         }
//     }
//     assert(false);
//     std::unreachable();
// }
