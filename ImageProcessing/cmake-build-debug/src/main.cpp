//
// Created by jatin on 3/28/2023.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>

using namespace std;

struct Header {
    int idLength;
    bool colorMapType;
    bool dataTypeCode;

    short colorMapOrigin ;
    short colorMapLength;
    short colorMapDepth;

    short xOrigin;
    short yOrigin ;
    short width;
    short height;

    char bitsPerPixel;
    char imageDescriptor;
    struct Pixel {
        unsigned char b;
        unsigned char g;
        unsigned char r;
    };
    vector<Pixel> pixels;
};

Header read(string name){
    Header header;
    ifstream inFile(name, ios_base::binary);

    inFile.read((char*)&header.idLength, 1);
    inFile.read((char*)&header.colorMapType, 1);
    inFile.read((char*)&header.dataTypeCode, 1);
    inFile.read((char*)&header.colorMapOrigin, 2);
    inFile.read((char*)&header.colorMapLength, 2);
    inFile.read((char*)&header.colorMapDepth, 1);
    inFile.read((char*)&header.xOrigin, 2);
    inFile.read((char*)&header.yOrigin, 2);
    inFile.read((char*)&header.width, 2);
    inFile.read((char*)&header.height, 2);
    inFile.read((char*)&header.bitsPerPixel, 1);
    inFile.read((char*)&header.imageDescriptor, 1);

    int a = (int)header.width * header.height;
        for(int i = 0; i <a ; i++){ //262144 TA told me to hardcode a with 262144 incase its not working
            Header::Pixel pixel_data;
            inFile.read((char*)&pixel_data.b, 1);
            inFile.read((char*)&pixel_data.g, 1);
            inFile.read((char*)&pixel_data.r, 1);

            header.pixels.push_back(pixel_data);
        }
        return header;
    }

void write(Header &header, string name){
    ofstream outFile(name, ios::binary);

    outFile.write((char*)&header.idLength, 1);
    outFile.write((char*)&header.colorMapType, 1);
    outFile.write((char*)&header.dataTypeCode, 1);
    outFile.write((char*)&header.colorMapOrigin, 2);
    outFile.write((char*)&header.colorMapLength, 2);
    outFile.write((char*)&header.colorMapDepth, 1);
    outFile.write((char*)&header.xOrigin, 2);
    outFile.write((char*)&header.yOrigin, 2);
    outFile.write((char*)&header.width, 2);
    outFile.write((char*)&header.height, 2);
    outFile.write((char*)&header.bitsPerPixel, 1);
    outFile.write((char*)&header.imageDescriptor, 1);

    for(int i = 0; i< header.pixels.size(); i++){
        outFile.write((char*)&header.pixels[i].b, 1);
        outFile.write((char*)&header.pixels[i].g, 1);
        outFile.write((char*)&header.pixels[i].r, 1);
        }
}

Header Multiply(Header &h1, Header &h2){ // right side me green blob aara h fix this mf
    Header multipliedHeader = h1;
    for(int i = 0; i< h1.pixels.size(); i++){
        Header::Pixel pixel_data;
        pixel_data.b = (unsigned char)(((float)h1.pixels[i].b * (float)h2.pixels[i].b / 255.0) + 0.5f);
        pixel_data.g = (unsigned char)(((float)h1.pixels[i].g * (float)h2.pixels[i].g / 255.0) + 0.5f);
        pixel_data.r = (unsigned char)(((float)h1.pixels[i].r * (float)h2.pixels[i].r / 255.0) + 0.5f);


        if ((float)pixel_data.b > 255.0) {
            pixel_data.b = (unsigned char)255;
        }
        if ((float)pixel_data.g > 255.0){
            pixel_data.g = (unsigned char)255;
        }
        if ((float)pixel_data.r > 255.0) {
            pixel_data.r = (unsigned char)255;
        }


        multipliedHeader.pixels[i] = pixel_data;
    }
    return multipliedHeader;

}

Header Subtract(Header &h1, Header &h2){
    Header multipliedHeader = h1;
    for(int i = 0; i< h1.pixels.size(); i++){
        Header::Pixel pixel_data;

        if (((int)h1.pixels[i].b - (int)h2.pixels[i].b) < 0) {
            pixel_data.b = (unsigned char)0;
        }
        else{
            pixel_data.b = (unsigned char)((int)h1.pixels[i].b - (int)h2.pixels[i].b);
        }

        if (((int)h1.pixels[i].g - (int)h2.pixels[i].g) < 0) {
            pixel_data.g = (unsigned char)0;
        }
        else{
            pixel_data.g = (unsigned char)((int)h1.pixels[i].g - (int)h2.pixels[i].g);
        }



        if (((int)h1.pixels[i].r - (int)h2.pixels[i].r) < 0) {
            pixel_data.r = (unsigned char)0;
        }
        else{
            pixel_data.r = (unsigned char)((int)h1.pixels[i].r - (int)h2.pixels[i].r);
        }

        multipliedHeader.pixels[i] = pixel_data;
    }
    return multipliedHeader;

}

