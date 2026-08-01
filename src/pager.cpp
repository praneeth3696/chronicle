#include "pager.hpp"
#include <iostream>
#include <algorithm>
#include <cstring>

Pager::Pager(const std::string& fname) : filename(fname) {
    pages.fill(nullptr);

    // Open existing binary file for reading and writing
    file_stream.open(filename, std::ios::in | std::ios::out | std::ios::binary);

    // If file does not exist, create it cleanly and reopen
    if (!file_stream.is_open()) {
        file_stream.clear();
        file_stream.open(filename, std::ios::out | std::ios::binary);
        file_stream.close();
        file_stream.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    }

    if (file_stream.is_open()) {
        file_stream.seekg(0, std::ios::end);
        file_length = static_cast<uint32_t>(file_stream.tellg());

        // Count restored valid entity records by checking Little-Endian uint32_t IDs at slot boundaries
        num_entities = 0;
        uint32_t max_possible_slots = file_length / PLAYER_RECORD_SIZE;
        for (uint32_t i = 0; i < max_possible_slots; ++i) {
            uint32_t page_num = i / ENTITIES_PER_PAGE;
            uint32_t slot_in_page = i % ENTITIES_PER_PAGE;
            uint32_t offset = (page_num * PAGE_SIZE) + (slot_in_page * PLAYER_RECORD_SIZE);

            if (offset + 4 > file_length) break;

            file_stream.seekg(offset, std::ios::beg);
            uint8_t id_bytes[4] = {0};
            file_stream.read(reinterpret_cast<char*>(id_bytes), 4);
            if (file_stream.gcount() < 4) break;

            uint32_t id32 = static_cast<uint32_t>(id_bytes[0]) |
                           (static_cast<uint32_t>(id_bytes[1]) << 8) |
                           (static_cast<uint32_t>(id_bytes[2]) << 16) |
                           (static_cast<uint32_t>(id_bytes[3]) << 24);

            if (id32 > 0) {
                num_entities = i + 1;
            } else {
                break;
            }
        }
        file_stream.clear();
    }
}

Pager::~Pager() {
    close();
}

uint8_t* Pager::get_entity_slot(uint32_t index) {
    if (index >= MAX_ENTITIES) return nullptr;

    uint32_t page_num = index / ENTITIES_PER_PAGE;
    uint32_t slot_offset = (index % ENTITIES_PER_PAGE) * PLAYER_RECORD_SIZE;

    Page* page = get_page(page_num);
    if (!page) return nullptr;

    return page->data + slot_offset;
}

Page* Pager::get_page(uint32_t page_num) {
    if (page_num >= MAX_PAGES) return nullptr;

    // Lazy loading: load 4KB page into RAM only on first access
    if (pages[page_num] == nullptr) {
        Page* new_page = new Page();
        std::memset(new_page->data, 0, PAGE_SIZE);

        if (file_stream.is_open()) {
            file_stream.clear();
            file_stream.seekg(page_num * PAGE_SIZE, std::ios::beg);
            file_stream.read(reinterpret_cast<char*>(new_page->data), PAGE_SIZE);
            file_stream.clear(); // Clear EOF or short-read flags
        }

        new_page->is_dirty = false;
        pages[page_num] = new_page;
    }

    return pages[page_num];
}

void Pager::mark_dirty(uint32_t page_num) {
    if (page_num < MAX_PAGES && pages[page_num] != nullptr) {
        pages[page_num]->is_dirty = true;
    }
}

void Pager::flush(uint32_t page_num) {
    if (page_num >= MAX_PAGES || pages[page_num] == nullptr) return;

    if (pages[page_num]->is_dirty && file_stream.is_open()) {
        file_stream.clear();
        file_stream.seekp(page_num * PAGE_SIZE, std::ios::beg);
        file_stream.write(reinterpret_cast<const char*>(pages[page_num]->data), PAGE_SIZE);
        file_stream.flush();
        pages[page_num]->is_dirty = false;
    }
}

void Pager::close() {
    if (file_stream.is_open()) {
        for (uint32_t i = 0; i < MAX_PAGES; ++i) {
            if (pages[i] != nullptr && pages[i]->is_dirty) {
                flush(i);
            }
        }
        file_stream.close();
    }

    for (uint32_t i = 0; i < MAX_PAGES; ++i) {
        if (pages[i] != nullptr) {
            delete pages[i];
            pages[i] = nullptr;
        }
    }
}

uint32_t Pager::get_num_pages() const {
    if (num_entities == 0) return 0;
    return (num_entities + ENTITIES_PER_PAGE - 1) / ENTITIES_PER_PAGE;
}
