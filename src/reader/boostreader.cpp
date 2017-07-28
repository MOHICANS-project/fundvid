/*
 * boostreader.cpp
 *
 *  Created on: 26 ago 2016
 *      Author: Nicola Pellicanò
 */

#include "boostreader.h"
#include <boost/algorithm/string/predicate.hpp>
#include <regex>

using namespace boost::filesystem;




BoostReader::BoostReader(std::string _folder, std::string _extension, std::string _base):folder(_folder),extension(_extension),base(_base){
	image_index=-1;
	
	//extension must have a . in order to mean the same thing as boost .extension() result
	if(extension[0] != '.')
		extension = std::string(".")+extension;

	//populate the filename vector
	path p(folder);
	try{
		if(exists(p)){
			if(is_directory(p)){

				std::vector<path> allNames; 
				copy(directory_iterator(p), directory_iterator(), back_inserter(allNames));

				for (std::vector<path>::const_iterator it (allNames.begin()); it != allNames.end(); ++it){
					if(it->has_extension()){
						if (!extension.compare(it->extension().c_str()) && boost::starts_with(it->filename().c_str(),base.c_str())){
							//they are the same
							imageNames.push_back(it->filename().c_str());
						}
					}
				}

				std::sort(imageNames.begin(), imageNames.end(), doj::alphanum_less<std::string>());

			}
			else
				std::cout << p << "is not a folder. Check  your config file and your data folder." << std::endl;
		}
		else
			std::cout << p << "not found. Check  your config file and your data folder." << std::endl;

	}
	catch(const filesystem_error& err){
		std::cerr << err.what() << std::endl;
	}

}

bool BoostReader::getNextFrame(cv::Mat & out){
	
	image_index ++;

    if(image_index < (int)imageNames.size()){
		std::string imgpath=folder+"/"+ imageNames[image_index];
		//std::cout << "Reading " << imgpath << std::endl;
		std::smatch match;
		std::regex_search(imageNames[image_index],match,std::regex("[A-Za-z]+(\\d+)[.]{1}\\w+"));
		std::string idx=match[1].str();
		frame_index=stoi(idx);
		out=cv::imread(imgpath,0);
		return true;
	}
	
	return false;

}
