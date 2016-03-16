#include "ITMForestRelocalizer.h"

using namespace ITMLib::Engine;


/** Create new forest
*/
ITMForestRelocalizer::ITMForestRelocalizer(const ITMRGBDCalib *calib, Vector2i dims)
{
  data = new Data();
  ITMIntrinsics::ProjectionParamsSimple intrinsics = calib->intrinsics_d.projectionParamsSimple;
  settings = new Settings(dims.x, dims.y, 1, intrinsics.fx, intrinsics.fy, intrinsics.px, intrinsics.py);
  forest = new Forest<float, cv::Vec4b>(data, settings);
}

/** Load forest from file
*/
ITMForestRelocalizer::ITMForestRelocalizer(const ITMRGBDCalib *calib, Vector2i dims, const std::string file_name)
{
  data = new Data();
  ITMIntrinsics::ProjectionParamsSimple intrinsics = calib->intrinsics_d.projectionParamsSimple;
  settings = new Settings(dims.x, dims.y, 1, intrinsics.fx, intrinsics.fy, intrinsics.px, intrinsics.py);
  forest = new Forest<float, cv::Vec4b>(data, settings, file_name);
  bool isValid = forest->IsValid();
}

void ITMForestRelocalizer::RelocalizeCamera(ITMTrackingState *trackingState, const ITMView *view)
{
  
}

void ITMForestRelocalizer::CollectFrame(ITMTrackingState *trackingState, const ITMView *view)
{
  cv::Mat rgb_frame = cv::Mat(
    view->rgb->noDims.height,
    view->rgb->noDims.width,
    CV_8UC4,
    const_cast<unsigned char*>(view->rgb->GetData(MEMORYDEVICE_CPU)->getValues())
    );

  // create clone so we dont change actual image
  cv::Mat rgb_frame_sa = rgb_frame.clone();
  cv::cvtColor(rgb_frame_sa, rgb_frame_sa, CV_RGBA2BGRA);

  cv::Mat depth_frame = cv::Mat(
    view->depth->noDims.height,
    view->depth->noDims.width,
    CV_32F,
    view->depth->GetData(MEMORYDEVICE_CPU)
    );

  cv::Mat depth_frame_sa = depth_frame.clone();

  cv::imshow("rgb", rgb_frame_sa);
  cv::imshow("depth", depth_frame);


  Matrix3f r = trackingState->pose_d->GetR();
  Eigen::Matrix3d rot;
  rot << r.m00, r.m01, r.m02,
         r.m10, r.m11, r.m12,
         r.m20, r.m21, r.m22;

  Vector3f t = trackingState->pose_d->GetT();
  Eigen::Vector3d translation;
  translation << t.x, t.y, t.z;

  data->AddFrame(rgb_frame_sa, depth_frame_sa, EigenPose(rot, translation));
}

void ITMForestRelocalizer::Train()
{
  forest->Train();
}

void ITMForestRelocalizer::Test(int frame)
{
    Eigen::Affine3d pose = forest->Test(data->GetRGBImage(frame), data->GetDepthImage(frame));

    // compare pose to known value 
    auto known_pose = data->poses_eigen_.at(frame);

    cout << "found pose:" << endl;
    cout << pose.rotation() << endl << endl;
    cout << pose.rotation().eulerAngles(0, 1, 2) * 180 / M_PI << endl;
    cout << pose.translation() << endl;

    cout << "known pose:" << endl;
    cout << known_pose.first << endl;
    cout << known_pose.first.eulerAngles(0, 1, 2) * 180 / M_PI << endl;
    cout << known_pose.second << endl;
}

void ITMForestRelocalizer::SaveToFile(const string name)
{
  forest->Serialize(name);
}