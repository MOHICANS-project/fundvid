# fundvid-demo
**Fundamental matrix estimation from syncrhonized video stream**

## Authors
.........

## Overview
The project implements a demo version of the algorithm for fundamental matrix estimation presented in the paper 
[link to draft](http:://google.com). 
The algorithms performs the estimation of the fundamental matrix of a wide baseline stereo pair by processing frames extracted from synchronized videos, addressing the limitations of single image pairs estimations and naive temporal accumulation of matches.

## Dependencies and build
The library has two dependencies from third party software:
  * [levmar](http://users.ics.forth.gr/~lourakis/levmar/) (Library for Levenberg-Marquardt non linear optimization) 
  * [eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page) (Linear algebra library)
The project includes a modified version of the Lionel Moisan's ORSA estimation framework. For more information see [here] (http://www.ipol.im/pub/art/2016/147/)
Build instructions here..............

## How to use the demo
An example configuration file is provided in the *example* folder, and includes all the parameters and the input data accepted by the program. The description of the meaning of each entry is already included in the configuration file.

## Licensing
This software is released under the GPL3 license. See the [LICENSE](LICENSE) file for more information.

## Notes on the current version
* The code at the actual state accepts frames as a sequence of images. The sequence for each camera is stored inside a separate   folder, and image frames at same instant have the same name (ex.cam0/img65.png and cam1/img65.png). An interface is included   to make simple the extension to new forms of input gathering in the future.
* ORSA estimator implementation is the default, but extensibility to other estimators (like RANSAC) is provided.
* Interfaces for adding different non linear optimizers and different matchers (used at initialization stage) are provided.

