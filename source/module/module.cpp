#define _USE_MATH_DEFINES
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <stdio.h>
#include <matplot/matplot.h>
#include <cmath>
#include <string>
#include <numbers>


namespace py = pybind11;

void say_hello(int x) {
	printf("Hello World!\n");
	std::cout << x << std::endl;
}

void plot_audio(py::array_t<float> buf, std::string path_to_save) {
	float* ptr = static_cast<float*>(buf.request().ptr);
	size_t size = buf.size();
	std::vector<float> dane_to_plot(ptr, ptr + size);
	matplot::plot(dane_to_plot);
	matplot::save(path_to_save);
	//matplot::show();
}

py::array_t<double> signal_generator(const char type, const double freq, const int samplerate, const py::ssize_t size) {
	std::vector<double> lista(size);
	double sampls_per_cycle = samplerate / freq;
	double stepping = 0;
	double signal_counter = 0;
	double triangle_count = 0;
	switch (type) {
	case 's': case 'S': //sin
		stepping = (2 * M_PI) / sampls_per_cycle;
		for (int i = 0; i < size; i++) {
			lista[i] = sin(signal_counter);
			signal_counter += stepping;
		}
		return py::array(size, lista.data());
		break;
	case 'c': case 'C': //cos
		stepping = (2 * M_PI) / sampls_per_cycle;
		for (int i = 0; i < size; i++) {
			lista[i] = cos(signal_counter);
			signal_counter += stepping;
		}
		return py::array(size, lista.data());
		break;
	case 'p': case 'P': //prostokatny
		stepping = (2 * M_PI) / sampls_per_cycle;
		for (int i = 0; i < size; i++) {
			if (sin(signal_counter) >= 0) lista[i] = 1;
			else lista[i] = -1;
			signal_counter += stepping;
		}
		return py::array(size, lista.data());
		break;
	case 't': case 'T': //piloksztaltny
		stepping = 1 / sampls_per_cycle;
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

py::array_t<double> filtracja_d(py::array_t<double> buf, const char type, const int kernel_size) {
	double* ptr = static_cast<double*>(buf.request().ptr);
	size_t size = buf.size();
	std::vector<double> dane(ptr, ptr + size);
	std::vector<double> kernel(kernel_size);
	double sum = 0.;
	switch (type) {
	case 'g': case 'G':
		for (int i = 0; i < kernel_size; i++) {
			double sigma = 1;
			kernel[i] = (1 / sqrt(2 * M_PI * pow(sigma, 2))) * exp(-1 * pow(i, 2) / (2 * pow(sigma, 2)));
			if (i == 0) {
				sum += kernel[i];
			}
			else sum += 2 * kernel[i];
		}
		break;
	default:
		for (int i = 0; i < kernel_size; i++) {
			kernel[i] = 1;
			if (i == 0) {
				sum += kernel[i];
			}
			else sum += 2 * kernel[i];
		}
		break;
	}
	for (int i = 0; i < kernel_size; i++) {
		kernel[i] /= sum;
		std::cout << kernel[i] << std::endl;
	}

	for (int i = 0; i < size; i++) {
		double new_kernl_ammount = 0.;
		for (int k = -1 * (kernel_size - 1); k < kernel_size - 1; k++) {
			if (i + k >= 0 && i + k < size) {
				new_kernl_ammount += dane[i] * kernel[abs(k)];
			}
		}
		dane[i] = new_kernl_ammount;
	}
	return py::array(size, dane.data());
}
//gauss dla 2d
py::array_t<double> filtracja_img(py::array_t<double> image, const int kernel_size)
{
	double sum = 0.;
	std::vector<std::vector<double>> kernel(kernel_size, std::vector<double> (kernel_size));
	for (int i = 0; i < kernel_size; i++) {
		for (int j = 0; j < kernel_size; j++) {
			double sigma = 1;
			kernel[i][j] = (1 /(2 * M_PI * pow(sigma, 2))) * exp(-1 * (pow(i, 2) + pow(j, 2)) / (2 * pow(sigma, 2)));
			sum += kernel[i][j];
		}
	}

	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			kernel[i][j] /= sum;
		}
	}


	for (int i = 0; i < kernel_size; ++i) {
		for (int j = 0; j < kernel_size; ++j){
			std::cout << kernel[i][j]<<" ";
			std::cout << std::endl;
		}
	}


	//kopia tablicy
	auto buf = image.request();
	int height = buf.shape[0];
	int width = buf.shape[1];
	int rgb = buf.shape[2];
	std::vector<std::vector<std::vector<double>>> kopia(height, std::vector<std::vector<double>>(width, std::vector<double>(rgb)));
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			for (int k = 0; k < rgb; ++k) {
				kopia[i][j][k] = image.at(i, j, k);
			}
		}
	}



	return py::array();
}


PYBIND11_MODULE(pybind11module, module) {
	module.doc() = "Pybind11Module";

	module.def("say_hello", &say_hello);
	module.def("plot_audio", &plot_audio);
	module.def("signal_generator", &signal_generator, "Generator sygnalow, jako typ przekazac pierwsza litere zachcianego typu");
	module.def("filtracja_d", &filtracja_d);
	module.def("filtracja_img", &filtracja_img);
}