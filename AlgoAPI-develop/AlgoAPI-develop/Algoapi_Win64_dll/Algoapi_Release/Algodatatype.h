//
// Created by xtgu on 03/04/18.
//

#ifndef ALGOAPI_ALGODATATYPE_H
#define ALGOAPI_ALGODATATYPE_H

#include <string>
typedef enum MARKET {
	SH,
	SZ
} MARKET;

typedef enum SIDE {
	BUY,
	SELL
} SIDE;

namespace ALGO {
	namespace API {


		enum class AlgoType {
			TWAP,
			VWAP,
			IS,
			POV,
			RANDOM,
			ICEBERG,
			MSTD,
			AIVWAP
		};

		std::string algoType_to_String(const AlgoType& algoType);
	}
}
#endif //ALGOAPI_ALGODATATYPE_H
