/*
 * estimation_error_exception.h
 *
 *  Created on: 31 ago 2016
 *      Author: Nicola Pellicanò
 */

#ifndef SRC_EXCEPTIONS_ESTIMATION_ERROR_EXCEPTION_H_
#define SRC_EXCEPTIONS_ESTIMATION_ERROR_EXCEPTION_H_

#include <iostream>
#include <exception>
#include <stdexcept>
#include <sstream>

/**
 * @class EstimationErrorException
 * Exception calss for errors in the model estimation process.
 */

class EstimationErrorException : public std::runtime_error{
public:
	EstimationErrorException():std::runtime_error("Unexpected error in the estimation procedure"){};
	virtual ~EstimationErrorException(){};
};

#endif /* SRC_EXCEPTIONS_ESTIMATION_ERROR_EXCEPTION_H_ */
