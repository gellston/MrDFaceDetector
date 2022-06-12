#pragma warning(disable : 4996)

#include "faceDetector.h"

#include <torch/torch.h>
#include <torch/script.h>


#include <opencv2/opencv.hpp>


namespace mrd {
	class impl_faceDetector {
	public:

		std::string path;
		double iou_threshold;
		double score_threshold;
        double feature_width;
        double feature_height;
        double inference_width;
        double inference_height;
		torch::jit::script::Module model;
        torch::DeviceType torch_device_type;

		impl_faceDetector() {
            inference_height = 640;
            inference_width = 640;
            feature_width = 160;
            feature_height = 160;

            iou_threshold = 0;
            score_threshold = 0;

            torch_device_type = torch::kCPU;
		}
	};
}



double boxiou(std::shared_ptr<mrd::box> box1, std::shared_ptr<mrd::box> box2) {
    
    double x1 = std::max(box1->startX(), box2->startX());
    double y1 = std::max(box1->startY(), box2->startY());
    double x2 = std::min(box1->endX(), box2->endX());
    double y2 = std::min(box1->endY(), box2->endY());

    double interArea = std::max(0.0, x2 - x1 + 1) * std::max(0.0, y2 - y1 + 1);

    double bboxAArea = box1->width() * box1->height();
    double bboxBArea = box2->width() * box2->height();


    double iou = interArea / (bboxAArea + bboxBArea - interArea);

    return iou;
}




mrd::faceDetector::faceDetector(std::string path, double inference_width, double inference_height, double downscale, double iou_threshold, double score_threshold) : _instance(new mrd::impl_faceDetector()) {

	this->_instance->path = path;
	this->_instance->iou_threshold = iou_threshold;
	this->_instance->score_threshold = score_threshold;
    this->_instance->inference_width = inference_width;
    this->_instance->inference_height = inference_height;
    this->_instance->feature_width = inference_width / downscale;
    this->_instance->feature_height = inference_height / downscale;




    //Device selection
    
    if (torch::cuda::is_available())
    {
        this->_instance->torch_device_type = torch::kCUDA;
    }
    else
    {
        this->_instance->torch_device_type = torch::kCPU;
    }


	try {
        
		this->_instance->model = torch::jit::load(path, this->_instance->torch_device_type);
		this->_instance->model.eval();

	}
	catch (std::exception e) {
        std::cout << e.what() << std::endl;
		throw std::exception("Invalid model info");
	}
}
mrd::faceDetector::~faceDetector() {

}


std::vector<std::shared_ptr<mrd::box>> mrd::faceDetector::prediction(unsigned char* buffer, int width, int height, int channel) {



    try {


        if (channel != 3 || width <= 0 || height <= 0)
            throw std::exception("Invalid image info");

        cv::Mat original(cv::Size(width, height), CV_8UC3, buffer, width * 3);

        cv::Mat resizeImage;

        cv::resize(original, resizeImage, cv::Size(this->_instance->inference_width, this->_instance->inference_height));






        std::vector<torch::jit::IValue> tensor_vec;

        torch::Tensor tensor_image = torch::from_blob(resizeImage.data, { 1, resizeImage.rows, resizeImage.cols, 3 }, torch::kByte).to(torch::kFloat32).to(this->_instance->torch_device_type);
        tensor_image = tensor_image.permute({ 0, 3, 1, 2 }); //   # (B, H, W, CH) -> (B, CH, H, W)
        tensor_image = tensor_image.contiguous();
        tensor_vec.push_back(tensor_image);

        auto outputs = this->_instance->model.forward(tensor_vec).toTuple();
        auto elements = outputs->elements();

        torch::Tensor prediction_heatmap = elements[0].toTensor().cpu().detach();
        torch::Tensor prediction_featuremap = elements[1].toTensor().cpu().detach();
        torch::Tensor prediction_sizemap = elements[2].toTensor().cpu().detach();
        torch::Tensor prediction_offsetmap = elements[3].toTensor().cpu().detach();



        std::vector<std::shared_ptr<mrd::box>> total_box;
        std::vector<std::shared_ptr<mrd::box>> final_box;
        for (int x = 0; x < this->_instance->feature_width; x++) {
            for (int y = 0; y < this->_instance->feature_height; y++) {
                double score = prediction_heatmap[0][0][y][x].item().toDouble();
                if (score > this->_instance->score_threshold) {

                    double offset_x = prediction_offsetmap[0][0][y][x].item().toDouble();
                    double offset_y = prediction_offsetmap[0][1][y][x].item().toDouble();


                    double size_width = prediction_sizemap[0][0][y][x].item().toDouble() / this->_instance->feature_width * width;
                    double size_height = prediction_sizemap[0][1][y][x].item().toDouble() / this->_instance->feature_height * height;


                    double box_x = ((x + offset_x) / this->_instance->feature_width * width) - size_width / 2;
                    double box_y = ((y + offset_y) / this->_instance->feature_height * height) - size_height / 2;


                    std::shared_ptr<mrd::box> box(new mrd::box(score, box_x, box_y, size_width, size_height));
                    total_box.push_back(box);
                }
            }
        }

        if (total_box.size() == 0)
            return final_box;

        std::sort(total_box.begin(), total_box.end(), [&](std::shared_ptr<mrd::box>& box1, std::shared_ptr<mrd::box>& box2) {
            return box1->score() > box2->score();
        });


        for (int index = 0; index < total_box.size() - 1; index++) {
            auto currentBox = total_box[index];
            if (currentBox->score() == 0) continue;
            for (int next = index + 1; next < total_box.size(); next++) {
                auto nextBox = total_box[next];

                if (nextBox->score() == 0) continue;
                if (boxiou(currentBox, nextBox) >= this->_instance->iou_threshold)
                    nextBox->score(0);
            }
        }

        for (auto box : total_box)
            if (box->score() > 0)
                final_box.push_back(box);



        return final_box;

    }
    catch (std::exception e) {
        throw e;
    }

}