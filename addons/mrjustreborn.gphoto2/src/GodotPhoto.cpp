#include "GodotPhoto.hpp"

using namespace godot;

GodotPhoto::GodotPhoto() {
    printf("GodotPhoto start\n");
    abilities = NULL;
    portinfolist = NULL;
    lastId = -1;
    isSetUp = false;

    setUp();
}

GodotPhoto::~GodotPhoto() {
    printf("GodotPhoto stop\n");
    reset();
}

Array GodotPhoto::get_connected_cameras() {
    setUp();
    Array _cameras;

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
        
        _cameras.append(Dictionary::make("id", i, "name", name, "port", value));
        printf("Camera: %d\n \t%s\n \t%s\n", i, name, value);
    }

    return _cameras;
}

Ref<Image> GodotPhoto::capture(int ptr) {
    setUpCamera(ptr);

    char *data;
    unsigned long size;
    capture_to_memory(camera, context,(const char**)&data, &size);

    if (lastImage.is_valid()) {
        lastImage.unref();
    }
    lastImage.instance();

    PoolByteArray bArr;
    for (int i=0; i<size; i++) {
        bArr.append((*(data + i)));
    }
    lastImage->load_jpg_from_buffer(bArr);

    return lastImage;
}

Ref<Image> GodotPhoto::capture_preview(int ptr) {
    setUpCamera(ptr);

    char *data;
    unsigned long size;
    capture_preview_to_memory(camera, context,(const char**)&data, &size);

    if (lastImage.is_valid()) {
        lastImage.unref();
    }
    lastImage.instance();

    PoolByteArray bArr;
    for (int i=0; i<size; i++) {
        bArr.append((*(data + i)));
    }
    lastImage->load_jpg_from_buffer(bArr);

    return lastImage;
}

void GodotPhoto::auto_focus(bool on) {
    camera_auto_focus(camera, context, on ? 1 : 0);
}

void GodotPhoto::reset() {
    if (lastImage.is_valid()) {
        lastImage.unref();
    }

    if (isSetUp) {
        gp_camera_exit(camera, context);
        gp_list_free(list);
        gp_context_unref(context);

        abilities = NULL;
        portinfolist = NULL;
        lastId = -1;

        isSetUp = false;
    }
}

void GodotPhoto::setUp() {
    if (!isSetUp) {
        context = gp_context_new();
        gp_list_new(&list);
        gp_camera_new(&camera);

        isSetUp = true;
    }
}

int GodotPhoto::setUpCamera(int ptr) {
    setUp();
    int count = gp_list_count(list);
    if (ptr > count-1 || ptr < 0) {
        printf("Unknow camera: %i!\n", ptr);
        reset();
    } else {
        if (ptr != lastId) {
            const char *name, *value;
            gp_list_get_name(list, ptr, &name);
            gp_list_get_value(list, ptr, &value);
            printf("Try to open: %s on %s\n", name, value);
            int ret = open_camera(&camera, name, value, context);
            if (ret < GP_OK) {
                reset();
                printf("Could not open: %s on %s\n", name, value);
                return ret;
            }
            lastId = ptr;
        }
    }
    return -1;
}

int GodotPhoto::camera_auto_focus(Camera *camera, GPContext *context, int onoff) {
    CameraWidget		*widget = NULL, *child = NULL;
    CameraWidgetType	type;
    int			ret,val;

    ret = gp_camera_get_config (camera, &widget, context);
    if (ret < GP_OK) {
        fprintf (stderr, "camera_get_config failed: %d\n", ret);
        return ret;
    }
    ret = lookup_widget (widget, "autofocusdrive", &child);
    if (ret < GP_OK) {
        fprintf (stderr, "lookup 'autofocusdrive' failed: %d\n", ret);
        goto out;
    }

    /* check that this is a toggle */
    ret = gp_widget_get_type (child, &type);
    if (ret < GP_OK) {
        fprintf (stderr, "widget get type failed: %d\n", ret);
        goto out;
    }
    switch (type) {
        case GP_WIDGET_TOGGLE:
        break;
    default:
        fprintf (stderr, "widget has bad type %d\n", type);
        ret = GP_ERROR_BAD_PARAMETERS;
        goto out;
    }

    ret = gp_widget_get_value (child, &val);
    if (ret < GP_OK) {
        fprintf (stderr, "could not get widget value: %d\n", ret);
        goto out;
    }

    val = onoff;

    ret = gp_widget_set_value (child, &val);
    if (ret < GP_OK) {
        fprintf (stderr, "could not set widget value to 1: %d\n", ret);
        goto out;
    }

    ret = gp_camera_set_config (camera, widget, context);
    if (ret < GP_OK) {
        fprintf (stderr, "could not set config tree to autofocus: %d\n", ret);
        goto out;
    }
    out:
    gp_widget_free (widget);
    return ret;
}

