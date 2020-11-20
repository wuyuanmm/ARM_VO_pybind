#include"./include/ARM_VO.hpp"


PYBIND11_MODULE(ARMVO, m)
{
	NDArrayConverter::init_numpy();

	m.doc() = "ARM_VO plugin";
	
	py::class_<ARM_VO>(m, "ARM_VO")
		.def(py::init<const std::string &>())
		.def("loadSetting", &ARM_VO::loadSetting)
		.def("init", &ARM_VO::init)
		.def("update", &ARM_VO::update)
		.def("get_prev_inliers", &ARM_VO::get_prev_inliers)
		.def("get_curr_inliers", &ARM_VO::get_curr_inliers)

		.def_readwrite("R_f", &ARM_VO::R_f)
		.def_readwrite("t_f", &ARM_VO::t_f)
		.def_readwrite("initialized", &ARM_VO::initialized);

	py::class_<Viewer>(m, "Viewer")
		.def(py::init<>())
		.def("show", &Viewer::show)
		.def_readwrite("frameWithFeatures", &Viewer::frameWithFeatures);
}