#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <Godot.hpp>
#include <gphoto2/gphoto2.h>
#include <ImageTexture.hpp>

namespace godot {

    class gdexample : public godot::GodotScript<Reference> {
        GODOT_CLASS(gdexample)

        public:
            gdexample();
            ~gdexample();

            Array test();
            Ref<Image> getImg();
            void clean();
        
        private:
            GPContext *context;
            CameraList	*list;

            Ref<Image> lastImage;
        
        public:
            static void _register_methods() {
                register_method("test", &gdexample::test);
                register_method("getImg", &gdexample::getImg);
                register_method("clean", &gdexample::clean);
            };
    };

}

#endif
