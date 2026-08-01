#ifndef PAGER_HPP
#define PAGER_HPP

#include <string>
#include <fstream>
#include <array>
#include <cstdint>
#include <cstddef>
#include <memory>

// Page and record sizing constants per Milestone 3 specification
constexpr size_t PAGE_SIZE = 4096;
constexpr size_t PLAYER_RECORD_SIZE = 291;
constexpr size_t ENTITIES_PER_PAGE = PAGE_SIZE / PLAYER_RECORD_SIZE; // 14 entities per page
constexpr size_t MAX_PAGES = 100;
constexpr size_t MAX_ENTITIES = ENTITIES_PER_PAGE * MAX_PAGES; // 1400 entities max capacity

// Represents a 4KB memory page matching hardware disk block size
struct Page {
    uint8_t data[PAGE_SIZE];
    bool is_dirty{false};
};

/**
 * @class Pager
 * @brief Manages lazy loading, memory caching, and disk persistence of 4KB binary pages.
 */
class Pager {
private:
    std::string filename;
    std::fstream file_stream;
    uint32_t file_length{0};
    uint32_t num_entities{0};
    std::array<Page*, MAX_PAGES> pages{};

public:
    explicit Pager(const std::string& fname);
    ~Pager();

    // Disable copy semantics to prevent accidental file handle duplication
    Pager(const Pager&) = delete;
    Pager& operator=(const Pager&) = delete;

    // Get pointer to a specific entity slot by linear index
    uint8_t* get_entity_slot(uint32_t index);

    // Retrieve or lazy-load a 4KB page from disk
    Page* get_page(uint32_t page_num);

    // Mark a page as modified needing disk write
    void mark_dirty(uint32_t page_num);

    // Flush a dirty page to disk
    void flush(uint32_t page_num);

    // Flush all dirty pages and close file stream
    void close();

    // Accessors
    uint32_t get_num_entities() const { return num_entities; }
    void set_num_entities(uint32_t count) { num_entities = count; }
    uint32_t get_num_pages() const;
    std::string get_filename() const { return filename; }
    bool is_open() const { return file_stream.is_open(); }
};

#endif // PAGER_HPP
