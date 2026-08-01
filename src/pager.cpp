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
        // Calculate restored entity count based on total binary records
        num_entities = file_length / PLAYER_RECORD_SIZE;
        file_stream.seekg(0, std::ios::beg);
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

    // Lazy loading: load page into RAM only on first access
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
    for (uint32_t i = 0; i < MAX_PAGES; ++i) {
        if (pages[i] != nullptr) {
            flush(i);
            delete pages[i];
            pages[i] = nullptr;
        }
    }

    if (file_stream.is_open()) {
        file_stream.close();
    }
}

uint32_t Pager::get_num_pages() const {
    if (num_entities == 0) return 0;
    return (num_entities + ENTITIES_PER_PAGE - 1) / ENTITIES_PER_PAGE;
}
