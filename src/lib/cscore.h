#include <wpimath/src/main/native/include/frc/geometry/Transform3d.h>
#include <victini.h>

VI_NAMESPACE_BEGIN
namespace cs {
    struct viCameraIntrinsics {
        float fx, fy, cx, cy;
    };
    struct viCamera {
        frc::Transform3d robot_to_camera;
        uint32_t height;
        uint32_t width;
        viCameraIntrinsics intrinsics;
        char* name;
        char* path;
    };
}
VI_NAMESPACE_END