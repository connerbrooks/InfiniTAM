#pragma once

#include "../Utils/ITMLibDefines.h"

#include "../Objects/ITMTrackingState.h"
#include "../Objects/ITMView.h"

using namespace ITMLib::Objects;

namespace ITMLib
{
  namespace Engine
  {
    /** \breif
        Basic interface to any relocalizer.
    */
    class ITMRelocalizer
    {
    public:
      /** Relocalize a View in the given scene. The result
          is stored in the tracking state.
      */
      virtual void RelocalizeCamera(ITMTrackingState *trackingState, const ITMView *view) = 0;

      /** Allows relocalizer to collect an rgb-d frame and a pose
          during train time. 
      */
      virtual void CollectFrame(ITMTrackingState *trackingState, const ITMView *view) = 0;

      /** Trains relocalizer. Required if training is not real time.
          Not pure virtual, as you may not need this.
      */
      virtual void Train() {};
      virtual void Test(int frame) {};

      /** Save relocalizer data to file.
      */
      virtual void SaveToFile(const std::string name) = 0;

      virtual ~ITMRelocalizer(void) {}
    };
  }
}