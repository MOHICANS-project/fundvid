# fundvid
**Fundamental matrix estimation from synchronized video stream**

## Authors
Nicola Pellicanò (nicola.pellicano@u-psud.fr) 

Emanuel Aldea (emanuel.aldea@u-psud.fr)

Sylvie Le Hégarat-Mascle (sylvie.le-hegarat@u-psud.fr)

SATIE - CNRS UMR 8029

Paris-Sud University, Paris-Saclay University, France


## Overview
The project implements a demo version of the algorithm for fundamental matrix estimation presented in the paper 

*Robust Wide Baseline Pose Estimation from Video*, Nicola Pellicanò, Emanuel Aldea, Sylvie Le Hégarat-Mascle, accepted for presentation at ICPR 2016.

(We kindly invite you to cite this paper if you find this software useful)

The algorithm performs the estimation of the fundamental matrix of a wide baseline stereo pair by processing frames extracted from synchronized videos, addressing the limitations of single image pairs estimations and naive temporal accumulation of matches.

This work is part of the project "MOHICANS : Towards Modelling High-density Crowds for Assisting Planning and Safety". For more information about this project see [here](http://hebergement.u-psud.fr/emi/MOHICANS/index.html).


## Dependencies and build
The library has four dependencies from third party software:
  * [OpenCV](http://opencv.org) (Computer vision package)
  * [gvars] (https://github.com/edrosten/gvars) (Configuration library)
  * [levmar](http://users.ics.forth.gr/~lourakis/levmar/) (Library for Levenberg-Marquardt non linear optimization) 
  * [eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page) (Linear algebra library)
  * [Boost](http://http://www.boost.org/) (For filesystem support)
  

The project includes a modified version of the Lionel Moisan's ORSA estimation framework. For more information see [here] (http://www.ipol.im/pub/art/2016/147/).

**Important**: *gcc 4.9* or higher is needed for building. 


## How to use the demo
An example configuration file is provided in the [example](src/example) folder, and includes all the parameters and the input data accepted by the program. The description of the meaning of each entry is already included in the configuration file. 

A small dataset is included as an example of the usage of the library. You can evaluate the solution provided by using the ground truth matches present inside an .xml file in the same folder.  For the full dataset containing the entire video sequence you can download it from the [project website](http://hebergement.u-psud.fr/emi/MOHICANS/index.html). 

```
#Program launch parameters
./estimate_fundamental [path-config-file] [output-directory]
#Example
./estimate_fundamental example/default_config.cfg results
```
The program will write three files in the *[output-directory]* :
1. *f.txt* : the  estimated 3x3 fundamental matrix
2. *inliers0.txt*: inlier points from first image
3. *inliers1.txt*: inlier points from second image

## Licensing
Please refer to the accompanying [LICENSE](LICENSE) file for more information.

## Bug reports
For any questions and bug report please contact nicola.pellicano@u-psud.fr.

## Notes on the current version
* The code at the actual state accepts frames as a sequence of images. The sequence for each camera is stored inside a separate   folder, and image frames at same instant have the same name (ex.cam0/img65.png and cam1/img65.png). An interface is included   to make simple the extension to new forms of input gathering in the future.
* ORSA estimator implementation is the default, but extensibility to other estimators (like RANSAC) is provided.
* Interfaces for adding different non linear optimizers and different matchers (used at initialization stage) are provided.

