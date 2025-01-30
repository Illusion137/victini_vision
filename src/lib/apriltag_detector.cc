#include "apriltags.h"

vi::apriltags::Detector::Detector(const vi::cs::viCamera& camera){
    this->handle = nullptr;
    const bool result = nvCreateAprilTagsDetector(
        handle, 
        camera.width, 
        camera.height,
        1, 
        cuAprilTagsFamily::NVAT_TAG36H11, 
        static_cast<cuAprilTagsCameraIntrinsics_t>(camera.intrinsics), 
        0.0f
    );

    VI_CHECK(VI_ISOK(result), "Failed to create apriltag detector");
}

vi::apriltags::Detector::~Detector(){
    cuAprilTagsDestroy(this->handle);
}

vi::apriltags::DetectionList vi::apriltags::Detector::detect(){
    DetectionList detections{};
    const uint32_t result = cuAprilTagsDetect(this->handle, img_input, detections.tags_out, detections.num_tags, MAX_TAGS, NULL);
    // VI_ISOK(result);
    return detections;
}