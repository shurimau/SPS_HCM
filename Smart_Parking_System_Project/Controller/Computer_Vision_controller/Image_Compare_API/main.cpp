#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>



using namespace cv;
using namespace std;

Mat PCA_image_position(Mat inp_matrix){
    int size_out = (inp_matrix.cols * inp_matrix.rows);
    Mat outp_matrix = Mat::zeros(size_out, 1, inp_matrix.type());
    int k = 0;
    for (int i = 0; i < inp_matrix.cols; i++){
        for (int j = 0; j < inp_matrix.rows; j++){
            outp_matrix.at<Vec3b>(k,0) = inp_matrix.at<Vec3b>(j,i);
            k++;
        }
    }

    //cout << outp_matrix.size <<endl;
    return outp_matrix;
}

float MinElement(const float* array, int size){


    //Giả định giá trị lớn nhất là giá trị đầu tiên của mảng.
    int min = array[0];

    /*So sánh từng phần tử trong mảng với giá trị đầu tiên để tìm ra giá trị lớn nhất*/
    for (int i = 1; i < size; ++i) {
        if (min > array[i]) { //Thay đổi giá trị max nếu tìm ra số lớn hơn
            min = array[i];
        }
    }
    return min;
}

Mat Multi_matrix(Mat matrix1, Mat matrix2){
    Mat output = Mat::zeros(matrix1.rows, matrix2.cols, matrix1.type());
    // cout << matrix1.rows<< endl;
    for (int i = 0; i < matrix1.rows; i++){
        for(int j =0; j < matrix2.cols; j++){
            output.at<float>(i,j) = 0;
            for (int k = 0; k < matrix2.rows; k++){
                output.at<float>(i,j) =  output.at<float>(i,j) + (matrix1.at<float>(i,k) * (matrix2.at<float>(k,j)));

            }
        }
    }
    return output;
}
Mat transit_matrix(Mat in_matrix){
    Mat out_matrix = Mat::zeros(in_matrix.cols, in_matrix.rows, in_matrix.type());
    for (int i = 0; i < in_matrix.rows; i++){
        for(int j =0; j < in_matrix.cols; j++){
            out_matrix.at<float>(j,i) = in_matrix.at<float>(i,j);
        }
    }
    return out_matrix;
}
int length_Vec(Mat in_matrix){
    float lengthVec = 0;
    for (int i = 0; i < in_matrix.rows; i++){

        lengthVec = lengthVec + in_matrix.at<float>(i,0)*in_matrix.at<float>(i,0);
    }
    //    lengthVec = sqrt(lengthVec);
    //    cout<< lengthVec << endl;
    lengthVec = sqrt(lengthVec);
    //    cout<< lengthVec << endl;
    return lengthVec;
}

