#include"HandLog.h"

HandLog::HandLog(){
	errorWeight = 0;
	for (int finger = 0; finger < 5; finger++){
		fingersMean.push_back(cv::Point(0, 0));
	}
}

void HandLog::setFingers(std::vector<cv::Point> fingers, cv::Point centroid){
	if (fingers.size()>5)//指の数がおかしい
		return;

	this->centroid = centroid;

	struct fingPos{
		double dist;
		int pointNum;
		int finger;
		bool operator<(fingPos p){
			return this->dist < p.dist;
		}
	};

	fingers = sort(&fingers);
	//各点を重心からの相対座標に変更
	for (int i = 0; i < fingers.size(); i++){
		fingers[i].x -= centroid.x;
		fingers[i].y -= centroid.y;
	}

	//各点につき各指の履歴との距離を求める
	std::vector<fingPos> may;
	for (int i = 0; i < fingers.size();i++){
		for (int finger = 0; finger < 5; finger++){
			if (UsePoints::distance(fingers[i], fingersMean[finger])<errorDistance){
				may.push_back({ UsePoints::distance(fingers[i], fingersMean[finger]), i, finger });
			}
		}
	}

	//距離でソート
	std::sort(may.begin(), may.end());

	boolean eFinger[] = { false, false, false, false, false };
	for (int i = thumb; i <= little; i++){
		existFingers[i] = false;
	}
	//距離最小の物から結びつけていき、logに保存
	std::vector<int> usedPoint;
	for (fingPos p:may){
		if (!eFinger[p.finger]){
			bool used = false;
			for (int i:usedPoint){
				if (i==p.pointNum){
					used = true;
					break;
				}
			}
			if (!used){
				fingersLog[p.finger].push_back(fingers[p.pointNum]);
				existFingers[p.finger] = true;
				eFinger[p.finger] = true;
				usedPoint.push_back(p.pointNum);
				//fingersMean[p.finger].x = (UsePoints::meanPoint(fingersLog[p.finger]).x + fingers[p.pointNum].x)/2;
				//fingersMean[p.finger].y = (UsePoints::meanPoint(fingersLog[p.finger]).y + fingers[p.pointNum].y)/2;

				if (fingersLog[p.finger].size() > logSize){
					fingersLog[p.finger].erase(fingersLog[p.finger].begin());
				}
			}
		}
	}
	//平均を更新
	for (int finger = 0; finger < 5; finger++){
		if (fingersLog[finger].size()>0)
			fingersMean[finger] = UsePoints::meanPoint(fingersLog[finger]);
	}

	fingersMean = sort(&fingersMean);
}

std::vector<cv::Point> HandLog::sort(std::vector<cv::Point>* points){
	cv::Point p = points->at(points->size()-1);
	points->pop_back();
	std::vector<cv::Point> temp1;
	std::vector<cv::Point> temp2;
	for (std::vector<cv::Point>::iterator it = points->begin(); it != points->end(); it++){
		if (it->x > p.x)
			temp2.push_back(*it);
		else
			temp1.push_back(*it);
	}
	if (temp1.size() > 1){
		temp1=sort(&temp1);
	}
	temp1.push_back(p);
	if (temp2.size() > 0){
		if (temp2.size() > 1){
			temp2 = sort(&temp2);
		}
		temp1.insert(temp1.end(), temp2.begin(), temp2.end());
	}
	return temp1;
}


std::vector<cv::Point> HandLog::getFingers(){
	return fingersMean;
}

POINT HandLog::getCentroid(){
	POINT p;
	p.x = centroid.x;
	p.y = centroid.y;
	return p;
}

bool* HandLog::getExist(){
	return existFingers;
}
