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

            Array test();
            Ref<Image> getImg();
            void clean();
        
        private:
            GPContext *context;
            CameraList	*list;

            Ref<Image> lastImage;
        
        public:
            static void _register_methods() {
                register_method("test", &GodotPhoto::test);
                register_method("getImg", &GodotPhoto::getImg);
                register_method("clean", &GodotPhoto::clean);
            };
    };

}

#endif
