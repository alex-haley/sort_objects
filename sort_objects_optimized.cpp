/* date = 6th of May 2025 */

#define internal static

#include <stdint.h>

typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef bool  b32;

#include <stdio.h>
#include <stdlib.h>

f32 PI = 3.141592653589f;

enum Sort_Alg {
    Bubble_Sort,
    Insertion_Sort,
    Merge_Sort,
    Alg_Count,
};

enum Shape_Type {
    Shape_Square,
    Shape_Rect,
    Shape_Triangle,
    Shape_Circle,
    Shape_Count,
};

struct Shape {
    Shape_Type type;
    union {
        struct {
            union {
                f32 width;
                f32 base;
            };
            f32 height;
        };
        f32 side;
        f32 radius;
    };
};

internal f32
Area(Shape base_shape)
{
    f32 Result = 0;
    
    f32 BaseArea[Shape_Count] = {
        base_shape.side*2.0f,
        base_shape.width*base_shape.height,
        base_shape.base*base_shape.height,
        base_shape.radius*PI*2.0f
    };
    
    Result = BaseArea[base_shape.type];
    return Result;
}

// ===========================================

#define SORT_WITH(name) void name(Shape **base_shape, u32 shape_count)
typedef SORT_WITH(sort_with);

SORT_WITH(bubble_sort)
{
    u8 i = 0;
    for (u8 counter = 0; counter < shape_count*2-2; ++counter) {
        if (i >= shape_count-1) {i = 0;}
        if (Area(*base_shape[i]) > Area(*base_shape[i+1])) {
            Shape tmp = *base_shape[i];
            
            *base_shape[i]   = *base_shape[i+1];
            *base_shape[i+1] = tmp;
        }
        ++i;
    }
}

SORT_WITH(insertion_sort)
{
    for (u32 i = 1; i < shape_count; ++i) {
        Shape value = *base_shape[i];
        for (;(i > 0) && (Area(*base_shape[i-1]) > Area(value));--i) {
            *base_shape[i] = *base_shape[i-1];
        }
        *base_shape[i] = value;
    }
}

internal Shape
create_shape(Shape_Type type, f32 value)
{
    Shape shape = {};
    switch (type) {
        case Shape_Square: {shape.type = type; shape.side = value;} break;
        case Shape_Circle: {shape.type = type; shape.radius = value;} break;
    }
    return shape;
}

internal Shape
create_shape(Shape_Type type, f32 value1, f32 value2)
{
    Shape shape = {};
    switch (type) {
        case Shape_Rect: {
            shape.type = type;
            shape.width = value1;
            shape.height = value2;
        } break;
        
        case Shape_Triangle: {
            shape.type = type;
            shape.base = value1;
            shape.height = value2;
        } break;
    }
    return shape;
}

internal void
choose_sort_alg(Sort_Alg sort_alg, Shape **base_shapes, u32 shape_count)
{
    for (u32 i = 0; i < shape_count; ++i) {
        printf("%f\n", Area(*base_shapes[i]));
    }
    printf("\n");
    
    switch (sort_alg) {
        case Bubble_Sort:    {bubble_sort(base_shapes, shape_count);} break;
        case Insertion_Sort: {insertion_sort(base_shapes, shape_count);} break;
    }
    
    for (u32 i = 0; i < shape_count; ++i) {
        printf("%f\n", Area(*base_shapes[i]));
    }
    printf("\n");
}

int main()
{
    const u32 shape_count = 4;
    Shape *base_shapes[shape_count];
    
    Shape shape1 = create_shape(Shape_Rect, 10.0f, 30.0f);
    base_shapes[0] = &shape1;
    
    Shape shape2 = create_shape(Shape_Square, 20.0f);
    base_shapes[1] = &shape2;
    
    Shape shape3 = create_shape(Shape_Circle, 5.0f);
    base_shapes[2] = &shape3;
    
    Shape shape4 = create_shape(Shape_Triangle, 7.5f, 10.0f);
    base_shapes[3] = &shape4;
    
    choose_sort_alg(Bubble_Sort, base_shapes, shape_count);
    
    return 0;
}
