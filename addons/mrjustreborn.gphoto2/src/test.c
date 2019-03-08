#include "test.h"
#include <gphoto2/gphoto2.h>

typedef struct user_data_struct {
    char data[256];
    CameraList	*list;
    GPContext *context;
} user_data_struct;

void *godotPhoto_constructor(godot_object *p_instance, void *p_method_data) {
    user_data_struct *user_data = api->godot_alloc(sizeof(user_data_struct));
    strcpy(user_data->data, "World from GDNative222222!");

    user_data->context = gp_context_new();

    return user_data;
}
void godotPhoto_destructor(godot_object *p_instance, void *p_method_data, void *p_user_data) {
    user_data_struct *user_data = (user_data_struct *) p_user_data;
    gp_list_free(user_data->list);
    gp_context_unref(user_data->context);

    api->godot_free(p_user_data);
}

godot_variant godotPhoto_get_cameras(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args) {
    user_data_struct * user_data = (user_data_struct *) p_user_data;

    gp_list_new(&user_data->list);

    gp_list_reset(user_data->list);
    int count = gp_camera_autodetect(user_data->list, user_data->context);
	if (count < GP_OK) {
		printf("No cameras detected.\n");
	} else {
        printf("Number of cameras: %d\n", count);
    }

    const char	*name, *value;
    for (int i = 0; i < count; i++) {
        gp_list_get_name(user_data->list, i, &name);
        gp_list_get_value(user_data->list, i, &value);

        printf("Camera: %d\n \t%s\n \t%s\n", i, name, value);
    }

    godot_variant ret;
    api->godot_variant_new_int(&ret, count);

    return ret;
}

godot_variant godotPhoto_get_data(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args) {
    godot_string data;
    godot_variant ret;
    user_data_struct * user_data = (user_data_struct *) p_user_data;

    api->godot_string_new(&data);
    api->godot_string_parse_utf8(&data, user_data->data);
    api->godot_variant_new_string(&ret, &data);
    api->godot_string_destroy(&data);

    return ret;
}