float PCA_recognize(Mat matrixA, Mat matrixU, Mat matrixF){
    matrixA.convertTo(matrixA, CV_32FC1);
    Mat om1 = Mat::zeros(2, 1, matrixA.type());
    Mat om2 = Mat::zeros(2, 1, matrixA.type());
    Mat om3 = Mat::zeros(2, 1, matrixA.type());
    Mat omF = Mat::zeros(2, 1, matrixA.type());
    //Mat om1, om2, om3, omF;

    //cout << matrixF << endl;

    for(int i =0; i< matrixA.rows; i++){
        om1.at<float>(0,0) +=  matrixU.at<float>(0,i)*matrixA.at<float>(i,0);
        om1.at<float>(1,0) +=  matrixU.at<float>(1,i)*matrixA.at<float>(i,0);

        om2.at<float>(0,0) +=  matrixU.at<float>(0,i)*matrixA.at<float>(i,1);
        om2.at<float>(1,0) +=  matrixU.at<float>(1,i)*matrixA.at<float>(i,1);

        om3.at<float>(0,0) +=  matrixU.at<float>(0,i)*matrixA.at<float>(i,2);
        om3.at<float>(1,0) +=  matrixU.at<float>(1,i)*matrixA.at<float>(i,2);

        omF.at<float>(0,0) +=  matrixU.at<float>(0,i)*matrixF.at<float>(i,0);
        omF.at<float>(1,0) +=  matrixU.at<float>(1,i)*matrixF.at<float>(i,0);
    }







    //tru sai so
    om1 = omF- om1;
    om2 = omF- om2;
    om3 = omF- om3;



    float l_om1, l_om2, l_om3, l_omF;
    l_om1 = length_Vec(om1);
    l_om2 = length_Vec(om2);
    l_om3 = length_Vec(om3);
    l_omF = length_Vec(omF);


//    cout << l_om1 << endl;
//    cout << l_om2 << endl;
//    cout << l_om3 << endl;



    float min_errors = l_om3;
    int max = 14000;
    int min = 9000;
    int mau = (max - min)/100;
    float percent =  (100 - ((min_errors - min)/mau));

    if (percent > 100) {percent = 100;}
    else if (percent < 0) {percent = 0;}
    return percent;
}
float PCA_algorithm(Mat matrix1, Mat matrix2, Mat matrix3, Mat matrix4){

    matrix1.convertTo(matrix1, CV_32FC1);
    matrix2.convertTo(matrix2, CV_32FC1);
    matrix3.convertTo(matrix3, CV_32FC1);
    matrix4.convertTo(matrix4, CV_32FC1);

    int max_rows = matrix1.rows*matrix1.cols;
    Mat average_matrix = Mat::zeros(max_rows, 1, matrix1.type());
    //cout <<max_rows << endl;

    for (int i = 0; i < max_rows; i++){
        average_matrix.at<float>(i,0) = (matrix1.at<float>(i,0) + matrix2.at<float>(i,0) + matrix3.at<float>(i,0))/3;
    }
    //cout << average_matrix.size << endl;
    Mat err1 = Mat::zeros(max_rows, 1, matrix1.type());
    Mat err2 = Mat::zeros(max_rows, 1, matrix1.type());
    Mat err3 = Mat::zeros(max_rows, 1, matrix1.type());

    //tinh errors
    for (int i = 0; i < average_matrix.rows; i++){
        err1.at<float>(i,0)= matrix1.at<float>(i,0) - average_matrix.at<float>(i,0);
        err2.at<float>(i,0)= matrix2.at<float>(i,0) - average_matrix.at<float>(i,0);
        err3.at<float>(i,0)= matrix3.at<float>(i,0) - average_matrix.at<float>(i,0);
        matrix4.at<float>(i,0)= matrix4.at<float>(i,0) - average_matrix.at<float>(i,0);
    }


    Mat matrixA =  Mat::zeros(max_rows, 3, matrix1.type()); // matrix lien hiep
    matrixA.convertTo(matrixA, CV_32FC1);
    for(int i = 0; i< max_rows; i++){

        matrixA.at<float>(i,0) = err1.at<float>(i,0);
        matrixA.at<float>(i,1) = err2.at<float>(i,0);
        matrixA.at<float>(i,2) = err3.at<float>(i,0);
    }
    cout<< "test"<< endl;
    //matrixA.convertTo(matrixA, CV_32FC1);
    //cout << average_matrix << endl;

    Mat matrixAT =  transit_matrix(matrixA);
    //matrixAT.convertTo(matrixAT, CV_32FC1);

    Mat Cov = matrixAT*matrixA;

    //cout<< Cov <<endl;
    PCA pca_cov(Cov, Mat(),1);
    Mat eigenval = pca_cov.eigenvalues.clone();
    Mat eigenvec = pca_cov.eigenvectors.clone();
    //cout<< eigenvec <<endl;
    //cout<< eigenval <<endl;
    Mat matrixu1 = matrixA*eigenvec.row(0).t();
    Mat matrixu2 = matrixA*eigenvec.row(1).t();
    Mat matrixU = Mat::zeros(matrixu1.rows, 2, matrixu1.type());

    float length_matrixu1, length_matrixu2;
    length_matrixu1 = length_Vec(matrixu1);
    length_matrixu2 = length_Vec(matrixu2);
    matrixu1 = matrixu1/length_matrixu1;
    matrixu2 = matrixu2/length_matrixu2;
    //cout << eigenvec << endl;
    //cout << matrixA << endl;
    for(int i =0; i< matrixu1.rows; i++){
        matrixU.at<float>(i,0) = matrixu1.at<float>(i,0);
        matrixU.at<float>(i,1) = matrixu2.at<float>(i,0);
    }
    matrixU =  matrixU.t();
    //cout << matrixF << endl;

    float min_errors = PCA_recognize(matrixA, matrixU, matrix4);
    return min_errors;
}

