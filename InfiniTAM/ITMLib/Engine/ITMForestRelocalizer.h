#pragma once

#include "../Utils/ITMLibDefines.h"

#include "../Engine/ITMRelocalizer.h"

#include "../../relocforests/include/reader.hpp"
#include "../../relocforests/include/settings.hpp"
#include "../../relocforests/include/forest.hpp"

using namespace ITMLib::Objects;
using namespace ISUE::RelocForests;

namespace ITMLib
{
  namespace Engine
  {
    class ITMForestRelocalizer : public ITMRelocalizer
    {
    private:
      Data *data;
      Settings *settings;
      Forest<float, cv::Vec4b> *forest;
    public:
      ITMForestRelocalizer(const ITMRGBDCalib *calib, Vector2i dims);
      ITMForestRelocalizer(const ITMRGBDCalib *calib, Vector2i dims, const std::string file_name);

      void RelocalizeCamera(ITMTrackingState *trackingState, const ITMView *view);
      void CollectFrame(ITMTrackingState *trackingState, const ITMView *view);
      void Train();
      void Test(int frame);
      void SaveToFile(const string name);
    };
  }
}