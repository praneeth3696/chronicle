#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <vector>
#include <algorithm>

struct Player {
    long long id;
    std::string username;
    std::string email;
};

class WorldState {
private:
    std::vector<Player> players;

public:
    WorldState() = default;

    bool addPlayer(const Player& player) {
        for (const auto& p : players) {
            if (p.id == player.id) {
                return false; // Duplicate ID
            }
        }
        players.push_back(player);
        return true;
    }

    const Player* getPlayerById(long long id) const {
        for (const auto& p : players) {
            if (p.id == id) return &p;
        }
        return nullptr;
    }

    std::vector<Player> getPlayersSorted() const {
        std::vector<Player> sorted = players;
        std::sort(sorted.begin(), sorted.end(), [](const Player& a, const Player& b) {
            return a.id < b.id;
        });
        return sorted;
    }

    size_t getPlayerCount() const {
        return players.size();
    }

    const std::vector<Player>& getPlayers() const {
        return players;
    }
};

#endif // ENTITY_HPP