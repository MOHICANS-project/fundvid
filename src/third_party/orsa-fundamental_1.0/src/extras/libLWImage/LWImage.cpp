/**
 * @file LWImage.cpp
 * @brief Light weight image structure
 * @author Pascal Monasse
 *
 * Copyright (c) 2010 Pascal Monasse
 * All rights reserved.
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD License. You
 * should have received a copy of this license along this program. If
 * not, see <http://www.opensource.org/licenses/bsd-license.html>.
 */

#ifdef LWIMAGE_H // Do nothing if not included from LWImage.h

/// Constructor.
template <typename T>
LWImage<T>::LWImage()
  : data(0), w(0), h(0), comps(0), planar(true)
{}

/// Constructor.
template <typename T>
LWImage<T>::LWImage(T* i_data, int i_w, int i_h, int i_comps)
  : data(i_data), w(i_w), h(i_h), comps(i_comps), planar(true)
{}

/// Is pixel inside image?
template <typename T>
bool LWImage<T>::valid(int x, int y) const
{
    return (0 <= x && x < w && 0 <= y && y < h);
}

/// Step between one pixel and next one.
template <typename T>
int LWImage<T>::step() const
{ return (planar? 1: comps); }

/// Step between one component of pixel and next one.
template <typename T>
int LWImage<T>::stepComp() const
{ return (planar? w*h: 1); }

/// Return pointer to data at pixel.
template <typename T>
T* LWImage<T>::pixel(int x, int y)
{
    return (data + step()*(y*w+x));
}

/// Return pointer to data at pixel.
template <typename T>
const T* LWImage<T>::pixel(int x, int y) const
{
    return (data + step()*(y*w+x));
}

/// Value of x in [0,w-1], with mirror and periodization.
inline void wrap(int& x, int w)
{
    if(x < 0)       x  =-(x+1);
    while(x >= 2*w) x -= 2*w;
    if(x >= w)      x  = 2*w-x-1;
}

/// Return pixel value even outside image: image is supposed infinite, with
/// mirror effect and periodization.
template <typename T>
const T* LWImage<T>::pixel_ext(int x, int y) const
{
    wrap(x,w);
    wrap(y,h);
    return pixel(x,y);
}

#endif // LWIMAGE_H
