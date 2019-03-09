#include "GodotPhoto.hpp"

using namespace godot;

GodotPhoto::GodotPhoto() {
    printf("GodotPhoto start\n");
    context = gp_context_new();
    gp_list_new(&list);
}

GodotPhoto::~GodotPhoto() {
    printf("GodotPhoto stop\n");
    clean();
    gp_context_unref(context);
    gp_list_free(list);
}

Array GodotPhoto::test() {
    gp_list_reset(list);
    int count = gp_camera_autodetect(list, context);
    if (count < GP_OK) {
        printf("No cameras detected.\n");
    } else {
        printf("Number of cameras: %d\n", count);
    }

    Array arr;
    const char	*name, *value;
    for (int i = 0; i < count; i++) {
        gp_list_get_name(list, i, &name);
        gp_list_get_value(list, i, &value);
        
        arr.append(Dictionary::make("name", name, "value", value));
        printf("Camera: %d\n \t%s\n \t%s\n", i, name, value);
    }


    return arr;
}

Ref<Image> GodotPhoto::getImg() {
    int size = 512;

    if (lastImage.is_valid()) {
        lastImage.unref();
    }
    lastImage.instance();

    lastImage.ptr()->create(size, size, false, Image::FORMAT_RGBA8);
    lastImage.ptr()->lock();
    for(int y = 0; y < size; y++){
        for(int x=0; x<size; x++){
            int i = 4*(x+y*size);
            float r = 0.5;
            float g = 0.5;
            float b = 0.5;
            float a = 0.5;
            lastImage.ptr()->set_pixel(x, y, Color(r, g, b, a));
        }
    }
    lastImage.ptr()->unlock();

    return lastImage;
}

void GodotPhoto::clean() {
    if (lastImage.is_valid()) {
        lastImage.unref();
    }
}
