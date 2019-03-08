#include <gdnative_api_struct.gen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const godot_gdnative_core_api_struct *api = NULL;
const godot_gdnative_ext_nativescript_api_struct *nativescript_api = NULL;



void *godotPhoto_constructor(godot_object *p_instance, void *p_method_data);

void godotPhoto_destructor(godot_object *p_instance, void *p_method_data, void *p_user_data);

godot_variant godotPhoto_get_data(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args);
godot_variant godotPhoto_get_cameras(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, godot_variant **p_args);

void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *p_options) {
    api = p_options->api_struct;

    // now find our extensions
    for (int i = 0; i < api->num_extensions; i++) {
        switch (api->extensions[i]->type) {
            case GDNATIVE_EXT_NATIVESCRIPT: {
                nativescript_api = (godot_gdnative_ext_nativescript_api_struct *)api->extensions[i];
            }; break;
            default: break;
        }
    }
}

void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *p_options) {
    api = NULL;
    nativescript_api = NULL;
}

void GDN_EXPORT godot_nativescript_init(void *p_handle) {
    godot_instance_create_func create = { NULL, NULL, NULL };
    create.create_func = &godotPhoto_constructor;

    godot_instance_destroy_func destroy = { NULL, NULL, NULL };
    destroy.destroy_func = &godotPhoto_destructor;

    nativescript_api->godot_nativescript_register_class(p_handle, "GodotPhoto", "Reference", create, destroy);

    godot_instance_method get_data = { NULL, NULL, NULL };
    get_data.method = &godotPhoto_get_data;

    godot_method_attributes attributes = { GODOT_METHOD_RPC_MODE_DISABLED };

    nativescript_api->godot_nativescript_register_method(p_handle, "GodotPhoto", "get_data", attributes, get_data);


    godot_instance_method get_cameras = { NULL, NULL, NULL };
    get_cameras.method = &godotPhoto_get_cameras;
    // godot_method_attributes attributes = { GODOT_METHOD_RPC_MODE_DISABLED };
    nativescript_api->godot_nativescript_register_method(p_handle, "GodotPhoto", "get_cameras", attributes, get_cameras);
}