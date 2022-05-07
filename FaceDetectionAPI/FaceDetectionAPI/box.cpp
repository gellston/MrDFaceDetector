#include "box.h"


namespace mrd {
	class impl_box {
	public:
		double _x;
		double _y;
		double _width;
		double _height;
		double _centerX;
		double _centerY;
		double _start_x;
		double _start_y;
		double _end_x;
		double _end_y;
		double _score;

		impl_box() {
			this->_x = 0;
			this->_y = 0;
			this->_width = 0;
			this->_height = 0;
			this->_centerX = 0;
			this->_centerY = 0;
			this->_start_x = 0;
			this->_start_y = 0;
			this->_end_x = 0;
			this->_end_y = 0;
			this->_score = 0;
		}

	};
}

mrd::box::box(double score, double x, double y, double width, double height) : _instance(new mrd::impl_box()) {

	this->_instance->_x = x;
	this->_instance->_y = y;
	this->_instance->_width = width;
	this->_instance->_height = height;


	this->_instance->_centerX = this->_instance->_x + this->_instance->_width / 2;
	this->_instance->_centerY = this->_instance->_y + this->_instance->_height / 2;

	this->_instance->_start_x = this->_instance->_x;
	this->_instance->_start_y = this->_instance->_y;
	this->_instance->_end_x = this->_instance->_x + this->_instance->_width;
	this->_instance->_end_y = this->_instance->_y + this->_instance->_height;

	this->_instance->_score = score;
}


mrd::box::~box() {

}

double mrd::box::x() {
	return this->_instance->_x;
}
double mrd::box::y() {
	return this->_instance->_y;
}
double mrd::box::width() {
	return this->_instance->_width;
}
double mrd::box::height() {
	return this->_instance->_height;
}
double mrd::box::centerX() {
	return this->_instance->_centerX;
}
double mrd::box::centerY() {
	return this->_instance->_centerY;
}
double mrd::box::startX() {
	return this->_instance->_start_x;
}
double mrd::box::startY() {
	return this->_instance->_start_y;
}
double mrd::box::endX() {
	return this->_instance->_end_x;
}
double mrd::box::endY() {
	return this->_instance->_end_y;
}

double mrd::box::score() {
	return this->_instance->_score;
}

void mrd::box::score(double _score) {
	this->_instance->_score = _score;
}