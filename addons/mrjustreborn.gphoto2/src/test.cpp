#include "test.hpp"

using namespace godot;

gdexample::gdexample() {
    printf("gdexample init\n");
    // context = gp_context_new();
    // arr = PoolByteArray();
}

gdexample::~gdexample() {
    printf("gdexample removed\n");
    // gp_context_unref(context);
    // gp_list_free(list);
}

float gdexample::test() {
    gp_list_new(&list);

    gp_list_reset(list);
    int count = gp_camera_autodetect(list, context);
    if (count < GP_OK) {
        printf("No cameras detected.\n");
    } else {
        printf("Number of cameras: %d\n", count);
    }

    const char	*name, *value;
    for (int i = 0; i < count; i++) {
        gp_list_get_name(list, i, &name);
        gp_list_get_value(list, i, &value);

        printf("Camera: %d\n \t%s\n \t%s\n", i, name, value);
    }

    return count;
}

Ref<Image> gdexample::getImg() {
    int size = 512;

    if (ref.is_valid()) {
        ref.unref();
    }
    ref.instance();

    ref.ptr()->create(size, size, false, Image::FORMAT_RGBA8);
    ref.ptr()->lock();
    for(int y = 0; y < size; y++){
        for(int x=0; x<size; x++){
            int i = 4*(x+y*size);
            float r = 0.5;
            float g = 0.5;
            float b = 0.5;
            float a = 0.5;
            ref.ptr()->set_pixel(x, y, Color(r, g, b, a));
        }
    }
    ref.ptr()->unlock();

    Test t;
    t.pr();

    return ref;
}

void gdexample::clean() {
    if (ref.is_valid()) {
        ref.unref();
    }
}

Test::Test() {
    printf("test init\n");
}
Test::~Test() {
    printf("test removed\n");
}
void Test::pr() {
    printf("Test printer!\n");
}