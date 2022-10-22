
#include "first_app.hpp"
#include "VoxelParser/VoxelSampleHelper.h"
#include "VoxelParser/VoxelSampleUtils.h"

// std
#include <cstdlib>
#include <iostream>
#include <stdexcept>


int main() {

	if constexpr (false)
	{
		generateSample("../models/sample.voxm", ripple, phantomAlways, { 600, 300, 600 });
	}
	else
	{

		lve::FirstApp app{};

		try {
			app.run();
		}
		catch (const std::exception& e) {
			std::cerr << e.what() << '\n';
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}
