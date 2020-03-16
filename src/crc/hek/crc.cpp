// SPDX-License-Identifier: GPL-3.0-only

#include <vector>
#include "../crc32.h"
#include "../crc_spoof.h"
#include <invader/tag/hek/definition.hpp>
#include <invader/crc/hek/crc.hpp>
#include <invader/map/map.hpp>

namespace Invader {
    std::uint32_t calculate_map_crc(const std::byte *data, std::size_t size, const std::uint32_t *new_crc, std::uint32_t *new_random, bool *check_dirty, bool allow_compressed) {
        std::vector<std::byte> data_crc;
        std::uint32_t crc = 0;

        if(new_crc && !new_random) {
            std::terminate();
        }

        if(new_crc) {
            data_crc.reserve(size);
        }

        // Parse the map
        Map map = allow_compressed ? Map::map_with_copy(data, size) : Map::map_with_pointer(const_cast<std::byte *>(data), size);
        auto engine = map.get_engine();
        if(engine == HEK::CacheFileEngine::CACHE_FILE_ANNIVERSARY || engine == HEK::CacheFileEngine::CACHE_FILE_XBOX) {
            return 0;
        }

        // Reassign variables if needed
        data = map.get_data();
        size = map.get_data_length();

        #define CRC_DATA(data_start, data_end) \
            if(new_crc) { \
                data_crc.insert(data_crc.end(), data + data_start, data + data_end); \
            } \
            else { \
                crc = crc32(crc, data + data_start, data_end - data_start); \
            }

        auto &scenario_tag = map.get_tag(map.get_scenario_tag_id());
        auto &scenario = scenario_tag.get_base_struct<HEK::Scenario>();
        std::size_t bsp_count = scenario.structure_bsps.count.read();
        auto *bsps = scenario_tag.resolve_reflexive(scenario.structure_bsps);

        // Iterate through all BSPs
        for(std::size_t b = 0; b < bsp_count; b++) {
            std::size_t start = bsps[b].bsp_start.read();
            std::size_t end = start + bsps[b].bsp_size.read();

            if(start >= size || end > size) {
                throw OutOfBoundsException();
            }

            // Add it
            CRC_DATA(start, end);
        }

        // Now copy model data
        std::size_t model_start = map.get_model_data_offset();
        std::size_t model_end = model_start + map.get_model_data_size();
        if(model_start >= size || model_end > size) {
            throw OutOfBoundsException();
        }
        CRC_DATA(model_start, model_end);

        // Lastly, do tag data
        auto *tag_data = map.get_tag_data_at_offset(0);
        std::size_t tag_data_start = map.get_tag_data_at_offset(0) - map.get_data_at_offset(0);
        std::size_t tag_data_end = tag_data_start + map.get_tag_data_length();
        if(tag_data_start >= size || tag_data_end > size) {
            throw OutOfBoundsException();
        }

        // Find out where we're going to be doing CRC32 stuff
        auto *random_number = &reinterpret_cast<HEK::CacheFileTagDataHeader *>(map.get_tag_data_at_offset(0, sizeof(HEK::CacheFileTagDataHeader)))->random_number;
        const std::byte *random_number_ptr = reinterpret_cast<const std::byte *>(random_number);
        std::size_t random_number_offset_in_memory = random_number_ptr - tag_data + data_crc.size();
        CRC_DATA(tag_data_start, tag_data_end);

        // Overwrite with new CRC32
        if(new_crc) {
            FakeFileHandle handle = { reinterpret_cast<std::uint8_t *>(data_crc.data()), data_crc.size(), 0 };
            std::uint32_t newcrc = ~crc_spoof_reverse_bits(*new_crc);
            crc_spoof_modify_file_crc32(&handle, random_number_offset_in_memory, newcrc, false);
            *new_random = *reinterpret_cast<std::uint32_t *>(data_crc.data() + random_number_offset_in_memory);

            // We have no way of knowing if the map was dirty or not because we just forged the CRC
            if(check_dirty) {
                *check_dirty = false;
            }

            return ~crc32(0, data_crc.data(), data_crc.size());
        }
        else {
            std::uint32_t crc_value = ~crc;
            if(check_dirty) {
                *check_dirty = crc_value != map.get_header_crc32();
            }
            return crc_value;
        }
    }
}