Header Addition(Header &h1, Header &h2){
    Header multipliedHeader = h1;
    for(int i = 0; i< h1.pixels.size(); i++){
        Header::Pixel pixel_data;

        if (((int)h1.pixels[i].b + (int)h2.pixels[i].b) > 255) {
            pixel_data.b = (unsigned char)255;
        }
        else{
            pixel_data.b = (unsigned char)((int)h1.pixels[i].b + (int)h2.pixels[i].b);
        }

        if (((int)h1.pixels[i].g + (int)h2.pixels[i].g) > 255) {
            pixel_data.g = (unsigned char)255;
        }
        else{
            pixel_data.g = (unsigned char)((int)h1.pixels[i].g + (int)h2.pixels[i].g);
        }



        if (((int)h1.pixels[i].r - (int)h2.pixels[i].r) > 255) {
            pixel_data.r = (unsigned char)255;
        }
        else{
            pixel_data.r = (unsigned char)((int)h1.pixels[i].r + (int)h2.pixels[i].r);
        }

        multipliedHeader.pixels[i] = pixel_data;
    }
    return multipliedHeader;

}

Header Screen(Header &h1, Header &h2){
    Header whiteImage = h1;
    for (int i = 0; i < h1.pixels.size(); i++) {
        Header::Pixel pixel;
        whiteImage.pixels[i].b = (unsigned char)255;
        whiteImage.pixels[i].g = (unsigned char)255;
        whiteImage.pixels[i].r = (unsigned char)255;
    }
    h1 = Subtract(whiteImage, h1);
    h2 = Subtract(whiteImage, h2);
    h1 = Multiply(h1,h2);
    return Subtract(whiteImage, h1);
}

unsigned char individual_pixel_overlay(unsigned char p1, unsigned char p2) {
    if(p2 < 128) {
        return (unsigned char)(2 * (int)p1 * (int)p2 / 255.0);
    }
    return (unsigned char)(255.0 - (2 * (255 - (int)p1) * (255 - (int)p2) / 255.0));
}

Header Overlay(Header& h1, Header& h2) {
    Header output = h1;
    for (int i = 0; i < h1.pixels.size(); i++) {

        output.pixels[i].b = individual_pixel_overlay(h1.pixels[i].b, h2.pixels[i].b);
        output.pixels[i].g = individual_pixel_overlay(h1.pixels[i].g, h2.pixels[i].g);
        output.pixels[i].r = individual_pixel_overlay(h1.pixels[i].r, h2.pixels[i].r);

        if ((float)output.pixels[i].b > 255.0) {
            output.pixels[i].b = (unsigned char)255;
        }
        if ((float)output.pixels[i].g > 255.0){
            output.pixels[i].g = (unsigned char)255;
        }
        if ((float)output.pixels[i].r > 255.0) {
            output.pixels[i].r = (unsigned char)255;
        }

        if ((float)output.pixels[i].b < 0) {
            output.pixels[i].b = (unsigned char)255;
        }
        if ((float)output.pixels[i].g < 0){
            output.pixels[i].g = (unsigned char)255;
        }
        if ((float)output.pixels[i].r < 0) {
            output.pixels[i].r = (unsigned char)255;
        }

    }
    return output;
}

Header AddGreen(Header &h1, int value){
    Header output = h1;
    for(int i=0; i < h1.pixels.size(); i++){

        if((int)output.pixels[i].g + value > 255){
            output.pixels[i].g = (unsigned char)255;
        }
        else{
            output.pixels[i].g = (unsigned char)((int)output.pixels[i].g + value);
        }

    }
    return output;

}

Header AddRed(Header &h1, int value){
    Header output = h1;
    for(int i=0; i < h1.pixels.size(); i++){

        if((int)output.pixels[i].r + value > 255){
            output.pixels[i].r = (unsigned char)255;
        }
        else{
            output.pixels[i].r = (unsigned char)((int)output.pixels[i].r + value);
        }

    }
    return output;

}

Header AddBlue(Header &h1, int value){
    Header output = h1;
    for(int i=0; i < h1.pixels.size(); i++){

        if((int)output.pixels[i].b + value > 255){
            output.pixels[i].b = (unsigned char)255;
        }
        else{
            output.pixels[i].b = (unsigned char)((int)output.pixels[i].b + value);
        }

    }
    return output;

}

