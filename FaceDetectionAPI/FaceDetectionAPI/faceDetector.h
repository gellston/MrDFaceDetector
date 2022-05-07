#pragma once

#ifndef MRD_DETECTOR
#define MRD_DETECTOR


#include <memory>
#include <vector>
#include <string>

#include "box.h"
#include "macro.h"

namespace mrd {

	class impl_faceDetector;

	class MRD_API faceDetector {
	private:
		std::shared_ptr<mrd::impl_faceDetector> _instance;
	public:

		faceDetector(std::string path, double iou_threshold, double score_threshold);
		~faceDetector();


		std::vector<std::shared_ptr<mrd::box>> prediction(unsigned char* buffer, int width, int height, int channel);


	};
}

#endif