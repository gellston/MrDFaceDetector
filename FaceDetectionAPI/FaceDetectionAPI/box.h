#pragma once

#ifndef MRD_BOX
#define MRD_BOX

#include <memory>

#include "macro.h"

namespace mrd {
	class impl_box;
	class MRD_API box {
	private:

		std::shared_ptr<impl_box> _instance;

	public:
		box(double score, double x, double y, double width, double height);
		~box();


		double x();
		double y();
		double width();
		double height();
		double centerX();
		double centerY();
		double startX();
		double startY();
		double endX();
		double endY();
		double score();
		void score(double _score);
	};

}

#endif