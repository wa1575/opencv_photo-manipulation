#include<stdio.h>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

string szNameWindow1 = "A"; //윈도우 이름 설정 
string szNameWindow2 = "B";
float sx = 0, sy = 0, ex = 0, ey = 0;
int bMouseDown = 0;
Mat img_A, img_B; //이미지 저장 변수 
void MyMerge();
void onMouse(int event, int mx, int my, int flag, void* param);

int main() {
	img_A = imread("img1.jpg");
	img_B = imread("img2.jpg");
	resize(img_B, img_B, Size(img_A.cols, img_A.rows)); //새로운 사진 resize 

	imshow(szNameWindow1, img_A); //결과 이미지 표시 
	setMouseCallback(szNameWindow1, onMouse, &img_A);

	imshow(szNameWindow2, img_B); //결과 이미지 표시 

	waitKey(0);
}

void MyMerge() {
	bitwise_or(img_A, img_B, img_B);
}

void onMouse(int event, int mx, int my, int flag, void* param) {
	Mat img_tmp = img_A.clone();
	Mat mask = Mat::zeros(img_A.rows, img_A.cols, CV_8UC3); //타원 밖에 요소를 없애기 위함 
	RotatedRect rect;

	switch (event) {
	case EVENT_LBUTTONDOWN: //첫 클릭
		sx = (float)mx; //save the start x, y 
		sy = (float)my; 
		bMouseDown = 1; //mouse flag on
		break;
	case EVENT_MOUSEMOVE: //드래그 시
		if (bMouseDown == 1) {
			ex = (float)mx; //update ex, ey 
			ey = (float)my;
			rect = RotatedRect(Point2f((sx + ex) / 2, (sy + ey) / 2), Size(abs(ex - sx), abs(ey - sy)), 0);
			ellipse(img_tmp, rect, Scalar(0, 0, 0), 2); 
			imshow(szNameWindow1, img_tmp); //show the ecllipse 
		}
		break;
	case EVENT_LBUTTONUP: // 드래그 종료 
		ex = (float)mx;
		ey = (float)my;
		rect = RotatedRect(Point2f((sx + ex) / 2, (sy + ey) / 2), Size(abs(ex - sx), abs(ey - sy)), 0);
		ellipse(mask, rect, Scalar(255, 255, 255), -1); //타원은 흰색(255, 255, 255)으로 그림 
		//mask 부분을 and 합니다. 흰 부분을 and 하므로 타원 영역만 남고 나머지는 없앱니다. 
		bitwise_and(mask, img_A, img_A); //mask와 bitwise_and 연산 처리한 후 img_A에 다시 저장
		//하얀색에서 현재 mask를 빼주면 0 -> 255, 255 -> 0으로 출력됨
		bitwise_and(Scalar(255, 255, 255) - mask, img_B, img_B);//mask와 bitwise_and 연산 처리한 후 img_B에 저장 
		imshow(szNameWindow1, img_A); //원 이미지 화면 출력 
		bMouseDown = 0; //mouse flag off 
		//검은색 영역은 원 이미지가 그대로 남고 나머지가 바뀜 
		MyMerge(); //두 이미지를 합성
		imshow(szNameWindow2, img_B); //이미지 출력 
		break;
	}
}
