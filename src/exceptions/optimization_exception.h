/*
 * optimization_exception.h
 *
 *  Created on: 31 ago 2016
 *      Author: Nicola Pellicanò
 */

#ifndef SRC_EXCEPTIONS_OPTIMIZATION_EXCEPTION_H_
#define SRC_EXCEPTIONS_OPTIMIZATION_EXCEPTION_H_

#include <iostream>
#include <exception>
#include <stdexcept>
#include <sstream>

/**
 * @class OptimizationException
 * Exception class for errors in the model optimization process.
 */

class OptimizationException : public std::runtime_error{

public:
	/**
	 * OptimizationException constructor.
	 * @param _code the error code returned by the algorithm
	 */
	OptimizationException(double _code):
			std::runtime_error("Error in the optimization process."),code(_code){};

	virtual const char* what() const throw(){
		cnvt.str("");
		cnvt<< runtime_error::what() << " Algorithm exited with code "<< (int)code;
		return cnvt.str().c_str();
	}
private:
	double code;
	static std::ostringstream cnvt;
};

#endif /* SRC_EXCEPTIONS_OPTIMIZATION_EXCEPTION_H_ */

