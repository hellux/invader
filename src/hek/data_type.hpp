/*
 * Invader (c) 2018 Kavawuvi
 *
 * This program is free software under the GNU General Public License v3.0 or later. See LICENSE for more information.
 */

#pragma once

#include <iostream>
#include <vector>

#include "../error.hpp"
#include "pad.hpp"
#include "class_int.hpp"
#include "endian.hpp"

/**
 * Calculate the required amount of padding to make a size divisible by 32 bits
 * @param  size size
 * @return      padding required
 */
#define REQUIRED_PADDING_32_BIT(size) static_cast<std::size_t>((~(size - 1)) & 3)

namespace Invader::HEK {
    using Pointer = std::uint32_t;
    using TagEnum = std::uint16_t;
    using Angle = float;
    using Fraction = float;

    /**
     * This refers to a tag ID when compiled.
     */
    union TagID {
        std::uint16_t index;
        std::uint32_t id;
    };

    /**
     * Single 20 character string
     */
    struct TagString {
        char string[0x20] = {};

        TagString() {}
        TagString(const std::string &copy) {
            if(copy.length() >= sizeof(string)) {
                throw OutOfBoundsException();
            }
            std::strcpy(string, copy.data());
        }
    };

    /**
     * Dependencies allow tags to reference other tags.
     */
    ENDIAN_TEMPLATE(EndianType) struct TagDependency {
        /** Tag class of the tag being depended upon */
        EndianType<TagClassInt> tag_class_int;

        /** Pointer to tag path when compiled. */
        EndianType<Pointer> path_pointer;

        /** Length of the path in bytes, not including the null terminator. */
        EndianType<std::uint32_t> path_size;

        /** Tag ID when compiled. Otherwise equals 0xFFFFFFFF */
        EndianType<TagID> tag_id;

        ENDIAN_TEMPLATE(OtherEndian) operator TagDependency<OtherEndian>() const {
            TagDependency<OtherEndian> copy;
            COPY_THIS(tag_class_int);
            COPY_THIS(path_pointer);
            COPY_THIS(path_size);
            COPY_THIS(tag_id);
            return copy;
        }
    };

    /**
     * Reflexives reference other tag blocks
     */
    template <template<typename> class EndianType, template<template<typename> typename> typename StructType> struct TagReflexive {
        /** Number of chunks */
        EndianType<std::uint32_t> count;

        /** Pointer to first block if compiled, otherwise I don't know what it does if it does anything */
        EndianType<Pointer> pointer;

        /** I don't know what this does but it doesn't seem to do anything. Always set to 0 when compiled. */
        EndianType<std::uint32_t> unknown;

        /** Little endian version of the struct type */
        using struct_type_little = StructType<LittleEndian>;

        /** Big endian version of the struct type */
        using struct_type_big = StructType<BigEndian>;

        /**
         * Return a pointer to the structs in data.
         * @param  data         reference to the data
         * @param  base_pointer base pointer to the beginning of the data
         * @return              pointer to the first struct or nullptr if count is 0
         * @throws              throw an exception if out of bounds
         */
        struct_type_little *get_structs(std::vector<std::byte> &data, Pointer base_pointer) const {
            if(count == 0) {
                return nullptr;
            }
            auto pointer = this->pointer.read();
            if(pointer < base_pointer) {
                throw OutOfBoundsException();
            }
            std::size_t offset = pointer - base_pointer;
            if(offset >= data.size() || offset + sizeof(struct_type_little) * this->count.read() > data.size()) {
                throw OutOfBoundsException();
            }
            return reinterpret_cast<struct_type_little *>(data.data() + offset);
        }

        operator TagReflexive<LittleEndian, StructType>() const {
            TagReflexive<LittleEndian, StructType> copy;
            COPY_THIS(count);
            copy.pointer = 0;
            copy.unknown = 0;
            return copy;
        }

        template <template<typename> class OtherEndian> operator TagReflexive<OtherEndian, StructType>() const {
            TagReflexive<OtherEndian, StructType> copy;
            COPY_THIS(count);
            COPY_THIS(pointer);
            COPY_THIS(unknown);
            return copy;
        }
    };
    static_assert(sizeof(TagReflexive<BigEndian, TagDependency>) == 0xC);

