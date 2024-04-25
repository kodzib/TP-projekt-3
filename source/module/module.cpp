#include <pybind11/pybind11.h>
#include <stdio.h>

void say_hello() {
	printf("Hello World!\n");
}

PYBIND11_MODULE(pybind11module, module) {
	module.doc() = "Pybind11Module";

	module.def("say_hello", &say_hello);
}