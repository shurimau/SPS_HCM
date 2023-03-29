#include "illuminationcorrections.h"
#include "vector"
#include <string>
using namespace std::chrono;
Illuminationcorrections::Illuminationcorrections()
{

}

Mat Illuminationcorrections::Linght_correction(Mat inputImage, float alpha, float beta)
{
//    Mat inputImage_hsv = RGB2HSVmatrix(inputImage);
//    Mat outputimage = inputImage_hsv.clone();
//    for(int i = 0; i<inputImage_hsv.rows;i++){
//        for (int  j = 0;  j< inputImage_hsv.cols; j++) {
//            outputimage.at<Vec3b>(i,j)[2] = alpha*inputImage_hsv.at<Vec3b>(i,j)[2] + beta;
//        }
//    }
//    outputimage = HSV2RGBmatrix(outputimage);
   return inputImage;
}

int Illuminationcorrections::Max(Vec3b vec)
{
    if(vec[0]>vec[1]){
        if(vec[0]>vec[2])
            return 0;
    }
    else if(vec[1]>vec[2])
        return 1;
    return 2;
}
float Illuminationcorrections::Average(Mat kernel, int x1, int y1, int x2, int y2,int chanel)
{
    int pixel = 0;
    int n = 0;
    for(int i = x1; i<x2;i++){
        for(int j = y1; j<y2;j++ ){
            pixel += kernel.at<Vec3b>(Point(j,i))[chanel];
            n++;
        }
    }
    return 1.0*pixel/n;
}

Mat Illuminationcorrections::FourierTransform(Mat img)
{
    Mat imgBGR = img.clone();
    Mat imgGray ;
    cvtColor(img,imgGray,COLOR_BGR2GRAY);

    Mat padded;                            //expand input image to optimal size
    int m = getOptimalDFTSize( imgGray.rows );
    int n = getOptimalDFTSize( imgGray.cols ); // on the border add zero values
    copyMakeBorder(imgGray, padded, 0, m - imgGray.rows, 0, n - imgGray.cols, BORDER_CONSTANT, Scalar::all(0));

    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexI;
    merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

    dft(complexI, complexI);            // this way the result may fit in the source matrix

    // compute the magnitude and switch to logarithmic scale
    // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    Mat magI = planes[0];

    magI += Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);

    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(magI, magI, 0, 1, NORM_MINMAX);
    //cout << typeid(magI).name() << endl;
    //cout << magI;
    return magI;

}

void Illuminationcorrections::RGB2HSVmatrix(Mat &RGBraw)
{
    for (int y = 0; y < RGBraw.rows; y++)
    {
        for (int x = 0; x < RGBraw.cols; x++)
        {
            float Rtemp = RGBraw.at<Vec3b>(y, x)[2] / 255.0, Gtemp = RGBraw.at<Vec3b>(y, x)[1] / 255.0, Btemp = RGBraw.at<Vec3b>(y, x)[0] / 255.0;
            float cmax = max(Rtemp, max(Gtemp, Btemp));
            float cmin = min(Rtemp, min(Gtemp, Btemp));
            float delta = cmax - cmin;

            /*Each pixel of HSVmatrix contains : Hue = HSVmatrix.at<Vec3b>(y, x)[0], Saturation = HSVmatrix.at<Vec3b>(y, x)[1],
            Value = HSVmatrix.at<Vec3b>(y, x)[2], with the range is[0..360], [0..1], [0..1], respectively*/
            // if cmax and cmin are equal then Hue = 0
            if (delta == 0)
                RGBraw.at<Vec3b>(y, x)[0] = 0;
            // if cmax equal Rtemp then compute Hue
            else if (cmax == Rtemp)
                if((60.0 * ((Gtemp - Btemp) / delta))/2 < 0)
                    RGBraw.at<Vec3b>(y, x)[0] = 360 + (60.0 * ((Gtemp - Btemp) / delta))/2;
                else
                    RGBraw.at<Vec3b>(y, x)[0] = (60.0 * ((Gtemp - Btemp) / delta))/2;
            // if cmax equal Gtemp then compute Hue
            else if (cmax == Gtemp)
                if((60.0 * ((Btemp - Rtemp) / delta) + 120)/2 < 0)
                    RGBraw.at<Vec3b>(y, x)[0] = 360 + (60.0 * ((Btemp - Rtemp) / delta) + 120)/2;
                else
                    RGBraw.at<Vec3b>(y, x)[0] = (60.0 * ((Btemp - Rtemp) / delta) + 120)/2;
            // if cmax equal Btemp then compute Hue
            else if (cmax == Btemp)
                if((60.0 * ((Rtemp - Gtemp) / delta) + 240)/2 < 0)
                    RGBraw.at<Vec3b>(y, x)[0] = 360 + (60.0 * ((Rtemp - Gtemp) / delta) + 240)/2;
                else
                    RGBraw.at<Vec3b>(y, x)[0] = (60.0 * ((Rtemp - Gtemp) / delta) + 240)/2;

            if (cmax > 0)
                RGBraw.at<Vec3b>(y, x)[1] = (delta / cmax) *255.0;  //Calculate Saturation = HSVpixel[1]
            else
                RGBraw.at<Vec3b>(y, x)[1] = 0.0;
            RGBraw.at<Vec3b>(y, x)[2] = cmax *255.0;              //Calculate Value = HSVpixel[2]
        }
    }
}

