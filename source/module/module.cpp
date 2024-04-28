#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <stdio.h>
#include <matplot/matplot.h>
#include <cmath>


namespace py = pybind11;

void say_hello() {
	printf("Hello World!\n");
}

void plot_audio(py::array_t<float> dane_to_plot) {
	py::buffer_info array_buf = dane_to_plot.request();
	float* ptr = static_cast<float*>(array_buf.ptr);
	int size = array_buf.shape[0];
	matplot::fplot([](double x) {return sin(x);}, std::array<double, 2>{-10, 10}, "x--b")->color("red").line_width(2);
	matplot::save("C:/Users/kbrow/OneDrive/Desktop/projekty/TP projekt 3/TP-projekt-3/test_graph.jpg");
}

PYBIND11_MODULE(pybind11module, module) {
	module.doc() = "Pybind11Module";

	module.def("say_hello", &say_hello);
	module.def("plot_audio", &plot_audio);
}