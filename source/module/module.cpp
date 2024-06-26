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

void say_hello(int x) { //funkcja do testowania czy biblioteka dziala
	printf("Hello World!\n");
	std::cout << x << std::endl;
}

void plot_audio(py::array_t<float> buf, std::string path_to_save) {
	float* ptr = static_cast<float*>(buf.request().ptr);
	size_t size = buf.size();
	std::vector<float> dane_to_plot(ptr, ptr + size);
	matplot::plot(dane_to_plot);
	matplot::save(path_to_save);
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

py::array_t<double> filtracja_d(py::array_t<double> buf, const char type, const int kernel_size) { //filtracja jedno wymiarowa

	py::buffer_info signal = buf.request();
	double* ptr = static_cast<double*>(signal.ptr);
	unsigned int size = signal.shape[0];

	py::array_t<double> filtered_signal({size});
	py::buffer_info filtered_buf = filtered_signal.request();
	double* filtered_ptr = (double*)filtered_buf.ptr;

	std::vector<double> kernel(kernel_size);
	double sum = 0.;
	switch (type) {
	case 'g': case 'G':
		for (int i = 0; i < kernel_size; i++) {
			double sigma = 1;
			kernel[i] = (1 / sqrt(2 * M_PI * pow(sigma, 2))) * exp(-1 * pow(i, 2) / (2 * pow(sigma, 2)));
		}
		break;
	default:
		for (int i = 0; i < kernel_size; i++) {
			kernel[i] = 1;
		}
		break;
	}

	for (int i = 0; i < kernel_size; i++) {
		if (i == 0) {
			sum += kernel[i];
		}
		else sum += 2 * kernel[i];
	}

	for (int i = 0; i < kernel_size; i++) {
		kernel[i] /= sum;
	}

	for (int i = 0; i < size; i++) {
		double new_kernl_ammount = 0.;
		for (int k = -1 * (kernel_size - 1); k < kernel_size - 1; k++) {
			if (i + k >= 0 && i + k < size) {
				new_kernl_ammount += buf.at(i + k) * kernel[abs(k)];
			}
		}
		filtered_ptr[i] = new_kernl_ammount;
	}
	return filtered_signal; 
}

py::array_t<UINT8> filtracja_img(py::array_t<UINT8> image,const int kernel_size, const char type) //filtracja 2D
{
	double sum = 0.;
	std::vector<std::vector<double>> kernel(kernel_size, std::vector<double>(kernel_size));

	switch (type) {
	case 'g': case 'G':
		//filtr gauusa
		for (int i = -kernel_size / 2; i <= kernel_size / 2; ++i) {
			for (int j = -kernel_size / 2; j <= kernel_size / 2; ++j) {
				const int sigma = 5;
				if (i + kernel_size / 2 < kernel_size && j + kernel_size / 2 < kernel_size) {
					kernel[i + kernel_size / 2][j + kernel_size / 2] = (1 / (2 * M_PI * pow(sigma, 2))) * exp(-1 * ((pow(i, 2) + pow(j, 2)) / (2 * pow(sigma, 2))));
					sum += kernel[i + kernel_size / 2][j + kernel_size / 2];
				}
			}
		}

		for (int k = 0; k < kernel_size; k++) {
			for (int l = 0; l < kernel_size; l++) {
				kernel[k][l] /= sum;
			}
		}
		break;
	case 'r': case 'R': case'b': case'B': case'z': case'Z':
		//kasacja R/G (z) /B
		for (int i = -kernel_size / 2; i <= kernel_size / 2; ++i) {
			for (int j = -kernel_size / 2; j <= kernel_size / 2; ++j) {
				if (i + kernel_size / 2 < kernel_size && j + kernel_size / 2 < kernel_size) {
					kernel[i + kernel_size / 2][j + kernel_size / 2] = 0;
				}
			}
		}

		break;
	default:
		//rgb--> black and white
		for (int i = -kernel_size / 2; i <= kernel_size / 2; ++i) {
			for (int j = -kernel_size / 2; j <= kernel_size / 2; ++j) {
				if (i + kernel_size / 2 < kernel_size && j + kernel_size / 2 < kernel_size) {
					kernel[i + kernel_size / 2][j + kernel_size / 2] = (double)1/3;
				}
			}
		}

		break;
	}


	py::buffer_info buf = image.request();
	UINT8* ptr = (UINT8*)buf.ptr;


	unsigned int height = buf.shape[0];
	unsigned int width = buf.shape[1];
	unsigned int rgb = buf.shape[2];

	py::array_t<UINT8> filtered_image({ height, width, rgb });
	py::buffer_info filtered_buf = filtered_image.request();
	UINT8* filtered_ptr = (UINT8*)filtered_buf.ptr;

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			for (int k = 0; k < rgb; ++k) {
				float pixel = 0;
				switch (type)
				{
				case 'g': case 'G':
					for (int m = 0; m < kernel_size; ++m) {
						for (int n = 0; n < kernel_size; ++n) {
							int x = i + m - kernel_size / 2;
							int y = j + n - kernel_size / 2;
							if (x >= 0 && x < height && y >= 0 && y < width) {
								pixel += kernel[m][n] * image.at(x, y, k);
							}
						}
					}
					break;

				case 'r': case 'R':
					if (k != 0) {
						pixel = kernel[0][0] * image.at(i, j, k);
					}
					else pixel = image.at(i, j, k);
					break;

				case 'z': case 'Z':
					if (k != 1) {
						pixel = kernel[0][0] * image.at(i, j, k);
					}
					else pixel = image.at(i, j, k);
					break;

				case 'b': case 'B':
					if (k != 2) {
						pixel = kernel[0][0] * image.at(i, j, k);
					}
					else pixel = image.at(i, j, k);
					break;

				default:
					pixel = kernel[0][0] *( image.at(i, j, 0)+  image.at(i, j, 1)+  image.at(i, j, 2));
					break;
				}

				filtered_ptr[i * width * rgb + j * rgb + k] = (UINT8)pixel;
							
			}
		}
	}
	
	return filtered_image;
}


PYBIND11_MODULE(Pybind11module, module) {
	module.doc() = "Pybind11Module";

	module.def("say_hello", &say_hello);
	module.def("plot_audio", &plot_audio);
	module.def("signal_generator", &signal_generator, "Generator sygnalow, jako typ przekazac pierwsza litere zachcianego typu");
	module.def("filtracja_d", &filtracja_d);
	module.def("filtracja_img", &filtracja_img);
}