    /**
     * Rotation Matrix
     */
    ENDIAN_TEMPLATE(EndianType) struct Matrix {
        EndianType<float> matrix[3][3];

        ENDIAN_TEMPLATE(OtherEndian) operator Matrix<OtherEndian>() const {
            Matrix<OtherEndian> copy;
            COPY_THIS_ARRAY(matrix[0]);
            COPY_THIS_ARRAY(matrix[1]);
            COPY_THIS_ARRAY(matrix[2]);
            return copy;
        }
    };
    static_assert(sizeof(Matrix<BigEndian>) == 0x24);

    /**
     * Quaternion
     */
    ENDIAN_TEMPLATE(EndianType) struct Quaternion {
        EndianType<float> i;
        EndianType<float> j;
        EndianType<float> k;
        EndianType<float> w;

        ENDIAN_TEMPLATE(OtherEndian) operator Quaternion<OtherEndian>() const {
            Quaternion<OtherEndian> copy;
            COPY_THIS(i);
            COPY_THIS(j);
            COPY_THIS(k);
            COPY_THIS(w);
            return copy;
        }
    };
    static_assert(sizeof(Quaternion<BigEndian>) == 0x10);

    /**
     * RGB Color with alpha
     */
    ENDIAN_TEMPLATE(EndianType) struct ColorARGB {
        EndianType<float> alpha;
        EndianType<float> red;
        EndianType<float> green;
        EndianType<float> blue;

        ENDIAN_TEMPLATE(OtherEndian) operator ColorARGB<OtherEndian>() const {
            ColorARGB<OtherEndian> copy;
            COPY_THIS(alpha);
            COPY_THIS(red);
            COPY_THIS(green);
            COPY_THIS(blue);
            return copy;
        }
    };
    static_assert(sizeof(ColorARGB<BigEndian>) == 0x10);

    /**
     * Rectangle
     */
    ENDIAN_TEMPLATE(EndianType) struct Rectangle2D {
        EndianType<std::int16_t> top;
        EndianType<std::int16_t> left;
        EndianType<std::int16_t> bottom;
        EndianType<std::int16_t> right;

        ENDIAN_TEMPLATE(OtherEndian) operator Rectangle2D<OtherEndian>() const {
            Rectangle2D<OtherEndian> copy;
            COPY_THIS(top);
            COPY_THIS(left);
            COPY_THIS(bottom);
            COPY_THIS(right);
            return copy;
        }
    };
    static_assert(sizeof(Rectangle2D<BigEndian>) == 0x8);

    /**
     * RGB Color with alpha
     */
    struct ColorARGBInt {
        std::uint8_t alpha;
        std::uint8_t red;
        std::uint8_t green;
        std::uint8_t blue;
    };
    static_assert(sizeof(ColorARGBInt) == 0x4);

    /**
     * RGB Color
     */
    ENDIAN_TEMPLATE(EndianType) struct ColorRGB {
        EndianType<float> red;
        EndianType<float> green;
        EndianType<float> blue;

        ENDIAN_TEMPLATE(OtherEndian) operator ColorRGB<OtherEndian>() const {
            ColorRGB<OtherEndian> copy;
            COPY_THIS(red);
            COPY_THIS(green);
            COPY_THIS(blue);
            return copy;
        }
    };
    static_assert(sizeof(ColorRGB<BigEndian>) == 0xC);

    /**
     * 3D Vector
     */
    ENDIAN_TEMPLATE(EndianType) struct Euler3D {
        EndianType<float> yaw;
        EndianType<float> pitch;
        EndianType<float> roll;

        ENDIAN_TEMPLATE(OtherEndian) operator Euler3D<OtherEndian>() const {
            Euler3D<OtherEndian> copy;
            COPY_THIS(yaw);
            COPY_THIS(pitch);
            COPY_THIS(roll);
            return copy;
        }
    };
    static_assert(sizeof(Euler3D<BigEndian>) == 0xC);

    /**
     * 2D Vector
     */
    ENDIAN_TEMPLATE(EndianType) struct Euler2D {
        EndianType<float> yaw;
        EndianType<float> pitch;

        ENDIAN_TEMPLATE(OtherEndian) operator Euler2D<OtherEndian>() const {
            Euler2D<OtherEndian> copy;
            COPY_THIS(yaw);
            COPY_THIS(pitch);
            return copy;
        }
    };
    static_assert(sizeof(Euler2D<BigEndian>) == 0x8);

