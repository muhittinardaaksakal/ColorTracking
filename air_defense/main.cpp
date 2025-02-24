#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

int main() {
    cv::VideoCapture cap(0);
    cv::Scalar lower_red1(0, 100, 100);
    cv::Scalar upper_red1(10, 255, 255);
    cv::Scalar lower_red2(170, 100, 100);
    cv::Scalar upper_red2(180, 255, 255);


    if (!cap.isOpened()) {
        std::cerr << "Kamera açılamadı. Kameranızın bağlı ve çalışır durumda olduğundan emin olun." << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            std::cerr << "Kare okunamadı!" << std::endl;
            break;
        }

        cv::resize(frame, frame, cv::Size(1090, 600));

        cv::Mat hsv;
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

        cv::Mat mask_red1;
        cv::Mat mask_red2;
        cv::Mat combined_mask;
        cv::inRange(hsv, lower_red1, upper_red1, mask_red1);
        cv::inRange(hsv, lower_red2, upper_red2, mask_red2);

        //combined_mask = mask_red1 | mask_red2;

        // Normlde kırmızı HSV çarkında 2 kısımda bulunduğu için üstteki combined_mask de ikisini bitwise or luyoruz pythondaki gibi ama
        // şahsin düşüncem mask_red2 renk aralığının kırmızı tespiti için daha iyi olduğu yönünde yukarıyı silmiyorum istersen öyle test edersin.

        combined_mask = mask_red2;

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(combined_mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        for (std::vector<cv::Point> contour : contours) {
            if (cv::contourArea(contour) > 100) {
                cv::Rect bounding_box = cv::boundingRect(contour);
                cv::rectangle(frame, bounding_box, cv::Scalar(0, 255, 0), 2);
            }
        }

        cv::imshow("Gerçek Görüntü", frame);
        cv::imshow("Maskelenmiş Görüntü", combined_mask); // Renk ayarını yapmanı kolaylaştırır.

        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