void Illuminationcorrections::HSV2RGBmatrix(Mat &HSVmatrix)
{
    for (int y = 0; y < HSVmatrix.rows; y++)
    {
        for (int x = 0; x < HSVmatrix.cols; x++)
        {
            float C = (HSVmatrix.at<Vec3b>(y, x)[1] / 255.0) * (HSVmatrix.at<Vec3b>(y, x)[2] / 255.0);
            float X = C * (1 - abs(fmod(HSVmatrix.at<Vec3b>(y, x)[0] / 60.0, 2) - 1));
            float m = HSVmatrix.at<Vec3b>(y, x)[2] / 255.0 - C;
            float r, g, b;
            Vec3b RGBpixel;


            if (HSVmatrix.at<Vec3b>(y, x)[0]*2 >= 0 && HSVmatrix.at<Vec3b>(y, x)[0]*2 < 60) {
                r = C, g = X, b = 0;
            }
            else if (HSVmatrix.at<Vec3b>(y, x)[0]*2 >= 60 && HSVmatrix.at<Vec3b>(y, x)[0]*2 < 120) {
                r = X, g = C, b = 0;
            }
            else if (HSVmatrix.at<Vec3b>(y, x)[0]*2 >= 120 && HSVmatrix.at<Vec3b>(y, x)[0]*2 < 180) {
                r = 0, g = C, b = X;
            }
            else if (HSVmatrix.at<Vec3b>(y, x)[0]*2 >= 180 && HSVmatrix.at<Vec3b>(y, x)[0]*2 < 240) {
                r = 0, g = X, b = C;
            }
            else if (HSVmatrix.at<Vec3b>(y, x)[0]*2 >= 240 && HSVmatrix.at<Vec3b>(y, x)[0]*2 < 300) {
                r = X, g = 0, b = C;
            }
            else {
                r = C, g = 0, b = X;
            }
            HSVmatrix.at<Vec3b>(y, x)[2] = (r + m) * 255;
            HSVmatrix.at<Vec3b>(y, x)[1] = (g + m) * 255;
            HSVmatrix.at<Vec3b>(y, x)[0] = (b + m) * 255;
        }
    }

}

void Illuminationcorrections::imageHistogram(Mat image, int histogram[])
{
    // initialize all intensity values to 0
    for(int i = 0; i < 256; i++) {
        histogram[i] = 0;
    }
    // calculate the number of pixels for each intensity value
    for(int y = 0; y < image.rows; y++) {
        for(int x = 0; x <image.cols; x++) {
            histogram[(int)image.at<uchar>(y,x)]++;
        }
    }
}

void Illuminationcorrections::cumulativeImageHistogram(int histogram[], int cumulativeHistogram[])
{
    cumulativeHistogram[0] = histogram[0];
    for(int i = 1; i < 256; i++) {
        cumulativeHistogram[i] = histogram[i] + cumulativeHistogram[i-1];
    }
}

void Illuminationcorrections::histogramDisplay(int histogram[], const char *name)
{
    int newHistogram[256];
    for(int i = 0; i < 256; i++) {
        newHistogram[i] = histogram[i];
    }
    //histogram size
    int histogramWidth = 640;
    int histogramHeight = 500;
    //creating "bins" for the range of 256 intensity values
    int binWidth = cvRound((double) histogramWidth/256);
    Mat histogramImage(histogramHeight, histogramWidth, CV_8UC1, Scalar(255, 255, 255));
    //finding maximum intensity level in the histogram
    int maximumIntensity  = newHistogram[0];
    for(int i = 1; i < 256; i++){
        if(maximumIntensity < newHistogram[i]){
            maximumIntensity = newHistogram[i];
        }
    }
    //normalizing histogram in terms of rows (y)
    for(int i = 0; i < 256; i++) {
        newHistogram[i] = ((double)newHistogram[i]/maximumIntensity)*histogramImage.rows;
    }
    //drawing the intensity level - line
    for(int i = 0; i < 256; i++) {
        line(histogramImage, Point(binWidth*(i), histogramHeight ), Point(binWidth*(i), histogramHeight - newHistogram[i]), Scalar(0,0,0), 1, 8, 0);
    }
    // display
    namedWindow(name, WINDOW_AUTOSIZE);
    imshow(name, histogramImage);
}

void Illuminationcorrections::histogramequalization(Mat &image)
{
    int histogram[256];
    imageHistogram(image, histogram);
    int size = image.rows*image.cols;
    float alpha = 255.0/size;
    //Probability distribution for intensity levels
    float PRk[256];
    for(int i = 0; i < 256; i++) {
        PRk[i] = (double)histogram[i] / size;
    }
    int cumulativeHistogram[256];
    cumulativeImageHistogram(histogram,cumulativeHistogram);
    //Scaling operation
    int Sk[256];
    for(int i = 0; i < 256; i++)
    {
        Sk[i] = cvRound((double)cumulativeHistogram[i] * alpha);
    }
    //Creating equalized image


    for(int x = 0; x <image.rows ; x++) {
        for(int y=  0; y < image.cols; y++) {
            image.at<uchar>(x,y) = saturate_cast<uchar>(Sk[image.at<uchar>(x,y)]);
        }
    }


}