    /**
     * 2D Vector
     */
    ENDIAN_TEMPLATE(EndianType) struct Vector2D {
        EndianType<float> i;
        EndianType<float> j;

        ENDIAN_TEMPLATE(OtherEndian) operator Vector2D<OtherEndian>() const {
            Vector2D<OtherEndian> copy;
            COPY_THIS(i);
            COPY_THIS(j);
            return copy;
        }
    };
    static_assert(sizeof(Vector2D<BigEndian>) == 0x8);

    /**
     * 3D Vector
     */
    ENDIAN_TEMPLATE(EndianType) struct Vector3D {
        EndianType<float> i;
        EndianType<float> j;
        EndianType<float> k;

        ENDIAN_TEMPLATE(OtherEndian) operator Vector3D<OtherEndian>() const {
            Vector3D<OtherEndian> copy;
            COPY_THIS(i);
            COPY_THIS(j);
            COPY_THIS(k);
            return copy;
        }

        Vector3D<EndianType> operator+(const Vector3D<EndianType> &add) const {
            Vector3D<EndianType> copy;
            copy.i = this->i + add.i;
            copy.j = this->j + add.j;
            copy.k = this->k + add.k;
            return copy;
        }
    };
    static_assert(sizeof(Vector3D<BigEndian>) == 0xC);

    /**
     * 3D Plane
     */
    ENDIAN_TEMPLATE(EndianType) struct Plane3D {
        EndianType<float> x;
        EndianType<float> y;
        EndianType<float> z;
        EndianType<float> w;

        ENDIAN_TEMPLATE(OtherEndian) operator Plane3D<OtherEndian>() const {
            Plane3D<OtherEndian> copy;
            COPY_THIS(x);
            COPY_THIS(y);
            COPY_THIS(z);
            COPY_THIS(w);
            return copy;
        }
    };
    static_assert(sizeof(Plane3D<BigEndian>) == 0x10);

    /**
     * 2D Plane
     */
    ENDIAN_TEMPLATE(EndianType) struct Plane2D {
        EndianType<float> x;
        EndianType<float> y;
        EndianType<float> w;

        ENDIAN_TEMPLATE(OtherEndian) operator Plane2D<OtherEndian>() const {
            Plane2D<OtherEndian> copy;
            COPY_THIS(x);
            COPY_THIS(y);
            COPY_THIS(w);
            return copy;
        }
    };
    static_assert(sizeof(Plane2D<BigEndian>) == 0xC);

    /**
     * 2D Point
     */
    ENDIAN_TEMPLATE(EndianType) struct Point2D {
        EndianType<float> x;
        EndianType<float> y;

        ENDIAN_TEMPLATE(OtherEndian) operator Point2D<OtherEndian>() const {
            Point2D<OtherEndian> copy;
            COPY_THIS(x);
            COPY_THIS(y);
            return copy;
        }

        Point2D<EndianType> operator+(const Point2D<EndianType> &add) const {
            Point2D<EndianType> copy;
            copy.y = this->x + add.x;
            copy.x = this->y + add.y;
            return copy;
        }
    };
    static_assert(sizeof(Point2D<BigEndian>) == 0x8);

    /**
     * 3D Point
     */
    ENDIAN_TEMPLATE(EndianType) struct Point3D {
        EndianType<float> x;
        EndianType<float> y;
        EndianType<float> z;

        ENDIAN_TEMPLATE(OtherEndian) operator Point3D<OtherEndian>() const {
            Point3D<OtherEndian> copy;
            COPY_THIS(x);
            COPY_THIS(y);
            COPY_THIS(z);
            return copy;
        }

        ENDIAN_TEMPLATE(OtherEndian) operator Vector3D<OtherEndian>() const {
            Vector3D<OtherEndian> copy;
            copy.i = this->x;
            copy.j = this->y;
            copy.k = this->z;
            return copy;
        }

        Point3D<EndianType> operator+(const Point3D<EndianType> &add) const {
            Point3D<EndianType> copy;
            copy.y = this->x + add.x;
            copy.x = this->y + add.y;
            copy.z = this->z + add.z;
            return copy;
        }

        Point3D(const Vector3D<EndianType> &copy) : x(copy.i), y(copy.j), z(copy.k) {}
        Point3D(const Point3D<EndianType> &copy) : x(copy.x), y(copy.y), z(copy.z) {}
        Point3D() {}
    };
    static_assert(sizeof(Point3D<BigEndian>) == 0xC);

