// SPDX-License-Identifier: GPL-3.0-only

#include <zlib.h>
#include <filesystem>
#include <optional>

#include <invader/printf.hpp>
#include <invader/version.hpp>
#include <invader/tag/hek/definition.hpp>
#include <invader/tag/hek/header.hpp>
#include "image_loader.hpp"
#include <invader/bitmap/color_plate_scanner.hpp>
#include <invader/bitmap/bitmap_processor.hpp>
#include "bitmap_data_writer.hpp"
#include <invader/command_line_option.hpp>
#include <invader/file/file.hpp>
#include <invader/tag/parser/parser.hpp>

enum SupportedFormatsInt {
    SUPPORTED_FORMATS_TIF = 0,
    SUPPORTED_FORMATS_TIFF,
    SUPPORTED_FORMATS_PNG,
    SUPPORTED_FORMATS_TGA,
    SUPPORTED_FORMATS_BMP,

    SUPPORTED_FORMATS_INT_COUNT
};

static const char *SUPPORTED_FORMATS[] = {
    ".tif",
    ".tiff",
    ".png",
    ".tga",
    ".bmp"
};

static_assert(sizeof(SUPPORTED_FORMATS) / sizeof(*SUPPORTED_FORMATS) == SUPPORTED_FORMATS_INT_COUNT);

using namespace Invader;
using namespace Invader::HEK;

struct BitmapOptions {
    // Data directory
    std::filesystem::path data = "data";
    
    // Tags directory
    std::filesystem::path tags = "tags";

    // Scale type?
    std::optional<BitmapMipmapScaleType> mipmap_scale_type;

    // Format?
    std::optional<BitmapFormat> format;
    
    // Find format automatically
    std::optional<bool> auto_format;

    // Usage?
    std::optional<BitmapUsage> usage;

    // Bump stuff
    std::optional<float> bump_height;

    // Palettize to p8 bump?
    std::optional<bool> palettize;

    // Mipmap fade factor
    std::optional<float> mipmap_fade;

    // Bitmap type
    std::optional<BitmapType> bitmap_type;

    // Sprite parameters
    std::optional<BitmapSpriteUsage> sprite_usage;
    std::optional<std::uint32_t> sprite_budget;
    std::optional<std::uint32_t> sprite_budget_count;
    std::optional<std::uint16_t> sprite_spacing;
    bool force_square_sprite_sheets = false;

    // Dithering?
    std::optional<bool> dither_alpha, dither_color, dithering;

    // Sharpen and blur; legacy support for older tags and should not be used in newer ones
    std::optional<float> sharpen;
    std::optional<float> blur;
    std::optional<float> alpha_bias;

    // Generate this many mipmaps
    std::optional<std::uint16_t> max_mipmap_count;
    
    // Filthy sprite bug fix?
    std::optional<bool> filthy_sprite_bug_fix;

    // Ignore the tag data?
    bool ignore_tag_data = false;

    // Use a filesystem path?
    bool filesystem_path = false;
    
    // Regenerate?
    bool regenerate = false;
};