int GodotPhoto::lookup_widget(CameraWidget*widget, const char *key, CameraWidget **child) {
    int ret;
    ret = gp_widget_get_child_by_name (widget, key, child);
    if (ret < GP_OK)
        ret = gp_widget_get_child_by_label (widget, key, child);
    return ret;
}

void GodotPhoto::capture_preview_to_memory(Camera *camera, GPContext *context, const char **ptr, unsigned long int *size) {
    int retval;
    CameraFile *file;

    printf("Capturing preview.\n");

    retval = gp_file_new(&file);
    printf("  Retval: %d\n", retval);
    retval = gp_camera_capture_preview(camera, file, context);
    printf("  Retval: %d\n", retval);

    gp_file_get_data_and_size (file, ptr, size);

    const char **mime;
    gp_file_get_mime_type(file, mime);
    printf("MIME: %s\n", mime);
}

void GodotPhoto::capture_to_memory(Camera *camera, GPContext *context, const char **ptr, unsigned long int *size) {
    int retval;
    CameraFile *file;
    CameraFilePath camera_file_path;

    printf("Capturing.\n");

    /* NOP: This gets overridden in the library to /capt0000.jpg */
    strcpy(camera_file_path.folder, "/");
    strcpy(camera_file_path.name, "foo.jpg");

    retval = gp_camera_capture(camera, GP_CAPTURE_IMAGE, &camera_file_path, context);
    printf("  Retval: %d\n", retval);

    printf("Pathname on the camera: %s/%s\n", camera_file_path.folder, camera_file_path.name);

    retval = gp_file_new(&file);
    printf("  Retval: %d\n", retval);
    retval = gp_camera_file_get(camera, camera_file_path.folder, camera_file_path.name,
                GP_FILE_TYPE_NORMAL, file, context);
    printf("  Retval: %d\n", retval);

    gp_file_get_data_and_size (file, ptr, size);

    printf("Deleting.\n");
    retval = gp_camera_file_delete(camera, camera_file_path.folder, camera_file_path.name,
            context);
    printf("  Retval: %d\n", retval);
    /*gp_file_free(file); */
}

int GodotPhoto::open_camera(Camera ** camera, const char *model, const char *port, GPContext *context) {
    int		ret, m, p;
    CameraAbilities	a;
    GPPortInfo	pi;


    if (!abilities) {
        /* Load all the camera drivers we have... */
        printf("Load abilities\n");
        ret = gp_abilities_list_new (&abilities);
        printf("Loaded abilities\n");
        if (ret < GP_OK) return ret;
        ret = gp_abilities_list_load (abilities, context);
        if (ret < GP_OK) return ret;
    }

    /* First lookup the model / driver */
    m = gp_abilities_list_lookup_model (abilities, model);
    if (m < GP_OK) return ret;
        ret = gp_abilities_list_get_abilities (abilities, m, &a);
    if (ret < GP_OK) return ret;
        ret = gp_camera_set_abilities (*camera, a);
    if (ret < GP_OK) return ret;

    if (!portinfolist) {
        /* Load all the port drivers we have... */
        ret = gp_port_info_list_new (&portinfolist);
        if (ret < GP_OK) return ret;
        ret = gp_port_info_list_load (portinfolist);
        if (ret < 0) return ret;
        ret = gp_port_info_list_count (portinfolist);
        if (ret < 0) return ret;
    }

    /* Then associate the camera with the specified port */
    p = gp_port_info_list_lookup_path (portinfolist, port);
    switch (p) {
    case GP_ERROR_UNKNOWN_PORT:
            fprintf (stderr, "The port you specified "
                    "('%s') can not be found. Please "
                    "specify one of the ports found by "
                    "'gphoto2 --list-ports' and make "
                    "sure the spelling is correct "
                    "(i.e. with prefix 'serial:' or 'usb:').",
                            port);
            break;
    default:
            break;
    }
    if (p < GP_OK) return p;

    ret = gp_port_info_list_get_info (portinfolist, p, &pi);
        if (ret < GP_OK) return ret;
        ret = gp_camera_set_port_info (*camera, pi);
        if (ret < GP_OK) return ret;
    return GP_OK;
}
