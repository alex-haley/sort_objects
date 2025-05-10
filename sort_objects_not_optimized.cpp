#include <cstdio>
#include <stdint.h>
#include <thread>
#include <mutex>

#define PI  3.14159265358979323846

typedef float       f32;
typedef uint32_t    u32;
typedef uint8_t     u8;

class base_shape
{
    public:
    base_shape() {}
    virtual f32 Area() = 0;
};

class circle : public base_shape
{
    public:
    circle(f32 init_radius) : radius(init_radius) {}
    virtual f32 Area() {return PI*radius*radius;}
    
    private:
    f32 radius;
};

class rectangle: public base_shape
{
    public:
    rectangle(f32 init_width, f32 init_height) : width(init_width), height(init_height) {}
    virtual f32 Area() {return width*height;}
    
    private:
    f32 width, height;
};

class triangle: public base_shape
{
    public:
    triangle(f32 init_base, f32 init_height) : base(init_base), height(init_height) {}
    virtual f32 Area() {return 0.5f*base*height;}
    
    private:
    f32 base, height;
};

// ===========================================================================================

void bubble_sort(u32 count_shapes, base_shape **Shapes)
{
    u32 area_count = 0;
    for (u32 counter = 0; counter < count_shapes*2-2; ++counter) {
        if (area_count >= count_shapes-1) {
            area_count = 0;
        }
        if (Shapes[area_count]->Area() > Shapes[area_count+1]->Area()) {
            base_shape *tmp = Shapes[area_count];
            
            Shapes[area_count] = Shapes[area_count+1];
            Shapes[area_count+1] = tmp;
        }
        ++area_count;
    }
}

void insertion_sort(u32 count_shapes, base_shape **Shapes)
{
    for (u32 i = 1; i < count_shapes; i++)
    {
        base_shape *value = Shapes[i];
        while ((i > 0) && (Shapes[i-1]->Area() > value->Area()))
        {
            Shapes[i] = Shapes[i-1];
            i--;
        }
        Shapes[i] = value;
    }
}

std::mutex mtx;
void pick_sort(u8 alg)
{
    const u32 count_shapes = 4;
    
    circle      c1(5.6f);
    circle      c2(6.4f);
    triangle    t1(4.0f, 6.3f);
    rectangle   r1(7.3f, 3.3f);
    
    base_shape* Shapes[count_shapes] = {&c1, &c2, &t1, &r1};
    
    printf("initial array:\n");
    for (u32 i = 0; i < count_shapes; i++)
    {
        printf("%f\n", Shapes[i]->Area());
    }
    printf("\n");
    
    char* name;
    switch(alg)
    {
        case 0:
        {
            name = "bubble_sort";
            bubble_sort(count_shapes, Shapes);
        } break;
        
        case 1:
        {
            name = "insertion_sort";
            insertion_sort(count_shapes, Shapes);
        } break;
        
        default: {}
    }
    
    std::lock_guard<std::mutex> lock(mtx);
    printf("sort type: %s\n", name);
    for (u32 i = 0; i < count_shapes; i++)
    {
        printf("\t%f\n", Shapes[i]->Area());
    }
    printf("\n");
}



int main(void)
{
    printf("\nexecuting sort algorithms in threads!\n\n");
    
    std::thread thr_1(pick_sort,0);
    thr_1.join();
    std::thread thr_2(pick_sort,1);
    thr_2.join();
    
    return 0;
}