Header ScaleBlue(Header &h1, int value){
    Header output = h1;
    for(int i=0; i < h1.pixels.size(); i++){
        if((int)output.pixels[i].b*value > 255){
            output.pixels[i].b = (unsigned char)255;
        }
        else{
            output.pixels[i].b = (unsigned char)((int)output.pixels[i].b*value);
        }
    }
    return output;
}

Header ScaleRed(Header &h1, int value){
    Header output = h1;
    for(int i=0; i < h1.pixels.size(); i++){
        if((int)output.pixels[i].r*value > 255){
            output.pixels[i].r = (unsigned char)255;
        }
        else{
            output.pixels[i].r = (unsigned char)((int)output.pixels[i].r*value);
        }
    }
    return output;
}

Header ScaleGreen(Header &h1, int value){
    Header output = h1;
    for(int i=0; i < h1.pixels.size(); i++){

        if((int)output.pixels[i].g*value > 255){
            output.pixels[i].g = (unsigned char)255;
        }
        else{
            output.pixels[i].g = (unsigned char)((int)output.pixels[i].g*value);
        }
    }
    return output;
}

Header OnlyBlue(Header &h1){
    Header output = h1;
    for(int i = 0; i<h1.pixels.size(); i++){
        output.pixels[i].b=h1.pixels[i].b;
        output.pixels[i].g=h1.pixels[i].b;
        output.pixels[i].r=h1.pixels[i].b;
    }
    return output;
};

Header OnlyGreen(Header &h1){
    Header output = h1;
    for(int i = 0; i<h1.pixels.size(); i++){
        output.pixels[i].b=h1.pixels[i].g;
        output.pixels[i].g=h1.pixels[i].g;
        output.pixels[i].r=h1.pixels[i].g;
    }
    return output;
};

Header OnlyRed(Header &h1){
    Header output = h1;
    for(int i = 0; i<h1.pixels.size(); i++){
        output.pixels[i].b=h1.pixels[i].r;
        output.pixels[i].g=h1.pixels[i].r;
        output.pixels[i].r=h1.pixels[i].r;
    }
    return output;
};

Header Combine(Header &red, Header &green, Header &blue){
    Header output = red;
    for(int i = 0; i<red.pixels.size(); i++){
        output.pixels[i].b=blue.pixels[i].r;
        output.pixels[i].g=green.pixels[i].r;
        output.pixels[i].r=red.pixels[i].r;
    }
    return output;
}

Header Flip(Header &h1){
    Header output = h1;
    for (int i = 0; i < h1.pixels.size(); i++) {

        output.pixels[i].b = h1.pixels[h1.pixels.size() - i - 1].b;
        output.pixels[i].g = h1.pixels[h1.pixels.size() - i - 1].g;
        output.pixels[i].r = h1.pixels[h1.pixels.size() - i - 1].r;
    }

    return output;
}

int main(){
    Header image1 = read("input/layer1.tga");
    Header image2 = read("input/pattern1.tga");
    Header result = Multiply(image1,image2);
    write(result, "output/part1.tga");

    image1 = read("input/car.tga");
    image2 = read("input/layer2.tga");
    result = Subtract(image1,image2);
    write(result, "output/part2.tga");

    image1 = read("input/layer1.tga");
    image2 = read("input/pattern2.tga");
    result = Multiply(image1, image2);
    Header image3 = read("input/text.tga");
    result = Screen(result,image3);
    write(result, "output/part3.tga");

    image1 = read("input/layer2.tga");
    image2 = read("input/circles.tga");
    result = Multiply(image1,image2);
    image3 = read("input/pattern2.tga");
    result = Subtract(result, image3);
    write(result, "output/part4.tga");

    image1 = read("input/layer1.tga");
    image2 = read("input/pattern1.tga");
    result = Overlay(image1, image2);
    write(result, "output/part5.tga");

    image1 = read("input/car.tga");
    result = AddGreen(image1, 200);
    write(result, "output/part6.tga");

    image1 = read("input/car.tga");
    result = ScaleRed(image1, 4);
    result = ScaleBlue(result, 0);
    write(result, "output/part7.tga");

    image1 = read("input/car.tga");
    result = OnlyRed(image1);
    write(result, "output/part8_r.tga");
    result = OnlyGreen(image1);
    write(result, "output/part8_g.tga");
    result = OnlyBlue(image1);
    write(result, "output/part8_b.tga");

    image1 = read("input/layer_red.tga");
    image2 = read("input/layer_green.tga");
    image3 = read("input/layer_blue.tga");
    result = Combine(image1, image2, image3);
    write(result, "output/part9.tga");

    image1 = read("input/text2.tga");
    result = Flip(image1);
    write(result, "output/part10.tga");
    return 0;
}
