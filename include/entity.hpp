#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <memory>
#include "pager.hpp"

// Player model struct
struct Player {
    long long id;
    std::string username;
    std::string email;
};

/**
 * @brief Serializes a Player struct into a 291-byte binary buffer.
 * Byte layout:
 * - Bytes 0..3   : uint32_t Little-Endian ID
 * - Bytes 4..35  : username (32 bytes, null-padded)
 * - Bytes 36..290: email (255 bytes, null-padded)
 */
inline void serialize_player(const Player& p, uint8_t* dest) {
    std::memset(dest, 0, PLAYER_RECORD_SIZE);

    // Pack 32-bit integer ID in Little-Endian format
    uint32_t id32 = static_cast<uint32_t>(p.id);
    dest[0] = static_cast<uint8_t>(id32 & 0xFF);
    dest[1] = static_cast<uint8_t>((id32 >> 8) & 0xFF);
    dest[2] = static_cast<uint8_t>((id32 >> 16) & 0xFF);
    dest[3] = static_cast<uint8_t>((id32 >> 24) & 0xFF);

    // Copy string contents into null-padded fixed buffers
    std::strncpy(reinterpret_cast<char*>(dest + 4), p.username.c_str(), 32);
    std::strncpy(reinterpret_cast<char*>(dest + 36), p.email.c_str(), 255);
}

/**
 * @brief Deserializes a 291-byte binary buffer back into a Player struct.
 */
inline Player deserialize_player(const uint8_t* src) {
    Player p;

    // Unpack 32-bit Little-Endian ID
    uint32_t id32 = static_cast<uint32_t>(src[0]) |
                   (static_cast<uint32_t>(src[1]) << 8) |
                   (static_cast<uint32_t>(src[2]) << 16) |
                   (static_cast<uint32_t>(src[3]) << 24);
    p.id = id32;

    // Unpack username and email buffers
    char user_buf[33] = {0};
    std::memcpy(user_buf, src + 4, 32);
    p.username = std::string(user_buf);

    char email_buf[256] = {0};
    std::memcpy(email_buf, src + 36, 255);
    p.email = std::string(email_buf);

    return p;
}

/**
 * @class WorldState
 * @brief Manages active player entities backed by a binary Pager file or in-memory fallback.
 */
class WorldState {
private:
    std::unique_ptr<Pager> pager;
    std::vector<Player> in_memory_players; // Fallback for offline in-memory execution

public:
    WorldState() = default;

    explicit WorldState(const std::string& world_file) {
        if (!world_file.empty()) {
            pager = std::make_unique<Pager>(world_file);
        }
    }

    bool hasPager() const { return pager != nullptr && pager->is_open(); }
    Pager* getPager() { return pager.get(); }
    const Pager* getPager() const { return pager.get(); }

    bool addPlayer(const Player& player) {
        if (hasPager()) {
            uint32_t count = pager->get_num_entities();
            if (count >= MAX_ENTITIES) {
                return false; // Reached maximum engine entity limit (1400)
            }

            // Check duplicate ID across existing saved records
            for (uint32_t i = 0; i < count; ++i) {
                uint8_t* slot = pager->get_entity_slot(i);
                if (slot) {
                    Player existing = deserialize_player(slot);
                    if (existing.id == player.id) return false;
                }
            }

            // Serialize into target page slot
            uint8_t* slot = pager->get_entity_slot(count);
            if (!slot) return false;

            serialize_player(player, slot);
            uint32_t page_num = count / ENTITIES_PER_PAGE;
            pager->mark_dirty(page_num);
            pager->set_num_entities(count + 1);
            return true;
        } else {
            // Fallback for in-memory only operations
            for (const auto& p : in_memory_players) {
                if (p.id == player.id) return false;
            }
            in_memory_players.push_back(player);
            return true;
        }
    }

    const Player* getPlayerById(long long id) const {
        if (hasPager()) {
            uint32_t count = pager->get_num_entities();
            for (uint32_t i = 0; i < count; ++i) {
                uint8_t* slot = const_cast<Pager*>(pager.get())->get_entity_slot(i);
                if (slot) {
                    Player p = deserialize_player(slot);
                    if (p.id == id) {
                        static Player temp;
                        temp = p;
                        return &temp;
                    }
                }
            }
            return nullptr;
        } else {
            for (const auto& p : in_memory_players) {
                if (p.id == id) return &p;
            }
            return nullptr;
        }
    }

    size_t getPlayerCount() const {
        return hasPager() ? pager->get_num_entities() : in_memory_players.size();
    }

    std::vector<Player> getPlayersSorted() const {
        std::vector<Player> result;
        if (hasPager()) {
            uint32_t count = pager->get_num_entities();
            for (uint32_t i = 0; i < count; ++i) {
                uint8_t* slot = const_cast<Pager*>(pager.get())->get_entity_slot(i);
                if (slot) {
                    result.push_back(deserialize_player(slot));
                }
            }
        } else {
            result = in_memory_players;
        }

        std::sort(result.begin(), result.end(), [](const Player& a, const Player& b) {
            return a.id < b.id;
        });
        return result;
    }

    std::vector<Player> getPlayers() const {
        return getPlayersSorted();
    }

    void close() {
        if (pager) {
            pager->close();
        }
    }
};

#endif // ENTITY_HPP