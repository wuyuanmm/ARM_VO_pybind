#pragma once


#include "common.hpp"

class Viewer
{
public:
	Viewer() {};
	~Viewer() {};

   // void show(const cv::Mat &frame, const std::vector<cv::Point2f>& prev_keypoints,
    //          const std::vector<cv::Point2f>& curr_keypoints, const int FPS, const cv::Mat& translation);

	cv::Mat show(cv::Mat &frame, py::array_t<float, py::array::c_style | py::array::forcecast> prev_keypoints,
		py::array_t<float, py::array::c_style | py::array::forcecast> curr_keypoints, int FPS, cv::Mat& translation);

	cv::Mat frameWithFeatures;
private:
    cv::Mat draw_tracked_kpts(const cv::Mat &frame, const std::vector<cv::Point2f> &prev_keypoints,
                              const std::vector<cv::Point2f> &curr_keypoints);
    cv::Mat draw_trajectory(void);
    cv::Point2i convertToImageCoordinates(const cv::Point2f pointInMeters);

    float scale = 1.0f;
    float min_x = -0.5f;
    float min_y = -0.5f;
    float max_x = 0.5f;
    float max_y = 0.5f;
    const float frameScale = 5.0f;
    float imageSize = 500.0f;

    std::vector<cv::Point2f> path;
};
