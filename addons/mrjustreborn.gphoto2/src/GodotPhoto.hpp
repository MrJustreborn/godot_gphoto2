#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <Godot.hpp>
#include <gphoto2/gphoto2.h>
#include <ImageTexture.hpp>

namespace godot {

    class GodotPhoto : public godot::GodotScript<Reference> {
        GODOT_CLASS(GodotPhoto)

        public:
            GodotPhoto();
            ~GodotPhoto();

            Array get_connected_cameras();

            Ref<Image> capture_preview(int ptr);
            Ref<Image> capture(int ptr);

            void clean();
            void setUp();
        
        private:
            int lastId;
            bool isSetUp;

            GPContext *context;
            CameraList	*list;
            Camera	*camera;

            CameraAbilitiesList	*abilities;
            GPPortInfoList *portinfolist;

            Ref<Image> lastImage;

            int open_camera(Camera ** camera, const char *model, const char *port, GPContext *context);
            void capture_to_memory(Camera *camera, GPContext *context, const char **ptr, unsigned long int *size);
            void capture_preview_to_memory(Camera *camera, GPContext *context, const char **ptr, unsigned long int *size);
            int setUpCamera(int prt);
        
        public:
            static void _register_methods() {
                register_method("get_connected_cameras", &GodotPhoto::get_connected_cameras);
                register_method("capture_preview", &GodotPhoto::capture_preview);
                register_method("capture", &GodotPhoto::capture);
                register_method("clean", &GodotPhoto::clean);
            };
    };

}

#endif