    /**
     * 2D Point
     */
    ENDIAN_TEMPLATE(EndianType) struct Point2DInt {
        EndianType<std::int16_t> x;
        EndianType<std::int16_t> y;

        ENDIAN_TEMPLATE(OtherEndian) operator Point2DInt<OtherEndian>() const {
            Point2DInt<OtherEndian> copy;
            COPY_THIS(x);
            COPY_THIS(y);
            return copy;
        }
    };
    static_assert(sizeof(Point2DInt<BigEndian>) == 0x4);

    /**
     * Bounds from one value to another
     */
    template <typename BoundsType> struct Bounds {
        BoundsType from;
        BoundsType to;

        template <typename NewType> operator Bounds<NewType>() const {
            Bounds<NewType> copy;
            COPY_THIS(from);
            COPY_THIS(to);
            return copy;
        }
    };
    static_assert(sizeof(Bounds<BigEndian<float>>) == 0x8);

    #define SINGLE_DEPENDENCY_STRUCT(name, dependency) ENDIAN_TEMPLATE(EndianType) struct name {\
        TagDependency<EndianType> dependency;\
        ENDIAN_TEMPLATE(NewType) operator name <NewType>() const noexcept {\
            name <NewType> copy = {};\
            COPY_THIS(dependency);\
            return copy;\
        }\
    }

    #define SINGLE_DEPENDENCY_PADDED_STRUCT(name, dependency, pad) ENDIAN_TEMPLATE(EndianType) struct name {\
        TagDependency<EndianType> dependency;\
        PAD(pad);\
        ENDIAN_TEMPLATE(NewType) operator name <NewType>() const noexcept {\
            name <NewType> copy = {};\
            COPY_THIS(dependency);\
            return copy;\
        }\
    }

    enum PredictedResourceType : TagEnum {
        PREDICTED_RESOUCE_TYPE_BITMAP,
        PREDICTED_RESOUCE_TYPE_SOUND
    };

    ENDIAN_TEMPLATE(EndianType) struct PredictedResource {
        EndianType<PredictedResourceType> type;
        EndianType<std::int16_t> resource_index;
        EndianType<TagID> tag;

        ENDIAN_TEMPLATE(OtherType) operator PredictedResource<OtherType>() const noexcept {
            PredictedResource<OtherType> copy;
            COPY_THIS(type);
            COPY_THIS(resource_index);
            COPY_THIS(tag);
            return copy;
        }
    };
    static_assert(sizeof(PredictedResource<BigEndian>) == 0x8);

    ENDIAN_TEMPLATE(EndianType) struct TagDataOffset {
        EndianType<std::int32_t> size;
        LittleEndian<std::int32_t> external;
        LittleEndian<std::int32_t> file_offset;
        LittleEndian<Pointer> pointer;
        PAD(0x4);

        ENDIAN_TEMPLATE(OtherType) operator TagDataOffset<OtherType>() const noexcept {
            TagDataOffset<OtherType> copy = {};
            COPY_THIS(size);
            COPY_THIS(external);
            COPY_THIS(file_offset);
            COPY_THIS(pointer);
            return copy;
        }
    };
    static_assert(sizeof(TagDataOffset<BigEndian>) == 0x14);

    enum MaterialType : TagEnum {
        MATERIAL_TYPE_DIRT,
        MATERIAL_TYPE_SAND,
        MATERIAL_TYPE_STONE,
        MATERIAL_TYPE_SNOW,
        MATERIAL_TYPE_WOOD,
        MATERIAL_TYPE_METAL_HOLLOW,
        MATERIAL_TYPE_METAL_THIN,
        MATERIAL_TYPE_METAL_THICK,
        MATERIAL_TYPE_RUBBER,
        MATERIAL_TYPE_GLASS,
        MATERIAL_TYPE_FORCE_FIELD,
        MATERIAL_TYPE_GRUNT,
        MATERIAL_TYPE_HUNTER_ARMOR,
        MATERIAL_TYPE_HUNTER_SKIN,
        MATERIAL_TYPE_ELITE,
        MATERIAL_TYPE_JACKAL,
        MATERIAL_TYPE_JACKAL_ENERGY_SHIELD,
        MATERIAL_TYPE_ENGINEER_SKIN,
        MATERIAL_TYPE_ENGINEER_FORCE_FIELD,
        MATERIAL_TYPE_FLOOD_COMBAT_FORM,
        MATERIAL_TYPE_FLOOD_CARRIER_FORM,
        MATERIAL_TYPE_CYBORG_ARMOR,
        MATERIAL_TYPE_CYBORG_ENERGY_SHIELD,
        MATERIAL_TYPE_HUMAN_ARMOR,
        MATERIAL_TYPE_HUMAN_SKIN,
        MATERIAL_TYPE_SENTINEL,
        MATERIAL_TYPE_MONITOR,
        MATERIAL_TYPE_PLASTIC,
        MATERIAL_TYPE_WATER,
        MATERIAL_TYPE_LEAVES,
        MATERIAL_TYPE_ELITE_ENERGY_SHIELD,
        MATERIAL_TYPE_ICE,
        MATERIAL_TYPE_HUNTER_SHIELD
    };

    enum FunctionType : TagEnum {
        FUNCTION_LINEAR,
        FUNCTION_EARLY,
        FUNCTION_VERY_EARLY,
        FUNCTION_LATE,
        FUNCTION_VERY_LATE,
        FUNCTION_COSINE
    };

    enum FunctionType2 : TagEnum {
        FUNCTION_TYPE_ONE,
        FUNCTION_TYPE_ZERO,
        FUNCTION_TYPE_COSINE,
        FUNCTION_TYPE_COSINE_VARIABLE_PERIOD,
        FUNCTION_TYPE_DIAGONAL_WAVE,
        FUNCTION_TYPE_DIAGONAL_WAVE_VARIABLE_PERIOD,
        FUNCTION_TYPE_SLIDE,
        FUNCTION_TYPE_SLIDE_VARIABLE_PERIOD,
        FUNCTION_TYPE_NOISE,
        FUNCTION_TYPE_JITTER,
        FUNCTION_TYPE_WANDER,
        FUNCTION_TYPE_SPARK
    };

    enum FunctionBoundsMode : TagEnum {
        FUNCTION_BOUNDS_MODE_CLIP,
        FUNCTION_BOUNDS_MODE_CLIP_AND_NORMALIZE,
        FUNCTION_BOUNDS_MODE_SCALE_TO_FIT
    };

    enum FunctionScaleBy : TagEnum {
        FUNCTION_SCALE_PERIOD_BY_NONE,
        FUNCTION_SCALE_PERIOD_BY_A_IN,
        FUNCTION_SCALE_PERIOD_BY_B_IN,
        FUNCTION_SCALE_PERIOD_BY_C_IN,
        FUNCTION_SCALE_PERIOD_BY_D_IN,
        FUNCTION_SCALE_PERIOD_BY_A_OUT,
        FUNCTION_SCALE_PERIOD_BY_B_OUT,
        FUNCTION_SCALE_PERIOD_BY_C_OUT,
        FUNCTION_SCALE_PERIOD_BY_D_OUT
    };

    Vector3D<NativeEndian> euler2d_to_vector(const Euler2D<NativeEndian> &rotation) noexcept;

    Quaternion<NativeEndian> vector_to_quaternion(const Vector3D<NativeEndian> &vector) noexcept;
    Quaternion<NativeEndian> euler_to_quaternion(const Euler3D<NativeEndian> &rotation) noexcept;
    Matrix<NativeEndian> quaternion_to_matrix(const Quaternion<NativeEndian> &rotation) noexcept;

    Matrix<NativeEndian> multiply_matrix(const Matrix<NativeEndian> &rotation, float value) noexcept;
    Matrix<NativeEndian> multiply_matrix(const Matrix<NativeEndian> &rotation, const Matrix<NativeEndian> &value) noexcept;
    Matrix<NativeEndian> invert_matrix(const Matrix<NativeEndian> &rotation) noexcept;

    Vector3D<NativeEndian> add_vector(const Vector3D<NativeEndian> &vector, const Vector3D<NativeEndian> &value) noexcept;
    Vector3D<NativeEndian> multiply_vector(const Vector3D<NativeEndian> &vector, float value) noexcept;
    Vector3D<NativeEndian> rotate_vector(const Vector3D<NativeEndian> &vector, const Quaternion<NativeEndian> &rotation) noexcept;
    Vector3D<NativeEndian> rotate_vector(const Vector3D<NativeEndian> &vector, const Matrix<NativeEndian> &rotation) noexcept;
}
