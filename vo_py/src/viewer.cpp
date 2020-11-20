#include "viewer.hpp"


// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------
//void Viewer::show(const cv::Mat &frame, const std::vector<cv::Point2f>& prev_keypoints,
//                  const std::vector<cv::Point2f>& curr_keypoints, const int FPS, const cv::Mat& translation)
//{
//    cv::Mat frameWithFeatures;
//    if (curr_keypoints.size() > 0)
//        frameWithFeatures = draw_tracked_kpts(frame, prev_keypoints, curr_keypoints);
//    else
//        cv::cvtColor(frame, frameWithFeatures, 8);
//
//    cv::resize(frameWithFeatures, frameWithFeatures, cv::Size(), 0.5, 0.5);
//
//    char text[20];
//    sprintf(text, "FPS: %d", FPS);
//    putText(frameWithFeatures, text, cv::Point(8,30), 0, 1.0, cv::Scalar(0,0,255), 3);
//
//    cv::imshow("Frames", frameWithFeatures);
//
//
//    float x =  translation.at<float>(0);
//    float y = -translation.at<float>(2);
//    path.push_back(cv::Point2f(x,y));
//
//    cv::Mat trajectory = draw_trajectory();
//    sprintf(text, "X[m]: %.2f", path.back().x);
//    putText(trajectory, text, cv::Point(350,20), 0, 0.6, cv::Scalar::all(255), 2);
//
//    sprintf(text, "Y[m]: %.2f", path.back().y);
//    putText(trajectory, text, cv::Point(350,50), 0, 0.6, cv::Scalar::all(255), 2);
//
//    cv::imshow("Trajectory", trajectory);
//    cv::waitKey(1);
//}


cv::Mat Viewer::show( cv::Mat &frame, py::array_t<float, py::array::c_style | py::array::forcecast> prev_keypoints,
	py::array_t<float, py::array::c_style | py::array::forcecast> curr_keypoints,  int FPS, cv::Mat& translation)
{
	if (prev_keypoints.ndim() != 2)
		throw std::runtime_error("Input should be 2-D NumPy array");


	std::vector<float> pos_prev(prev_keypoints.size()), pos_curr(curr_keypoints.size());
	std::vector<cv::Point2f> pos_prev_pts, pos_curr_pts;
	std::memcpy(pos_prev.data(), prev_keypoints.data(), prev_keypoints.size() * sizeof(float));
	std::memcpy(pos_curr.data(), curr_keypoints.data(), curr_keypoints.size() * sizeof(float));

	int rows_N = prev_keypoints.shape()[0];

	
	for (int i = 0; i < rows_N; i++)
	{

		pos_prev_pts.push_back(cv::Point2f(pos_prev[i * 2], pos_prev[i * 2+1]));
	}
	rows_N = curr_keypoints.shape()[0];


	for (int i = 0; i < rows_N; i++)
	{

		pos_curr_pts.push_back(cv::Point2f(pos_curr[i * 2], pos_curr[i * 2 + 1]));
	}

	//cv::Mat frameWithFeatures;
	if (pos_curr_pts.size() > 0)
		frameWithFeatures = draw_tracked_kpts(frame, pos_prev_pts, pos_curr_pts);
	else
		cv::cvtColor(frame, frameWithFeatures, 8);

	cv::resize(frameWithFeatures, frameWithFeatures, cv::Size(), 0.5, 0.5);

	char text[20];
	sprintf(text, "FPS: %d", FPS);
	putText(frameWithFeatures, text, cv::Point(8, 30), 0, 1.0, cv::Scalar(0, 0, 255), 3);

	//cv::imshow("Frames", frameWithFeatures);


	float x = translation.at<float>(0);
	float y = -translation.at<float>(2);
	path.push_back(cv::Point2f(x, y));

	cv::Mat trajectory = draw_trajectory();
	sprintf(text, "X[m]: %.2f", path.back().x);
	putText(trajectory, text, cv::Point(350, 20), 0, 0.6, cv::Scalar::all(255), 2);

	sprintf(text, "Y[m]: %.2f", path.back().y);
	putText(trajectory, text, cv::Point(350, 50), 0, 0.6, cv::Scalar::all(255), 2);

	//cv::imshow("Trajectory", trajectory);
	//cv::waitKey(1);

	return trajectory;
}



// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------
cv::Mat Viewer::draw_tracked_kpts(const cv::Mat &frame, const std::vector<cv::Point2f> &prev_keypoints,
                                  const std::vector<cv::Point2f> &curr_keypoints)
{
    cv::Mat frameWithFeatures;
    cv::cvtColor(frame, frameWithFeatures, 8);

    for (size_t i=0; i<curr_keypoints.size(); i++)
    {
        cv::line(frameWithFeatures, prev_keypoints[i], curr_keypoints[i], cv::Scalar(255,0,0), 2);
        cv::circle(frameWithFeatures, curr_keypoints[i], 3, CV_RGB(0,255,0), -1);
    }

    return frameWithFeatures;
}


// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------
cv::Point2i Viewer::convertToImageCoordinates(const cv::Point2f pointInMeters)
{
    cv::Point2f pt = (pointInMeters - cv::Point2f(min_x, min_y)) * scale;
    return cv::Point2i(pt.x, imageSize - pt.y); // reverse y for more intuitive top-down plot
}


// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------
cv::Mat Viewer::draw_trajectory(void)
{
    float x = path.back().x;
    float y = path.back().y;

    if (x - frameScale < min_x)
        min_x = x - frameScale;
    if (y - frameScale < min_y)
        min_y = y - frameScale;
    if (x + frameScale > max_x)
        max_x = x + frameScale;
    if (y + frameScale > max_y)
        max_y = y + frameScale;

    cv::Mat traj(imageSize, imageSize, CV_8UC3, cv::Scalar::all(0));
    scale = std::min(imageSize / (max_x - min_x), imageSize / (max_y - min_y));

    for (size_t i = 0; i + 1 < path.size(); )
    {
        cv::Point2i p0 = convertToImageCoordinates(path[i]);
        cv::Point2i p1 = convertToImageCoordinates(path[i + 1]);

        cv::line(traj,p0,p1,cv::Scalar(0,255,0),3);

        i++;
    }

    return traj;
}
