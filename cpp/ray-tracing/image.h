#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <vector>
#include <assert.h>

struct Pixel {
  int r,g,b;

  static Pixel average(Pixel p1, Pixel p2) {
    return {
      (p1.r + p2.r)/2,
      (p1.g + p2.g)/2,
      (p1.b + p2.b)/2,
    };
  }

  Pixel& operator += (Pixel other) {
    r += other.r;
    g += other.g;
    b += other.b;
    return *this;
  }

  Pixel& operator/=(int denom) {
    r /= denom;
    g /= denom;
    b /= denom;
    return *this;
  }
};

class Image {
public:
  Image(int width, int height): width(width), height(height)
  {
    pixels.reserve(width * height);
  }

  void pushPixel(Pixel p) {
    pixels.push_back(p);
  }

  Image& operator/=(int denom) {
    for (auto& pixel: pixels) {
      pixel /= denom;
    }
    return *this;
  }

  static Image average(Image& image1, Image& image2){
    assert(image1.width == image2.width && image1.height == image2.height);

    Image averageImage(image1.width, image1.height);

    for (int i=0 ; i<image1.pixels.size() ; ++i) {
      averageImage.pixels.push_back(Pixel::average(image1.pixels[i], image2.pixels[i]));
    }

    return averageImage;
  }

  static Image average(std::vector<Image> images) {

    Image averageImage(images[0].width, images[0].height);

    for (int i=0 ; i<images[0].pixels.size() ; i++) {
      averageImage.pushPixel({0,0,0});
    }

    for (int image=0 ; image<images.size() ; image++) {
      for (int pixel=0 ; pixel<images[image].pixels.size() ; ++pixel) {
        averageImage.pixels[pixel] += images[image].pixels[pixel];
      }
    }
    auto size = images.size();
    averageImage /= size;

    return averageImage;
  }

  void output() {
    std::cout << "P3\n" << width << " " << height << "\n255\n";
    for (Pixel pixel : pixels) {
      std::cout << pixel.r << ' ' << pixel.g << ' ' << pixel.b << ' ' << '\n';
    }
  }

private:
  int width; //pixels
  int height; //pixels
  std::vector<Pixel> pixels;
};


#endif