#include <vector>
#include <unordered_set>
#include <ctime>
#include <victini.h>
#include <cuapriltags/cuapriltags/cuAprilTags.h>
#include <wpimath/src/main/native/include/frc/geometry/Pose2d.h>
#include <wpimath/src/main/native/include/frc/geometry/Pose3d.h>
#include <wpimath/src/main/native/include/frc/geometry/Transform3d.h>
#include "cscore.h"

VI_NAMESPACE_BEGIN
namespace apriltags {
    constexpr uint32_t MAX_TAGS = 8;

    struct PoseObservation{
        double timestamp;
        frc::Pose3d pose;
        double ambiguity;
        int tag_count;
        double average_tag_distance;
    };

    struct DetectionList {
        cuAprilTagsID_t *tags_out;
        uint32_t *num_tags;
    };

    class Detector {
    public:
        Detector(const vi::cs::viCamera& camera);
        ~Detector();
        DetectionList detect();
    private:
        cuAprilTagsHandle* handle;
    };



    frc::Pose2d get_pose_observations(const vi::cs::viCamera& camera, const cuAprilTagsID_t *tags_out, const uint32_t num_tags){
        // Read new camera observations
        std::unordered_set<uint16_t> tag_ids{};
        std::vector<PoseObservation> pose_observations{};
        pose_observations.reserve(MAX_TAGS);

        for(uint32_t i = 0; i < num_tags; i++){
            const cuAprilTagsID_t target = tags_out[i];
            if(target.id < 0 || target.id > 22) continue;
            const frc::Pose3d tag_pose = apriltag_layout.getTagPose(target.id);

            const frc::Transform3d field_to_target = frc::Transform3d(tag_pose.Translation(), tag_pose.Rotation);
            const frc::Transform3d camera_to_target = target.bestCameraToTarget;
            const frc::Transform3d field_to_camera = field_to_target + camera_to_target.inverse();
            const frc::Transform3d field_to_robot = field_to_camera + camera.robot_to_camera.inverse();
            const frc::Pose3d robot_pose = frc::Pose3d(field_to_robot.Translation(), field_to_robot.Rotation());

            // Add tag ID
            tag_ids.insert(target.id);

            // Add observation
            pose_observations.push_back(
                PoseObservation{
                    .timestamp = std::time(0);
                    .pose = robot_pose;
                    .ambiguity = target.poseAmbiguity;
                    .tag_count = 1;
                    .average_tag_distance = camera_to_target.Translation().Norm();
                });
        }
        return pose_observations;
    }
}
VI_NAMESPACE_END