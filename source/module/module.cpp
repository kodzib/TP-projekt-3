#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <stdio.h>
#include <matplot/matplot.h>
#include <cmath>
#include <string>


namespace py = pybind11;

void say_hello(int x) {
	printf("Hello World!\n");
	std::cout << x << std::endl;
}

void plot_audio(py::array_t<float> dane_to_plot) {
	py::buffer_info array_buf = dane_to_plot.request();
	float* ptr = static_cast<float*>(array_buf.ptr);
	int size = array_buf.shape[0];
	matplot::fplot([](double x) {return sin(x);}, std::array<double, 2>{-10, 10}, "x--b")->color("red").line_width(2);
	matplot::save("C:/Users/kbrow/OneDrive/Desktop/projekty/TP projekt 3/TP-projekt-3/test_graph.jpg");
}

py::array_t<double> signal_generator(char type, double stepping, const py::ssize_t size) {
	std::vector<double> lista(size);
	double signal_counter = 0;
	double triangle_count = 0;
	switch (type) {
	case 's': case 'S': //sin
		for (int i = 0; i < size; i++) {
			lista[i] = sin(signal_counter);
			signal_counter += stepping;
		}
		return py::array(size, lista.data());
		break;
	case 'c': case 'C': //cos
		for (int i = 0; i < size; i++) {
			lista[i] = cos(signal_counter);
			signal_counter += stepping;
		}
		return py::array(size, lista.data());
		break;
	case 'p': case 'P': //prostokatny
		for (int i = 0; i < size; i++) {
			if (sin(signal_counter) >= 0) lista[i] = 1;
			else lista[i] = -1;
			signal_counter += stepping;
		}
		return py::array(size, lista.data());
		break;
	case 't': case 'T': //piloksztaltny
		for (int i = 0; i < size; i++) {
			lista[i] = triangle_count;
			triangle_count += stepping;
			if (triangle_count > 1) triangle_count = 0;
		}
		return py::array(size, lista.data());
		break;
	default:
		for (int i = 0; i < size; i++) {
			lista[i] = 0;
		}
		return py::array(size, lista.data());
		break;
	}
}

PYBIND11_MODULE(pybind11module, module) {
	module.doc() = "Pybind11Module";

	module.def("say_hello", &say_hello);
	module.def("plot_audio", &plot_audio);
	module.def("signal_generator", &signal_generator, "Generator sygnalow, jako typ przekazac pierwsza litere zachcianego typu");
}