#include "maskchar.h"

Maskchar::Maskchar()
{

}

int Maskchar::getLicenseplate(std::vector<Rect> boxes, std::vector<string> classes, string &licenseplate,Mat &image, std::vector<int> classIds,std::vector<float> scores)
{
    licenseplateInfor license;
    BVmodel bvmodel;
    float averageY = 0;
    std::vector<Rect> box;
    string line1,line2,line3,line4;
    int flags;

    float averageHeight = 0, averageWidth = 0;
    bvmodel.BVKmean(boxes,image,flags);

    // calculate average of y, height, width
    for(int i =0;i <int(boxes.size());i++){
        averageY += boxes[i].y;
        averageHeight += boxes[i].height;
        if(classes[classIds[i]] != "1")
            averageWidth += boxes[i].width;
        else
            averageWidth += 1.5*boxes[i].width;
    }

    averageY /= int(boxes.size());
    averageHeight /=int(boxes.size());
    averageWidth /=int(boxes.size());
    line(image,Point(0,averageY),Point(image.cols,averageY),Scalar(0,0,255));
    // classify
    // classify
    vector<licenseplateInfor> licenseVec1,licenseVec2;
    onelineFlag = true;
    for(int i=1;i <int(boxes.size());i++){
        if(boxes.size() >=2){
            if(abs(boxes[0].y - boxes[i].y) > averageHeight/2){
                onelineFlag = false ;
            }
            //            for(int j =0;j< int(boxes.size());j++){
            //                if(i != j)
            //                    if(abs(boxes[j].x - boxes[i].x) < averageWidth/4){
            //                        scores[j] > scores[i] ? boxes.erase(boxes.begin()+i): boxes.erase(boxes.begin()+j);
            //                    }
            //            }
        }
    }
    if(onelineFlag){
        if(averageY < averageHeight/2){
            for(int i=0;i <int(boxes.size());i++){
                license.x = boxes[i].x ;
                license.y = boxes[i].y ;
                license.classes = classes[classIds[i]];
                license.scores = scores[i];
                licenseVec1.push_back(license);
            }

        }
        else{
            for(int i=0;i <int(boxes.size());i++){
                license.x = boxes[i].x ;
                license.y = boxes[i].y ;
                license.scores = scores[i];
                license.classes = classes[classIds[i]];
                licenseVec2.push_back(license);
            }
        }
    }
    else{
        for(int i=0;i <int(boxes.size());i++){
            if(boxes[i].y <averageY){
                license.x = boxes[i].x ;
                license.y = boxes[i].y ;
                license.scores = scores[i];
                license.classes = classes[classIds[i]];
                licenseVec1.push_back(license);
            }
            else{
                license.x = boxes[i].x ;
                license.y = boxes[i].y ;
                license.scores = scores[i];
                license.classes = classes[classIds[i]];
                licenseVec2.push_back(license);
                box.push_back(boxes[i]);
            }
        }
    }
    // bubble sort
    if(licenseVec1.size())
        for(int j =1 ; j< int(licenseVec1.size());j++)
            for(int i =1 ; i< int(licenseVec1.size());i++){
                if(licenseVec1[i-1].x > licenseVec1[i].x){
                    licenseplateInfor temp =  licenseVec1[i];
                    licenseVec1[i] = licenseVec1[i-1];
                    licenseVec1[i-1] = temp;
                }
            }
    if(licenseVec2.size())
        for(int j =1 ; j< int(licenseVec2.size());j++)
            for(int i =1 ; i< int(licenseVec2.size( ));i++){
                if(licenseVec2[i-1].x > licenseVec2[i].x){
                    licenseplateInfor temp =  licenseVec2[i];
                    licenseVec2[i] = licenseVec2[i-1];
                    licenseVec2[i-1] = temp;
                }
            }

    int index = 0;
    int pointx,pointy;
    float totalerror1 = 0,totalerror2;
    vector<int> xVec1,xVec2,xVec3,xVec4;
    float angel =  bvmodel.BVKmean(box,image,flags);
    if(!licenseVec1.empty()){
        //----------------------hang tren 4 so---------------------
        for(int i =0; i < 4;i++){
            index =i;
            if(i >= 2)
                index =i +1;
            pointx = (-5*averageWidth*cos(angel) +image.cols)/(2) + averageWidth*index;
            if(i > licenseVec1.size())
                pointy = licenseVec1[licenseVec1.size() - 1].y;
            else
                pointy = licenseVec1[i].y;
            xVec1.push_back(pointx);
            circle(image,Point(pointx,pointy),3,Scalar(0,255,255));
            //            circle(image,Point(licenseVec1[i].x,licenseVec1[i].y),3,Scalar(0,0,255));
        }
        //---------------------Hang tren 5 so--------------------------
        pointx = (-6*averageWidth*cos(angel) +image.cols)/(2);
        xVec4.push_back(pointx);
        pointy = licenseVec1[0].y;
        circle(image,Point(pointx,pointy),5,Scalar(0,0,0));

        pointx = (-6*averageWidth +image.cols )/(2)+1.1*cos(angel)*averageWidth;
        xVec4.push_back(pointx);
        pointy = licenseVec1[1].y;
        circle(image,Point(pointx,pointy),5,Scalar(0,0,0));

        pointx = (-6*averageWidth*cos(angel) +image.cols)/(2)+1.1*cos(angel)*averageWidth*3;
        xVec4.push_back(pointx);
        pointy = licenseVec1[2].y;
        circle(image,Point(pointx,pointy),5,Scalar(0,0,0));

        pointx = (-6*averageWidth*cos(angel) +image.cols)/(2) +1.1*cos(angel)*averageWidth*4;
        xVec4.push_back(pointx);
        pointy = licenseVec1[3].y;
        circle(image,Point(pointx,pointy),5,Scalar(0,0,0));

        pointx =  (-6*averageWidth*cos(angel) +image.cols)/(2) +1.1*cos(angel)*averageWidth*5 ;
        xVec4.push_back(pointx);
        pointy = licenseVec1[3].y;
        circle(image,Point(pointx,pointy),5,Scalar(0,0,0));
    }


    if(!licenseVec2.empty()){
        // -------------------hang duoi 4 so--------------------------
        pointx = (-5*averageWidth*cos(angel) +image.cols+ 8*averageHeight*(1-cos(angel)))/(2);
        xVec3.push_back(pointx);
        //    pointy = licenseVec2[0].y;
        //    circle(image,Point(pointx,pointy),5,Scalar(255,0,255));


        pointx = (-5*averageWidth*cos(angel)+image.cols+ 8*averageHeight*(1-cos(angel)))/(2)+1.4*cos(angel)*averageWidth;
        xVec3.push_back(pointx);
        //    pointy = licenseVec2[1].y;
        //    circle(image,Point(pointx,pointy),5,Scalar(255,0,255));


        pointx = (-5*averageWidth*cos(angel) +image.cols+ 8*averageHeight*(1-cos(angel)))/(2)+1.4*cos(angel)*averageWidth*2;
        xVec3.push_back(pointx);
        //    pointy = licenseVec2[2].y;
        //    circle(image,Point(pointx,pointy),5,Scalar(255,0,255));


        pointx = (-5*averageWidth*cos(angel) +image.cols+ 8*averageHeight*(1-cos(angel)))/(2)+1.4*cos(angel)*averageWidth*3;
        xVec3.push_back(pointx);
        //    pointy = licenseVec2[3].y;
        //    circle(image,Point(pointx,pointy),5,Scalar(255,0,255));


        //----------- hang duoi 5 so--------------------------------
        pointx = (-6*averageWidth*cos(angel) +image.cols+ 10*averageHeight*(1-cos(angel)))/(2);
        xVec2.push_back(pointx);
        //    pointy = licenseVec2[0].y;
        //    circle(image,Point(pointx,pointy),5,Scalar(0,0,0));

        pointx = (-6*averageWidth +image.cols + 10*averageHeight*(1-cos(angel)))/(2)+1.1*cos(angel)*averageWidth;
        xVec2.push_back(pointx);
        //    pointy = licenseVec2[1].y;
        //    circle(image,Point(pointx,pointy),5,Scalar(0,0,0));

        pointx = (-6*averageWidth*cos(angel) +image.cols+ 10*averageHeight*(1-cos(angel)))/(2)+1.1*cos(angel)*averageWidth*2;
        xVec2.push_back(pointx);
        //    pointy = licenseVec2[2].y;
        //    circle(image,Point(pointx,pointy),5,Scalar(0,0,0));

        pointx = image.cols - (-6*averageWidth*cos(angel) +image.cols+ 10*averageHeight*(1-cos(angel)))/(2) -cos(angel)*averageWidth*2;
        xVec2.push_back(pointx);
        //    pointy = licenseVec2[3].y;
        //    circle(image,Point(pointx,pointy),5,Scalar(0,0,0));

        pointx = image.cols - (-6*averageWidth*cos(angel) +image.cols+ 10*averageHeight*(1-cos(angel)))/(2) -0.8*cos(angel)*averageWidth ;
        xVec2.push_back(pointx);
        //    pointy = licenseVec2[4].y;
        //    circle(image,Point(pointx,pointy),5,Scalar(0,0,0));
    }
    for(int i =0; i < licenseVec2.size();i++){
        circle(image,Point(licenseVec2[i].x,licenseVec2[i].y),3,Scalar(0,0,255));
    }
    bottomrowfullFlag = false;
    if(licenseVec2.size() == 5){
        bottomrowfullFlag = true;
    }
    //-----------------------------------------hang tren-------------------------
    if(licenseVec1.size() == 5){
        line4 = "$$$$$\n";
        for(int i = 0; i<licenseVec1.size();i++){
            int index = 0;
            float distance = abs(xVec4[0] - licenseVec1[i].x);
            for(int j = 1; j < xVec4.size();j++){
                if(abs(xVec4[j] - licenseVec1[i].x) < distance ){
                    distance = abs(xVec4[j] - licenseVec1[i].x);

                    index = j;
                }
            }
            if(line4[index] == '$')
                line4[index] = licenseVec1[i].classes[0];
            else
                line4[index+1] = licenseVec1[i].classes[0];
        }
        licenseplate = line4;
    }
    else if(!licenseVec1.empty()){
        line4 ="$$$$$\n";
        line1 = "$$$$\n";
        if( bottomrowfullFlag && licenseVec1[0].x> licenseVec2[0].x && licenseVec1[0].x < licenseVec2[1].x &&
                licenseVec1[licenseVec1.size()-1].x < licenseVec2[4].x &&  licenseVec1[licenseVec1.size()-1].x > licenseVec2[3].x ){
            for(int i = 0; i<licenseVec1.size();i++){
                int index = 0;
                float distance = abs(xVec1[0] - licenseVec1[i].x);
                for(int j = 1; j < xVec1.size();j++){
                    if(abs(xVec1[j] - licenseVec1[i].x) < distance ){
                        distance = abs(xVec1[j] - licenseVec1[i].x);
                        index = j;
                    }
                }
                if(line1[index] == '$')
                    line1[index] = licenseVec1[i].classes[0];
                else
                    line1[index+1] = licenseVec1[i].classes[0];
            }
            licenseplate = line1;
        }
        else{

            totalerror1 = 0;
            totalerror2 =0;
            for(int i = 0; i<licenseVec1.size();i++){
                int index = 0;
                float distance = abs(xVec4[0] - licenseVec1[i].x);
                for(int j = 1; j < xVec4.size();j++){
                    if(abs(xVec4[j] - licenseVec1[i].x) < distance ){
                        distance = abs(xVec4[j] - licenseVec1[i].x);
                        index = j;

                    }
                }
                totalerror1 +=distance;
                if(line4[index] == '$')
                    line4[index] = licenseVec1[i].classes[0];
                else
                    line4[index+1] = licenseVec1[i].classes[0];
            }
            for(int i = 0; i<licenseVec1.size();i++){
                int index = 0;
                float distance = abs(xVec1[0] - licenseVec1[i].x);
                for(int j = 1; j < xVec1.size();j++){
                    if(abs(xVec1[j] - licenseVec1[i].x) < distance ){
                        distance = abs(xVec1[j] - licenseVec1[i].x);
                        index = j;

                    }
                }
                totalerror2 +=distance;
                if(line1[index] == '$')
                    line1[index] = licenseVec1[i].classes[0];
                else
                    line1[index+1] = licenseVec1[i].classes[0];
            }
            totalerror1 > totalerror2 ? licenseplate = line1 :licenseplate =line4 ;
        }
    }
    //------------------------hang duoi-----------------------------------------
    bottomrowfullFlag = false;
    if(licenseVec2.size() == 5){
        line2 = "$$$$$";
        for(int i = 0; i<licenseVec2.size();i++){
            line2[i] = licenseVec2[i].classes[0];
            //            int index = 0;
            //            float distance = abs(xVec2[0] - licenseVec2[i].x);
            //            for(int j = 1; j < xVec2.size();j++){
            //                if(abs(xVec2[j] - licenseVec2[i].x) < distance ){
            //                    distance = abs(xVec2[j] - licenseVec2[i].x);

            //                    index = j;
            //                }
            //            }
            //            if(line2[index] == '$')
            //                line2[index] = licenseVec2[i].classes[0];
            //            else
            //                line2[index+1] = licenseVec2[i].classes[0];
        }
        licenseplate  +=line2;
    }
    else if(!licenseVec2.empty()){
        line2 ="$$$$$";
        line3 = "$$$$";
        totalerror1 = 0;
        totalerror2 =0;
        for(int i = 0; i<licenseVec2.size();i++){
            int index = 0;
            float distance = abs(xVec2[0] - licenseVec2[i].x);
            for(int j = 1; j < xVec2.size();j++){
                if(abs(xVec2[j] - licenseVec2[i].x) < distance ){
                    distance = abs(xVec2[j] - licenseVec2[i].x);
                    index = j;

                }


            }
            totalerror1 +=distance;
            if(line2[index] == '$')
                line2[index] = licenseVec2[i].classes[0];
            else
                line2[index+1] = licenseVec2[i].classes[0];
        }
        for(int i = 0; i<licenseVec2.size();i++){
            int index = 0;
            float distance = abs(xVec3[0] - licenseVec2[i].x);
            for(int j = 1; j < xVec3.size();j++){
                if(abs(xVec3[j] - licenseVec2[i].x) < distance ){
                    distance = abs(xVec3[j] - licenseVec2[i].x);
                    index = j;

                }


            }
            totalerror2 +=distance;
            if(line3[index] == '$')
                line3[index] = licenseVec2[i].classes[0];
            else
                line3[index+1] = licenseVec2[i].classes[0];
        }
        totalerror1 > totalerror2 ? licenseplate +=line3 :licenseplate +=line2;
    }



    return 0 ;

}
void Maskchar::preprocess(Mat &image)
{


}
