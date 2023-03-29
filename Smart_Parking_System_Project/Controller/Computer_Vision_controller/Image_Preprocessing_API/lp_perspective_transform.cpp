#include "lp_perspective_transform.h"

PerspectiveTransform::PerspectiveTransform()
{

}

float PerspectiveTransform::LuminanceValue(Mat image)
{
    if(image.empty())
    {
        cout << "Could not open or find the image!\n" << endl;
        return 0;
    }

    int channel = image.channels();
    int width = image.cols;
    int height = image.rows;
    float sum = 0;
    float count = 0;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            Vec3b bgrPixel = image.at<Vec3b>(i, j);
            // Y' = 0.299*R + 0.587*G + 0.144*B
            float Y = 0.299*bgrPixel.val[2] + 0.587*bgrPixel.val[1] + 0.144*bgrPixel.val[0];
            sum += Y;
            count ++;
        }
    }
    return (sum/count);
}

Mat PerspectiveTransform::Transform(Mat image)
{
    float width = image.cols;
    float height = image.rows;
    int max_area = 0;
    float threshold_value = 0;
    Mat src_gray, canny_output, matrix;
    Mat imgWarp = Mat::zeros(image.size(), CV_8UC1);
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    vector<Point> position;
    vector<Point2f> corners;
    Point2f corners2[4];

    cvtColor( image, src_gray, COLOR_BGR2GRAY );
    threshold_value = LuminanceValue(image);
    if(threshold_value == 0){
        return imgWarp;
    }
    threshold(src_gray, canny_output, threshold_value, 255, THRESH_BINARY);
    findContours( canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE );
    vector<vector<Point>> hull(contours.size());
    for(int i = 0; i < contours.size(); i++){
        convexHull(Mat(contours[i]), hull[i], false);
    }
    for(int i = 0; i < contours.size(); i++) {
        if(contourArea(hull[i])>contourArea(hull[max_area])){
            max_area = i;
        }
    }
    if(!hull.empty()){
        RotatedRect boundingBox = minAreaRect(hull[max_area]);
        Point2f corners2[4];
        boundingBox.points(corners2);
        Point2f dst_img[4] = {{0.0f,0.0f},{width,0.0f},{0.0f,height},{width,height}};
        if(abs(corners2[0].x - corners2[3].x) < width/3){
            Point2f src_img[4] = {corners2[0],corners2[1],corners2[3],corners2[2]};
            matrix = getPerspectiveTransform(src_img,dst_img);
        }
        else{
            Point2f src_img[4] = {corners2[1],corners2[2],corners2[0],corners2[3]};
            matrix = getPerspectiveTransform(src_img,dst_img);
        }
        warpPerspective(image,imgWarp,matrix,Point(width,height));
    }
    return imgWarp;
}
