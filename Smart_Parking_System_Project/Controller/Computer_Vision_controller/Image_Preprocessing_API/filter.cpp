#include "filter.h"
#include "algorithm"
#include "chrono"

Filter::Filter()
{

}

void Filter::medianFilter(Mat &input, int size,int mode)
{
    Mat temp =input.clone();
   int kernel[size*size];
   int index,xcdt,ycdt,icdt,jcdt;
   // Mat output = input.clone();
   icdt =input.rows-size/2;
   jcdt = input.cols-size/2;
    for(int channels =0; channels < 3; channels ++){
        for(int i = size/2 ; i< icdt;i++){
            for(int j = size/2 ; j <jcdt ; j++){
                //caculate median
                //kernel.clear();
                index =0;
                xcdt = i+size/2;
                ycdt = j+size/2;
                for(int x = i-size/2; x <= xcdt ;x++)
                    for (int y = j-size/2; y <= ycdt ;y++) {
                        kernel[index++] = (input.at<Vec3b>(x,y)[channels]);
                    }
                sort(kernel,kernel+size*size);

                if(mode == 0)
                   temp.at<Vec3b>(i,j)[channels] =kernel[0];

                else if(mode == 1)
                    temp.at<Vec3b>(i,j)[channels] =kernel[size*size/2];
                else if(mode == 2)
                    temp.at<Vec3b>(i,j)[channels] =kernel[size*size-1];
                else
                    temp.at<Vec3b>(i,j)[channels] =kernel[size*size*2/3];

            }
        }
    }
    input =temp;


}
