/*
 * abstract_matcher.h
 *
 *  Created on: 29 ago 2016
 *      Author: Nicola Pellicanò
 */

#ifndef SRC_MATCHERS_ABSTRACT_MATCHER_H_
#define SRC_MATCHERS_ABSTRACT_MATCHER_H_

#include <utility>
#include <vector>

/**
 * @namespace FVMatcher
 */

namespace FVMatcher{


	struct FVMatch{
		std::pair<double,double> p0;
		std::pair<double,double> p1;
		double dist;
	};

	/**
	 * @class AbstractMatcher
	 *
	 * Interface for getting keypoints matches from two sources.
	 *
	 */
	class AbstractMatcher {

		public:
			AbstractMatcher(){};
			virtual ~AbstractMatcher(){};

			/**
			 * Compute matches from the current sources.
			 * @return vector of matches
			 */
			virtual std::vector<FVMatcher::FVMatch> computeMatches()=0;
	};
}
#endif /* SRC_MATCHERS_ABSTRACT_MATCHER_H_ */