int main() {

    int c = 0;
    for (int i =1; i<40; i+=3){

    string pathx = "/home/banvien/Downloads/00.Resource_cropped/xxx.jpg";
    string pathz = "/home/banvien/Downloads/00.Resource_cropped/zzz.jpg";

    //path image
    string path3 = "/home/banvien/Downloads/00.Resource_cropped/" +to_string(i)+ ".jpg";
    string path4 = "/home/banvien/Downloads/00.Resource_cropped/" +to_string(i+1)+ ".jpg";
    string path5 = "/home/banvien/Downloads/00.Resource_cropped/" +to_string(i+2)+ ".jpg";

    Mat img1 = imread(pathx);
    Mat img2 = imread(pathz);
    Mat img3 = Mat::zeros(200, 160, img1.type());;
    Mat img4 = Mat::zeros(200, 160, img1.type());;;

    for (int j =0; j<3; j++){

        if(j == 0){

             img3 = imread(path3);
             img4 = imread(path4);
        }
        else if(j==1){
             img3 = imread(path3);
             img4 = imread(path5);
        }
        else if(j ==2){
             img3 = imread(path4);
             img4 = imread(path5);
        }


//    Mat img1 = Mat::ones(200, 160, img3.type());
//    Mat img2 = Mat::ones(200, 160, img3.type());
//    resize(img1,img1,Size(),0.5,0.5);
    resize(img1,img1,Size(200,160));
    resize(img2,img2,Size(img1.cols,img1.rows));
    resize(img3,img3,Size(img1.cols,img1.rows));
    resize(img4,img4,Size(img1.cols,img1.rows));
    //img4 = trans.Transform(img4);

    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));

    Mat imgGray1, imgBlur1, imgCanny1, imgDil1, imgErode1;
    cvtColor(img1, imgGray1, COLOR_BGR2GRAY);
    GaussianBlur(imgGray1, imgBlur1, Size(7, 7), 5, 0);
    Canny(imgBlur1, imgCanny1, 25,75);
    dilate(imgCanny1, imgDil1, kernel);
    erode(imgDil1, imgErode1, kernel);

    Mat imgGray2, imgBlur2, imgCanny2, imgDil2, imgErode2;
    cvtColor(img2, imgGray2, COLOR_BGR2GRAY);
    GaussianBlur(imgGray2, imgBlur2, Size(7, 7), 5, 0);
    Canny(imgBlur2, imgCanny2, 25,75);
    dilate(imgCanny2, imgDil2, kernel);
    erode(imgDil2, imgErode2, kernel);

    Mat imgGray3, imgBlur3, imgCanny3, imgDil3, imgErode3;
    cvtColor(img3, imgGray3, COLOR_BGR2GRAY);
    GaussianBlur(imgGray3, imgBlur3, Size(7, 7), 5, 0);
    Canny(imgBlur3, imgCanny3, 25,75);
    dilate(imgCanny3, imgDil3, kernel);
    erode(imgDil3, imgErode3, kernel);

    Mat imgGray4, imgBlur4, imgCanny4, imgDil4, imgErode4;
    cvtColor(img4, imgGray4, COLOR_BGR2GRAY);
    GaussianBlur(imgGray4, imgBlur4, Size(7, 7), 5, 0);
    Canny(imgBlur4, imgCanny4, 25,75);
    dilate(imgCanny4, imgDil4, kernel);
    erode(imgDil4, imgErode4, kernel);


    Mat result = Mat::zeros(imgErode4.rows,imgErode4.cols, imgErode4.type());

    cv::compare(imgErode3 , imgErode4  , result , cv::CMP_EQ );


    imshow("Image1", imgErode1);
    imshow("Image2", imgErode2);
    imshow("Image3", imgErode3);
    imshow("Image4", imgErode4);

    Mat matrix1, matrix2, matrix3, matrixF; // matrixF: matrix need to recognize
    matrix1 = PCA_image_position(imgErode1);
    matrix2 = PCA_image_position(imgErode2);
    matrix3 = PCA_image_position(imgErode3);
    matrixF = PCA_image_position(imgErode4);



    float percent = PCA_algorithm(matrix1, matrix2, matrix3, matrixF);

    if(percent>=90){
        c++;
    }
    cout << i << endl;
    cout << percent <<"%"<< endl;


    waitKey(100);
        }
    }
    cout << c << endl;
}