template <typename T> static int perform_the_ritual(const std::string &bitmap_tag, const std::filesystem::path &tag_path, const std::filesystem::path &final_path, BitmapOptions &bitmap_options, SupportedFormatsInt found_format, TagFourCC tag_fourcc) {
    // Let's begin
    std::filesystem::path data_path = bitmap_options.data;

    // Start building the bitmap tag
    T bitmap_tag_data = {};

    // See if we can get anything out of this
    if(!bitmap_options.ignore_tag_data && std::filesystem::exists(final_path)) {
        auto tag_data = Invader::File::open_file(final_path).value();
        bitmap_tag_data = T::parse_hek_tag_file(tag_data.data(), tag_data.size());

        // Set some default values
        if(!bitmap_options.format.has_value() && !bitmap_options.auto_format.value_or(false)) {
            bitmap_options.format = bitmap_tag_data.encoding_format;
            bitmap_options.auto_format = false;
        }
        if(!bitmap_options.mipmap_fade.has_value()) {
            bitmap_options.mipmap_fade = bitmap_tag_data.detail_fade_factor;
        }
        if(!bitmap_options.bitmap_type.has_value()) {
            bitmap_options.bitmap_type = bitmap_tag_data.type;
        }
        if(!bitmap_options.max_mipmap_count.has_value()) {
            std::int16_t mipmap_count = bitmap_tag_data.mipmap_count;
            if(mipmap_count == 0) {
                bitmap_options.max_mipmap_count = INT16_MAX;
            }
            else {
                bitmap_options.max_mipmap_count = mipmap_count - 1;
            }
        }
        if(!bitmap_options.sprite_usage.has_value()) {
            bitmap_options.sprite_usage = bitmap_tag_data.sprite_usage;
        }
        if(!bitmap_options.sprite_budget.has_value()) {
            bitmap_options.sprite_budget = 32 << bitmap_tag_data.sprite_budget_size;
        }
        if(!bitmap_options.sprite_budget_count.has_value()) {
            bitmap_options.sprite_budget_count = bitmap_tag_data.sprite_budget_count;
        }
        if(!bitmap_options.usage.has_value()) {
            bitmap_options.usage = bitmap_tag_data.usage;
        }
        if(!bitmap_options.palettize.has_value()) {
            bitmap_options.palettize = !(bitmap_tag_data.flags & HEK::BitmapFlagsFlag::BITMAP_FLAGS_FLAG_DISABLE_HEIGHT_MAP_COMPRESSION);
        }
        if(!bitmap_options.bump_height.has_value()) {
            bitmap_options.bump_height = bitmap_tag_data.bump_height;
        }
        if(!bitmap_options.sharpen.has_value() && bitmap_tag_data.sharpen_amount > 0.0F && bitmap_tag_data.sharpen_amount <= 1.0F) {
            bitmap_options.sharpen = bitmap_tag_data.sharpen_amount;
        }
        if(!bitmap_options.blur.has_value() && bitmap_tag_data.blur_filter_size > 0.0F) {
            bitmap_options.blur = bitmap_tag_data.blur_filter_size;
        }
        if(!bitmap_options.sprite_spacing.has_value()) {
            bitmap_options.sprite_spacing = bitmap_tag_data.sprite_spacing;
        }
        if(!bitmap_options.filthy_sprite_bug_fix.has_value()) {
            bitmap_options.filthy_sprite_bug_fix = (bitmap_tag_data.flags & HEK::BitmapFlagsFlag::BITMAP_FLAGS_FLAG_FILTHY_SPRITE_BUG_FIX) != 0;
        }
        if(!bitmap_options.alpha_bias.has_value()) {
            bitmap_options.alpha_bias = bitmap_tag_data.alpha_bias;
        }
        
        // Clear existing data
        bitmap_tag_data.bitmap_data.clear();
        bitmap_tag_data.bitmap_group_sequence.clear();
        bitmap_tag_data.processed_pixel_data.clear();
    }
    else if(bitmap_options.regenerate) {
        eprintf_error("Cannot regenerate. No bitmap tag exists at %s", final_path.string().c_str());
        std::exit(EXIT_FAILURE);
    }

    // If these values weren't set, set them
    #define DEFAULT_VALUE(what, default) if(!what.has_value()) { what = default; }
    
    if(!bitmap_options.auto_format.has_value()) {
        bitmap_options.auto_format = true;
    }

    DEFAULT_VALUE(bitmap_options.bitmap_type,BitmapType::BITMAP_TYPE_2D_TEXTURES);
    DEFAULT_VALUE(bitmap_options.max_mipmap_count,INT16_MAX);
    DEFAULT_VALUE(bitmap_options.sprite_usage,BitmapSpriteUsage::BITMAP_SPRITE_USAGE_BLEND_ADD_SUBTRACT_MAX);
    DEFAULT_VALUE(bitmap_options.sprite_budget,32);
    DEFAULT_VALUE(bitmap_options.sprite_budget_count,0);
    DEFAULT_VALUE(bitmap_options.mipmap_scale_type,BitmapMipmapScaleType::BITMAP_MIPMAP_SCALE_TYPE_LINEAR);
    DEFAULT_VALUE(bitmap_options.mipmap_fade,0.0F);
    DEFAULT_VALUE(bitmap_options.usage,BitmapUsage::BITMAP_USAGE_DEFAULT);
    DEFAULT_VALUE(bitmap_options.palettize,false);
    DEFAULT_VALUE(bitmap_options.bump_height,0.026F);
    DEFAULT_VALUE(bitmap_options.mipmap_fade,0.0F);
    DEFAULT_VALUE(bitmap_options.alpha_bias,0.0F);
    DEFAULT_VALUE(bitmap_options.dithering,false);
    DEFAULT_VALUE(bitmap_options.dither_alpha,false);
    DEFAULT_VALUE(bitmap_options.dither_color,false);
    DEFAULT_VALUE(bitmap_options.filthy_sprite_bug_fix,false);
    DEFAULT_VALUE(bitmap_options.sprite_spacing,0);

    #undef DEFAULT_VALUE

    // Have these variables handy
    std::uint32_t image_width = 0, image_height = 0;
    std::size_t image_size = 0;
    std::vector<Pixel> image_pixels;

    // If we're regenerating, our color plate data is in the tag
    if(bitmap_options.regenerate) {
        // Check to see if we have data
        auto size = bitmap_tag_data.compressed_color_plate_data.size();
        image_width = bitmap_tag_data.color_plate_width;
        image_height = bitmap_tag_data.color_plate_height;
        if(size < sizeof(std::uint32_t) || image_width == 0 || image_height == 0) {
            eprintf_error("Cannot regenerate a bitmap that doesn't have color plate data.");
            return EXIT_FAILURE;
        }
        
        // Get the size of the data we're going to decompress
        auto *data = bitmap_tag_data.compressed_color_plate_data.data();
        image_size = reinterpret_cast<HEK::BigEndian<std::uint32_t> *>(data)->read();
        if((image_size % sizeof(Pixel)) != 0) {
            eprintf_error("Cannot regenerate due the compressed color plate data size being wrong");
            return EXIT_FAILURE;
        }
        image_pixels = std::vector<Pixel>(image_size / sizeof(Pixel));
        
        data += sizeof(std::uint32_t);
        size -= sizeof(std::uint32_t);
        
        z_stream inflate_stream;
        inflate_stream.zalloc = Z_NULL;
        inflate_stream.zfree = Z_NULL;
        inflate_stream.opaque = Z_NULL;
        inflate_stream.avail_out = image_size;
        inflate_stream.next_out = reinterpret_cast<Bytef *>(image_pixels.data());
        inflate_stream.avail_in = size;
        inflate_stream.next_in = reinterpret_cast<Bytef *>(data);

        // Do it
        inflateInit(&inflate_stream);
        inflate(&inflate_stream, Z_FINISH);
        inflateEnd(&inflate_stream);
    }
    
    // Otherwise, find the file
    else {
        // Try to figure out the extension
        auto bitmap_data_path = (data_path / bitmap_tag).string();
        for(auto i = found_format; i < SUPPORTED_FORMATS_INT_COUNT; i = static_cast<SupportedFormatsInt>(i + 1)) {
            std::string image_path = bitmap_data_path + SUPPORTED_FORMATS[i];
            if(std::filesystem::exists(image_path)) {
                switch(i) {
                    case SUPPORTED_FORMATS_TIF:
                    case SUPPORTED_FORMATS_TIFF:
                        image_pixels = load_tiff(image_path.c_str(), image_width, image_height, image_size);
                        break;
                    case SUPPORTED_FORMATS_PNG:
                    case SUPPORTED_FORMATS_TGA:
                    case SUPPORTED_FORMATS_BMP:
                        image_pixels = load_image(image_path.c_str(), image_width, image_height, image_size);
                        break;
                    default:
                        std::terminate();
                        break;
                }
                break;
            }
        }

        if(image_pixels.empty()) {
            eprintf_error("Failed to find %s in %s", bitmap_tag.c_str(), bitmap_options.data.string().c_str());
            eprintf("Valid formats are:\n");
            for(auto *format : SUPPORTED_FORMATS) {
                eprintf("    %s\n", format);
            }
            return EXIT_FAILURE;
        }
    }

    // Set up sprite parameters
    std::optional<BitmapProcessorSpriteParameters> sprite_parameters;
    if(bitmap_options.bitmap_type.value() == BitmapType::BITMAP_TYPE_SPRITES) {
        sprite_parameters.emplace();
        auto &p = sprite_parameters.value();
        p.sprite_budget = bitmap_options.sprite_budget.value();
        p.sprite_budget_count = bitmap_options.sprite_budget_count.value();
        p.sprite_usage = bitmap_options.sprite_usage.value();
        p.sprite_spacing = bitmap_options.sprite_spacing.value();
        p.force_square_sprite_sheets = bitmap_options.force_square_sprite_sheets;
    }

    // Do it!
    auto try_to_scan_color_plate = [&image_pixels, &image_width, &image_height, &bitmap_options, &sprite_parameters]() {
        try {
            auto scanned_data = ColorPlateScanner::scan_color_plate(image_pixels.data(), image_width, image_height, bitmap_options.bitmap_type.value(), bitmap_options.usage.value(), *bitmap_options.filthy_sprite_bug_fix);
            BitmapProcessor::process_bitmap_data(scanned_data, bitmap_options.bitmap_type.value(), bitmap_options.usage.value(), bitmap_options.bump_height.value(), sprite_parameters, bitmap_options.max_mipmap_count.value(), bitmap_options.mipmap_scale_type.value(), bitmap_options.usage == BitmapUsage::BITMAP_USAGE_DETAIL_MAP ? bitmap_options.mipmap_fade : std::nullopt, bitmap_options.sharpen, bitmap_options.blur, bitmap_options.alpha_bias);
            return scanned_data;
        }
        catch (std::exception &e) {
            eprintf_error("Failed to process the image: %s", e.what());
            std::exit(1);
        };
    };

    auto scanned_color_plate = try_to_scan_color_plate();

    // Compress the original input blob
    if(!bitmap_options.regenerate) {
        // Get ready
        bitmap_tag_data.compressed_color_plate_data.clear();
        std::vector<std::byte> compressed_data(image_size * 4);
        BigEndian<std::uint32_t> decompressed_size;
        decompressed_size = static_cast<std::uint32_t>(image_size);
        bitmap_tag_data.color_plate_width = image_width;
        bitmap_tag_data.color_plate_height = image_height;

        // Set compressed size
        bitmap_tag_data.compressed_color_plate_data.resize(sizeof(decompressed_size));
        *reinterpret_cast<BigEndian<std::uint32_t> *>(bitmap_tag_data.compressed_color_plate_data.data()) = decompressed_size;

        // Deflate color plate data
        compressed_data.resize(image_size * 4);
        z_stream deflate_stream;
        deflate_stream.zalloc = Z_NULL;
        deflate_stream.zfree = Z_NULL;
        deflate_stream.opaque = Z_NULL;
        deflate_stream.avail_in = image_size;
        deflate_stream.next_in = const_cast<Bytef *>(reinterpret_cast<const Bytef *>(image_pixels.data()));
        deflate_stream.avail_out = compressed_data.size();
        deflate_stream.next_out = reinterpret_cast<Bytef *>(compressed_data.data());

        // Do it
        deflateInit(&deflate_stream, Z_BEST_COMPRESSION);
        deflate(&deflate_stream, Z_FINISH);
        deflateEnd(&deflate_stream);
        bitmap_tag_data.compressed_color_plate_data.insert(bitmap_tag_data.compressed_color_plate_data.end(), compressed_data.data(), compressed_data.data() + deflate_stream.total_out);
    }

    // Now let's add the actual bitmap data
    #define BYTES_TO_MIB(bytes) (bytes / 1024.0F / 1024.0F)

    // Add our bitmap data
    try {
        // If we don't have a format, set it to null (it will determine it instead)
        if(*bitmap_options.auto_format) {
            bitmap_options.format = std::nullopt;
        }
        
        write_bitmap_data(scanned_color_plate, bitmap_tag_data.processed_pixel_data, bitmap_tag_data.bitmap_data, bitmap_options.usage.value(), bitmap_options.format, bitmap_options.bitmap_type.value(), bitmap_options.palettize.value(), bitmap_options.dither_alpha.value(), bitmap_options.dither_color.value(), bitmap_options.dither_color.value(), bitmap_options.dither_color.value());
    }
    catch (std::exception &e) {
        eprintf_error("Failed to generate bitmap data: %s", e.what());
        std::exit(1);
    }
    oprintf("Total: %.03f MiB\n", BYTES_TO_MIB(bitmap_tag_data.processed_pixel_data.size()));

    // Add all sequences
    for(auto &sequence : scanned_color_plate.sequences) {
        auto &bgs = bitmap_tag_data.bitmap_group_sequence.emplace_back();
        
        if(bitmap_options.bitmap_type.value() == BitmapType::BITMAP_TYPE_SPRITES) {
            bgs.bitmap_count = sequence.sprites.size() == 1 ? 1 : 0;
            bgs.first_bitmap_index = NULL_INDEX;
        }
        else {
            bgs.bitmap_count = sequence.bitmap_count;
            bgs.first_bitmap_index = sequence.first_bitmap;
        }

        // Add the sprites in the sequence
        for(auto &sprite : sequence.sprites) {
            auto &bgss = bgs.sprites.emplace_back();
            auto &bitmap = scanned_color_plate.bitmaps[sprite.bitmap_index];
            bgss.bitmap_index = sprite.bitmap_index;

            bgss.bottom = static_cast<float>(sprite.bottom) / bitmap.height;
            bgss.top = static_cast<float>(sprite.top) / bitmap.height;
            bgss.registration_point.y = static_cast<float>(sprite.registration_point_y) / bitmap.height;

            bgss.left = static_cast<float>(sprite.left) / bitmap.width;
            bgss.right = static_cast<float>(sprite.right) / bitmap.width;
            bgss.registration_point.x = static_cast<float>(sprite.registration_point_x) / bitmap.width;
            
            // Set the first bitmap index here
            if(bgss.bitmap_index < bgs.first_bitmap_index) {
                bgs.first_bitmap_index = bgss.bitmap_index;
            }
        }
        
        // If we never set it, set it to 0
        if(bgs.first_bitmap_index == NULL_INDEX) {
            bgs.first_bitmap_index = 0;
        }
    }

    // Set more parameters
    bitmap_tag_data.type = bitmap_options.bitmap_type.value();
    bitmap_tag_data.usage = bitmap_options.usage.value();
    bitmap_tag_data.bump_height = bitmap_options.bump_height.value();
    bitmap_tag_data.detail_fade_factor = bitmap_options.mipmap_fade.value();
    bitmap_tag_data.encoding_format = bitmap_options.format.value();
    bitmap_tag_data.sharpen_amount = bitmap_options.sharpen.value_or(0.0F);
    bitmap_tag_data.blur_filter_size = bitmap_options.blur.value_or(0.0F);
    bitmap_tag_data.alpha_bias = bitmap_options.alpha_bias.value_or(0.0F);
    bitmap_tag_data.flags = (bitmap_tag_data.flags & ~HEK::BitmapFlagsFlag::BITMAP_FLAGS_FLAG_DISABLE_HEIGHT_MAP_COMPRESSION & ~HEK::BitmapFlagsFlag::BITMAP_FLAGS_FLAG_FILTHY_SPRITE_BUG_FIX) | 
                            (*bitmap_options.palettize ? 0 : HEK::BitmapFlagsFlag::BITMAP_FLAGS_FLAG_DISABLE_HEIGHT_MAP_COMPRESSION) | 
                            (*bitmap_options.filthy_sprite_bug_fix ? HEK::BitmapFlagsFlag::BITMAP_FLAGS_FLAG_FILTHY_SPRITE_BUG_FIX : 0);
    if(bitmap_options.max_mipmap_count.value() >= INT16_MAX) {
        bitmap_tag_data.mipmap_count = 0;
    }
    else {
        bitmap_tag_data.mipmap_count = bitmap_options.max_mipmap_count.value() + 1;
    }

    // Set sprite stuff
    bitmap_tag_data.sprite_spacing = bitmap_options.sprite_spacing.value();
    bitmap_tag_data.sprite_budget_count = bitmap_options.sprite_budget_count.value();
    bitmap_tag_data.sprite_usage = bitmap_options.sprite_usage.value();
    auto &sprite_budget_value = bitmap_options.sprite_budget.value();
    switch(sprite_budget_value) {
        case 32:
            bitmap_tag_data.sprite_budget_size = BitmapSpriteBudgetSize::BITMAP_SPRITE_BUDGET_SIZE_32X32;
            break;
        case 64:
            bitmap_tag_data.sprite_budget_size = BitmapSpriteBudgetSize::BITMAP_SPRITE_BUDGET_SIZE_64X64;
            break;
        case 128:
            bitmap_tag_data.sprite_budget_size = BitmapSpriteBudgetSize::BITMAP_SPRITE_BUDGET_SIZE_128X128;
            break;
        case 256:
            bitmap_tag_data.sprite_budget_size = BitmapSpriteBudgetSize::BITMAP_SPRITE_BUDGET_SIZE_256X256;
            break;
        case 512:
            bitmap_tag_data.sprite_budget_size = BitmapSpriteBudgetSize::BITMAP_SPRITE_BUDGET_SIZE_512X512;
            break;
        case 1024:
            bitmap_tag_data.sprite_budget_size = BitmapSpriteBudgetSize::BITMAP_SPRITE_BUDGET_SIZE_1024X1024;
            break;
        default:
            bitmap_tag_data.sprite_budget_size = BitmapSpriteBudgetSize::BITMAP_SPRITE_BUDGET_SIZE_32X32;
            break;
    }

    // Write it all
    std::error_code ec;
    std::filesystem::create_directories(tag_path.parent_path(), ec);
    
    if(!File::save_file(final_path.c_str(), bitmap_tag_data.generate_hek_tag_data(tag_fourcc, true))) {
        eprintf_error("Error: Failed to write to %s.", final_path.string().c_str());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    BitmapOptions bitmap_options;

    std::vector<CommandLineOption> options;
    options.emplace_back("info", 'i', 0, "Show license and credits.");
    options.emplace_back("ignore-tag", 'I', 0, "Ignore the tag data if the tag exists.");
    options.emplace_back("dithering", 'D', 1, "Apply dithering to 16-bit or p8 bitmaps. This does not save in .bitmap tags. Can be: a, rgb, or argb. Default: none", "<channels>");
    options.emplace_back("data", 'd', 1, "Use the specified data directory.", "<dir>");
    options.emplace_back("tags", 't', 1, "Use the specified tags directory.", "<dir>");
    options.emplace_back("format", 'F', 1, "Pixel format. Can be: 32-bit, 16-bit, monochrome, dxt5, dxt3, dxt1, or auto. 'auto' will be replaced with the best lossless format. Default (new tag): auto", "<type>");
    options.emplace_back("type", 'T', 1, "Set the type of bitmap. Can be: 2d_textures, 3d_textures, cube_maps, interface_bitmaps, or sprites. Default (new tag): 2d", "<type>");
    options.emplace_back("mipmap-count", 'M', 1, "Set maximum mipmaps. Default (new tag): 32767", "<count>");
    options.emplace_back("mipmap-scale", 's', 1, "Mipmap scale type. This does not save in .bitmap tags. Can be: linear, nearest_alpha, nearest. Default (new tag): linear", "<type>");
    options.emplace_back("detail-fade", 'f', 1, "Set detail fade factor. Default (new tag): 0.0", "<factor>");
    options.emplace_back("budget", 'B', 1, "Set the maximum length of a sprite sheet. Can be 32, 64, 128, 256, 512, or 1024. Default (new tag): 32", "<length>");
    options.emplace_back("budget-count", 'C', 1, "Multiply the maximum length squared to set the maximum number of pixels. Setting this to 0 disables budgeting. Default (new tag): 0", "<count>");
    options.emplace_back("square-sheets", 'S', 0, "Force square sprite sheets (works around particles being incorrectly stretched).");
    options.emplace_back("bump-palettize", 'p', 1, "Set the bumpmap palettization setting. Can be: off or on. Default (new tag): off", "<val>");
    options.emplace_back("bump-height", 'H', 1, "Set the apparent bumpmap height from 0.0 to 1.0. Default (new tag): 0.026", "<height>");
    options.emplace_back("alpha-bias", 'A', 1, "Set the alpha bias from -1.0 to 1.0. Default (new tag): 0.0", "<bias>");
    options.emplace_back("usage", 'u', 1, "Set the bitmap usage. Can be: alpha_blend, default, height_map, detail_map, light_map, vector_map. Default: default", "<usage>");
    options.emplace_back("reg-point-hack", 'r', 1, "Ignore sequence borders when calculating registration point (AKA 'filthy sprite bug fix'). Can be: off or on. Default (new tag): off", "<val>");
    options.emplace_back("fs-path", 'P', 0, "Use a filesystem path for the data.");
    options.emplace_back("regenerate", 'R', 0, "Use the bitmap tag's compressed color plate data as data.");

    static constexpr char DESCRIPTION[] = "Create or modify a bitmap tag.";
    static constexpr char USAGE[] = "[options] <bitmap-tag>";

    // Go through each argument
    auto remaining_arguments = CommandLineOption::parse_arguments<BitmapOptions &>(argc, argv, options, USAGE, DESCRIPTION, 1, 1, bitmap_options, [](char opt, const std::vector<const char *> &arguments, auto &bitmap_options) {
        switch(opt) {
            case 'd':
                bitmap_options.data = arguments[0];
                break;
                
            case 'A':
                bitmap_options.alpha_bias = std::strtof(arguments[0], nullptr);
                if(bitmap_options.alpha_bias < -1.0F || bitmap_options.alpha_bias > 1.0F) {
                    eprintf_error("Alpha bias must be between -1.0 and 1.0");
                    std::exit(EXIT_FAILURE);
                }
                break;

            case 't':
                bitmap_options.tags = arguments[0];
                break;

            case 'R':
                bitmap_options.regenerate = true;
                break;

            case 'i':
                show_version_info();
                std::exit(EXIT_SUCCESS);

            case 'I':
                bitmap_options.ignore_tag_data = true;
                break;

            case 'f':
                bitmap_options.mipmap_fade = std::strtof(arguments[0], nullptr);
                if(bitmap_options.mipmap_fade < 0.0F || bitmap_options.mipmap_fade > 1.0F) {
                    eprintf_error("Mipmap fade must be between 0.0 and 1.0");
                    std::exit(EXIT_FAILURE);
                }
                break;

            case 's':
                if(std::strcmp(arguments[0], "linear") == 0) {
                    bitmap_options.mipmap_scale_type = BitmapMipmapScaleType::BITMAP_MIPMAP_SCALE_TYPE_LINEAR;
                }
                else if(std::strcmp(arguments[0], "nearest_alpha") == 0) {
                    bitmap_options.mipmap_scale_type = BitmapMipmapScaleType::BITMAP_MIPMAP_SCALE_TYPE_NEAREST_ALPHA;
                }
                else if(std::strcmp(arguments[0], "nearest") == 0) {
                    bitmap_options.mipmap_scale_type = BitmapMipmapScaleType::BITMAP_MIPMAP_SCALE_TYPE_NEAREST;
                }
                else {
                    eprintf_error("Invalid mipmap scale type %s", arguments[0]);
                    std::exit(EXIT_FAILURE);
                }
                break;

            case 'F':
                try {
                    if(std::strcmp(arguments[0], "auto") == 0) {
                        bitmap_options.format = std::nullopt;
                        bitmap_options.auto_format = true;
                    }
                    else {
                        bitmap_options.format = BitmapFormat_from_string(arguments[0]);
                        bitmap_options.auto_format = false;
                    }
                }
                catch(std::exception &) {
                    eprintf_error("Invalid bitmap format %s", arguments[0]);
                    std::exit(EXIT_FAILURE);
                }
                break;

            case 'T':
                try {
                    bitmap_options.bitmap_type = BitmapType_from_string(arguments[0]);
                }
                catch(std::exception &) {
                    eprintf_error("Invalid bitmap type %s", arguments[0]);
                    std::exit(EXIT_FAILURE);
                }
                break;
            
            case 'r':
                if(std::strcmp(arguments[0], "on") == 0) {
                    bitmap_options.filthy_sprite_bug_fix = true;
                }
                else if(std::strcmp(arguments[0], "off") == 0) {
                    bitmap_options.filthy_sprite_bug_fix = false;
                }
                else {
                    eprintf_error("Unknown registration point hack setting %s", arguments[0]);
                    std::exit(EXIT_FAILURE);
                }
                break;

            case 'D':
                if(std::strcmp(arguments[0],"a") == 0) {
                    bitmap_options.dither_alpha = true;
                    bitmap_options.dither_color = false;
                    bitmap_options.dithering = true;
                }
                else if(std::strcmp(arguments[0],"rgb") == 0) {
                    bitmap_options.dither_color = true;
                    bitmap_options.dither_alpha = false;
                    bitmap_options.dithering = true;
                }
                else if(std::strcmp(arguments[0],"argb") == 0) {
                    bitmap_options.dither_alpha = true;
                    bitmap_options.dither_color = true;
                    bitmap_options.dithering = true;
                }
                else if(std::strcmp(arguments[0],"none") == 0) {
                    bitmap_options.dither_alpha = false;
                    bitmap_options.dither_color = false;
                    bitmap_options.dithering = false;
                }
                else {
                    eprintf_error("Unknown dither type %s", arguments[0]);
                    std::exit(EXIT_FAILURE);
                }
                break;

            case 'p':
                if(strcmp(arguments[0],"on") == 0) {
                    bitmap_options.palettize = true;
                }
                else if(strcmp(arguments[0],"off") == 0) {
                    bitmap_options.palettize = false;
                }
                else {
                    eprintf_error("Unknown palettize setting %s", arguments[0]);
                    std::exit(EXIT_FAILURE);
                }
                break;

            case 'u':
                try {
                    bitmap_options.usage = BitmapUsage_from_string(arguments[0]);
                }
                catch(std::exception &) {
                    eprintf_error("Invalid bitmap usage %s", arguments[0]);
                    std::exit(EXIT_FAILURE);
                }
                break;

            case 'H':
                bitmap_options.bump_height = static_cast<float>(std::strtof(arguments[0], nullptr));
                break;

            case 'M':
                bitmap_options.max_mipmap_count = static_cast<std::uint32_t>(std::strtol(arguments[0], nullptr, 10));
                break;

            case 'C':
                bitmap_options.sprite_budget_count = static_cast<std::uint32_t>(std::strtoul(arguments[0], nullptr, 10));
                break;

            case 'B':
                bitmap_options.sprite_budget = static_cast<std::uint32_t>(std::strtoul(arguments[0], nullptr, 10));
                switch(bitmap_options.sprite_budget.value()) {
                    case 32:
                    case 64:
                    case 128:
                    case 256:
                    case 512:
                    case 1024:
                        break;
                    default:
                        eprintf_error("Invalid sprite budget %u.", bitmap_options.sprite_budget.value());
                        std::exit(EXIT_FAILURE);
                }

                break;
                
            case 'S':
                bitmap_options.force_square_sprite_sheets = true;
                break;

            case 'P':
                bitmap_options.filesystem_path = true;
                break;
        }
    });

    // Resolve the bitmap tag
    std::string bitmap_tag;
    SupportedFormatsInt found_format = static_cast<SupportedFormatsInt>(0);
    if(bitmap_options.filesystem_path) {
        auto bitmap_tag_maybe = File::file_path_to_tag_path(remaining_arguments[0], bitmap_options.tags);
        if(bitmap_tag_maybe.has_value() && std::filesystem::exists(remaining_arguments[0])) {
            bitmap_tag = std::filesystem::path(*bitmap_tag_maybe).replace_extension().string();
        }
        else if(!bitmap_options.regenerate) {
            auto bitmap_file_maybe = File::file_path_to_tag_path(remaining_arguments[0], bitmap_options.data);
            if(bitmap_file_maybe.has_value() && std::filesystem::exists(remaining_arguments[0])) {
                SupportedFormatsInt i;
                auto path_test = std::filesystem::path(*bitmap_file_maybe);
                auto extension = path_test.extension();
                bitmap_tag = std::filesystem::path(*bitmap_file_maybe).replace_extension().string();
                for(i = found_format; i < SupportedFormatsInt::SUPPORTED_FORMATS_INT_COUNT; i = static_cast<SupportedFormatsInt>(i + 1)) {
                    if(extension == SUPPORTED_FORMATS[i]) {
                        found_format = i;
                        break;
                    }
                }
            }
            else {
                eprintf_error("Failed to find a valid bitmap %s in the data or tags directories.", remaining_arguments[0]);
            }
        }
        else {
            eprintf_error("Failed to find a valid bitmap %s in the tags directory.", remaining_arguments[0]);
            return EXIT_FAILURE;
        }
    }
    
    else {
        bitmap_tag = remaining_arguments[0];
    }

    // Check if the tags directory exists
    if(!std::filesystem::is_directory(bitmap_options.tags)) {
        eprintf_error("Directory %s was not found or is not a directory", bitmap_options.tags.string().c_str());
        return EXIT_FAILURE;
    }

    auto tag_path = bitmap_options.tags / bitmap_tag;
    auto final_path_bitmap = std::filesystem::path(tag_path) += ".bitmap";
    return perform_the_ritual<Invader::Parser::Bitmap>(bitmap_tag, tag_path, final_path_bitmap, bitmap_options, found_format, TagFourCC::TAG_FOURCC_BITMAP);
}
