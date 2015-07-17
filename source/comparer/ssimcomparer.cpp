#include "ssimcomparer.h"

SSIMComparer::SSIMComparer()
{
}

void SSIMComparer::compare()
{
    position = 0;
    result.images.clear();
    result.images.push_back(getCurrentImage());

    QTime timer;
    timer.start();
    while (hasNextTwo())
    {
        notifyProgress("Images compared: " + QString::number(position)
                                   + " (time: " + QString::number(timer.elapsed()/1000) + "s)");

        double ssimIndex = ssimCalculate(getNext().toStdString().c_str(), getNext().toStdString().c_str());

        if (ssimIndex < threshold)
        {
            result.images.push_back(getNextImage());
        }
    }

    result.images.push_back(getCurrentImage());

    notifyResult(result);
}

double SSIMComparer::ssimCalculate(const char* argv1, const char* argv2)
{
        double C1 = 6.5025;
        double C2 = 58.5225;

        IplImage
            *img1=NULL, *img2=NULL, *img1_img2=NULL,
            *img1_temp=NULL, *img2_temp=NULL,
            *img1_sq=NULL, *img2_sq=NULL,
            *mu1=NULL, *mu2=NULL,
            *mu1_sq=NULL, *mu2_sq=NULL, *mu1_mu2=NULL,
            *sigma1_sq=NULL, *sigma2_sq=NULL, *sigma12=NULL,
            *ssim_map=NULL, *temp1=NULL, *temp2=NULL, *temp3=NULL;

        img1_temp = cvLoadImage(argv1);
        img2_temp = cvLoadImage(argv2);

        if(img1_temp==NULL || img2_temp==NULL)
            return -1;

        int x=img1_temp->width, y=img1_temp->height;
        int nChan=img1_temp->nChannels, d=IPL_DEPTH_32F;
        CvSize size = cvSize(x, y);

        img1 = cvCreateImage( size, d, nChan);
        img2 = cvCreateImage( size, d, nChan);

        cvConvert(img1_temp, img1);
        cvConvert(img2_temp, img2);
        cvReleaseImage(&img1_temp);
        cvReleaseImage(&img2_temp);

        img1_sq = cvCreateImage( size, d, nChan);
        img2_sq = cvCreateImage( size, d, nChan);
        img1_img2 = cvCreateImage( size, d, nChan);

        cvPow( img1, img1_sq, 2 );
        cvPow( img2, img2_sq, 2 );
        cvMul( img1, img2, img1_img2, 1 );

        mu1 = cvCreateImage( size, d, nChan);
        mu2 = cvCreateImage( size, d, nChan);

        mu1_sq = cvCreateImage( size, d, nChan);
        mu2_sq = cvCreateImage( size, d, nChan);
        mu1_mu2 = cvCreateImage( size, d, nChan);


        sigma1_sq = cvCreateImage( size, d, nChan);
        sigma2_sq = cvCreateImage( size, d, nChan);
        sigma12 = cvCreateImage( size, d, nChan);

        temp1 = cvCreateImage( size, d, nChan);
        temp2 = cvCreateImage( size, d, nChan);
        temp3 = cvCreateImage( size, d, nChan);

        ssim_map = cvCreateImage( size, d, nChan);

        cvSmooth( img1, mu1, CV_GAUSSIAN, 11, 11, 1.5 );
        cvSmooth( img2, mu2, CV_GAUSSIAN, 11, 11, 1.5 );

        cvPow( mu1, mu1_sq, 2 );
        cvPow( mu2, mu2_sq, 2 );
        cvMul( mu1, mu2, mu1_mu2, 1 );

        cvSmooth( img1_sq, sigma1_sq, CV_GAUSSIAN, 11, 11, 1.5 );
        cvAddWeighted( sigma1_sq, 1, mu1_sq, -1, 0, sigma1_sq );

        cvSmooth( img2_sq, sigma2_sq, CV_GAUSSIAN, 11, 11, 1.5 );
        cvAddWeighted( sigma2_sq, 1, mu2_sq, -1, 0, sigma2_sq );

        cvSmooth( img1_img2, sigma12, CV_GAUSSIAN, 11, 11, 1.5 );
        cvAddWeighted( sigma12, 1, mu1_mu2, -1, 0, sigma12 );

        cvScale( mu1_mu2, temp1, 2 );
        cvAddS( temp1, cvScalarAll(C1), temp1 );

        cvScale( sigma12, temp2, 2 );
        cvAddS( temp2, cvScalarAll(C2), temp2 );

        cvMul( temp1, temp2, temp3, 1 );

        cvAdd( mu1_sq, mu2_sq, temp1 );
        cvAddS( temp1, cvScalarAll(C1), temp1 );

        cvAdd( sigma1_sq, sigma2_sq, temp2 );
        cvAddS( temp2, cvScalarAll(C2), temp2 );

        cvMul( temp1, temp2, temp1, 1 );

        cvDiv( temp3, temp1, ssim_map, 1 );


        CvScalar index_scalar = cvAvg( ssim_map );

        cvReleaseImage(&img1);
        cvReleaseImage(&img2);
        cvReleaseImage(&img1_sq);
        cvReleaseImage(&img2_sq);
        cvReleaseImage(&img1_img2);
        cvReleaseImage(&mu1);
        cvReleaseImage(&mu2);
        cvReleaseImage(&mu1_sq);
        cvReleaseImage(&mu2_sq);
        cvReleaseImage(&mu1_mu2);
        cvReleaseImage(&sigma1_sq);
        cvReleaseImage(&sigma2_sq);
        cvReleaseImage(&sigma12);
        cvReleaseImage(&ssim_map);
        cvReleaseImage(&temp1);
        cvReleaseImage(&temp2);
        cvReleaseImage(&temp3);
        return (index_scalar.val[2] + index_scalar.val[1] + index_scalar.val[0]) / 3.;
}
