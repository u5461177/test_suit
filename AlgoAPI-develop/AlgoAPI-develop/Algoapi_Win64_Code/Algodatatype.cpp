//
// Created by xtgu on 4/3/19.
//

#include "Algodatatype.h"
#include <string>

namespace ALGO {
	namespace API {
		using namespace std;

		string algoType_to_String(const AlgoType& algoType) {

			switch (algoType) {
			case AlgoType::TWAP:
				return "TWAP";
			case AlgoType::VWAP:
				return "VWAP";
			case AlgoType::IS:
				return "IS";
			case AlgoType::POV:
				return "POV";
			case AlgoType::RANDOM:
				return "RANDOM";
			case AlgoType::ICEBERG:
				return "ICEBERG";
			case AlgoType::MSTD:
				return "MSTD";
			case AlgoType::AIVWAP:
				return "AIVWAP";
			default:
				return "UNKNOWN";
			}

		}

	}
}