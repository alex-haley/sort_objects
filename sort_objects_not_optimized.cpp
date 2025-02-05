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
    bool no_swaps = true;
    for (u32 i = 0; i < count_shapes - 1; i++)
    {
        for (u32 area_count = 0; area_count < count_shapes - 1; area_count++)
        {
            if (Shapes[area_count]->Area() > Shapes[area_count+1]->Area())
            {
                no_swaps = false;
                base_shape *tmp = Shapes[area_count];
                Shapes[area_count] = Shapes[area_count+1];
                Shapes[area_count+1] = tmp;
            }
        }
    }
    if (no_swaps)
    {
        return;
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

void merge_sort(u32 count_shapes, base_shape **Shapes)
{
    u32 mid;
    mid = count_shapes / 2;
    if (count_shapes > 1)
    {
        base_shape *left_half[mid];
        base_shape *right_half[mid];

        for (u32 i = 0; i < mid; i++)
        {
            left_half[i] = Shapes[i];
        }
	u32 z = mid;
        for (u32 i = 0; i < mid; i++)
        {
	    right_half[i] = Shapes[z];
	    z++;
        }

        merge_sort(mid, left_half);
        merge_sort(mid, right_half);

        u32 left_ind = 0;
        u32 right_ind = 0;
        u32 shapes_ind = 0;
        while ((left_ind < mid) && (right_ind < mid))
        {
	    if (left_half[left_ind]->Area() <= right_half[right_ind]->Area())
            {
                Shapes[shapes_ind] = left_half[left_ind];
                left_ind++;
            }
            else
            {
                Shapes[shapes_ind] = right_half[right_ind];
                right_ind++;
            }
            shapes_ind++;
        }

        while (left_ind < mid)
        {
            Shapes[shapes_ind] = left_half[left_ind];
            left_ind++;
            shapes_ind++;
        }

        while (right_ind < mid)
        {
            Shapes[shapes_ind] = right_half[right_ind];
            right_ind++;
            shapes_ind++;
        }
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

        case 2:
        {
            name = "merge_sort";
            merge_sort(count_shapes, Shapes);
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
    // THIS IS THE DIRECT CALL OF SORT FUNCTIONS
    // UNCOMMENT THIS CODE TO CALL SORTING DIRECTLY
    //
    // const u32 count_shapes = 4;

    // circle      c1(5.6f);
    // circle      c2(6.4f);
    // triangle    t1(4.0f, 6.3f);
    // rectangle   r1(7.3f, 3.3f);

    // base_shape* Shapes1[count_shapes] = {&c1, &c2, &t1, &r1};
    // base_shape* Shapes2[count_shapes] = {&c1, &c2, &t1, &r1};
    // base_shape* Shapes3[count_shapes] = {&c1, &c2, &t1, &r1};

    // bubble_sort(count_shapes, Shapes1);
    // insertion_sort(count_shapes, Shapes2);
    // merge_sort(count_shapes, Shapes3);

    // printf("\nSTART OF DIRECT CALL\n\n");

    // printf("bubble_sort:\n");
    // for (u32 i = 0; i < count_shapes; i++)
    // {
    //     printf("\t%f\n", Shapes1[i]->Area());
    // }
    // printf("\ninsertion_sort:\n");
    // for (u32 i = 0; i < count_shapes; i++)
    // {
    //     printf("\t%f\n", Shapes2[i]->Area());
    // }
    // printf("\nmerge_sort:\n");
    // for (u32 i = 0; i < count_shapes; i++)
    // {
    //     printf("\t%f\n", Shapes3[i]->Area());
    // }
    // printf("\nEND OF DIRECT CALL\n\n");

    printf("\nexecuting sort algorithms in threads!\n\n");

    std::thread thr_1(pick_sort,0);
    thr_1.join();
    std::thread thr_2(pick_sort,1);
    thr_2.join();
    std::thread thr_3(pick_sort,2);
    thr_3.join();

    return 0;
}
