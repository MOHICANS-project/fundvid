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


int BoostReader::getImageIndex(std::string imagename)
{
    std::smatch match;
    std::regex_search(imagename, match, std::regex(R"([A-Za-z]+[\-_]*+(\d+)[.]{1}\w+)"));
    std::string idx=match[1].str();
    //frame_index=stoi(idx);
    return stoi(idx);
}


BoostReader::BoostReader(std::string _folder, std::string _extension, std::string _base, std::string _first_imgname,int _dt):folder(_folder),extension(_extension),base(_base),dt(_dt){
    firstImageIndex=getImageIndex(_first_imgname);
    frame_index=firstImageIndex-dt; //first dt increment will lead to start at firstImageIndex
    image_index=-1; //last processed frame position in the folder
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

                for(auto & name : imageNames) {
#ifdef ENABLE_DEBUG
                    std::cout << name << " " << getImageIndex(name) << std::endl;
#endif
                   frameIndices.push_back(getImageIndex(name));
                }


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
	
    frame_index +=dt;
    image_index++; //start from the image after
    if(frame_index < frameIndices[frameIndices.size()-1]){
        int pos=-1;
        for (size_t i = image_index; i < frameIndices.size(); ++i) {
            if(frameIndices[i]>=frame_index){
                //if greater a jump occurred: select the next image
                pos=i;
                frame_index=frameIndices[i];
                break;
            }
        }
        image_index=pos;
		std::string imgpath=folder+"/"+ imageNames[image_index];
		out=cv::imread(imgpath,0);
		return true;
	}
	
	return false;

}
