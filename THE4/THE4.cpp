
//Author: Zeynep Taskin 33851
//Date: 25.08.2024 
//Purpose: This program processes a file through the use of multithreading.

#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <mutex>
using namespace std;

struct District {//structure that holds the necessary district information 
	int id, medianHouseAge, medianPrice;//district information
	double avgRoomCount, avgBedroomCount, latitude, longitude;
	District(int i = 0, int mha = 0, int mp = 0, double arc = 0.1, double abc = 0.1, double lat = 0.1, double lon = 0.1) {//constructor
		id = i;
		medianHouseAge = mha;
		medianPrice = mp;
		avgRoomCount = arc;
		avgBedroomCount = abc;
		latitude = lat;
		longitude = lon;
	}
};

struct LowestDistricts {//structure that holds the necessary district and dissimmilarity score information 
	District lowDistrict;
	double dissimScore;
	LowestDistricts(District ld = District(), double s = 0.1) {//constructor
		lowDistrict = ld;
		dissimScore = s;
	}
};


template <class type>class DynQueue {//dynamic queue class to store any kind of districts
	struct QueueNode {//structure representing a node in the queue
		type value;//district type it holds
		QueueNode* next;//pointer to the next node
		QueueNode(type& p, QueueNode* n = nullptr) {//constructor
			value = p;
			next = n;
		}
	};

private:
	QueueNode* front;// pointer to the first node of the queue
	QueueNode* rear;// pointer to the last node of the queue
public:
	DynQueue();//constructor
	~DynQueue();//destructor
	void operator << (type);//overloads "<<" operator to enqueue any type
	void operator >> (type&);//overloads ">>" operator to dequeue any type
	bool isEmpty(void) const;//checks if the queue is empty
	void clear(void);//clears the queue
};

//DynQueue.cpp

template <class type>DynQueue<type>::DynQueue() {//constructor that initializes the queue empty
	front = NULL;
	rear = NULL;

}

template <class type>DynQueue<type>::~DynQueue() {//destructor that clears the queue
	clear();

}

template <class type>void DynQueue<type>::operator << (type rhs) {//overloads "<<" operator to enqueue any type into the queue
	if (isEmpty()) {//if the queue is empty
		//make it the first element
		front = new QueueNode(rhs);
		rear = front;
	}
	else {//if the queue is not empty
		//add it after rear
		rear->next = new QueueNode(rhs);
		rear = rear->next;
	}
}

template <class type>void DynQueue<type>::operator >> (type& rhs) {//overloads ">>" operator to denqueue any type into the queue

	QueueNode* temp;
	if (isEmpty()) {//check if the queue is empty
		cout << "Attempting to dequeue on empty queue, exiting program...\n";
		exit(1);
	}
	else {
		rhs = front->value;//copy the type values on to rhs
		temp = front;//point to the front
		front = front->next;//update the front
		delete temp;//delete the previous front
	}

}

template <class type>bool DynQueue<type>::isEmpty(void) const {//checks if the queue is empty
	if (front == NULL)//check if front pointer is null which means queue is empty
		return true;
	else
		return false;
}


template <class type>void DynQueue<type>::clear(void) {//clears the queue by dequeueing all type objects
	type value;
	while (!isEmpty())
		*this >> value; //delete all elements
}

//main.cpp
double dissimilarityScore(District& queueDis, District& inputDis) {//function to calculate the dissimilarity score of a district
	int ageDiff = abs(queueDis.medianHouseAge - inputDis.medianHouseAge);
	double roomDiff = abs(queueDis.avgRoomCount - inputDis.avgRoomCount);
	double bedroomDiff = abs(queueDis.avgBedroomCount - inputDis.avgBedroomCount);
	double latDiff = abs(queueDis.latitude - inputDis.latitude);
	double lonDiff = abs(queueDis.longitude - inputDis.longitude);
	double score = sqrt( pow((ageDiff / 26.0), 2)+ pow((roomDiff / 10.0), 2)+ pow((bedroomDiff / 4.0), 2)+ pow(latDiff, 2)+ pow(lonDiff, 2));

	return score;
}

int findIndexOfMax(vector<LowestDistricts> ld) {//function to find the maximum dissimilarity score's index in a vector that holds three districts
	int maxIdx=0;
	if (ld[0].dissimScore > ld[1].dissimScore && ld[0].dissimScore>ld[2].dissimScore) {//compare each score with the other two score
		maxIdx = 0;
	}else if (ld[1].dissimScore > ld[0].dissimScore && ld[1].dissimScore > ld[2].dissimScore) {
		maxIdx = 1;
	}else if (ld[2].dissimScore > ld[1].dissimScore && ld[2].dissimScore > ld[0].dissimScore) {
		maxIdx = 2;
	}
	return maxIdx;//return the index
}

DynQueue<LowestDistricts> resultQueue;//the queue that holds the thread's findings
mutex queueMutex;//mutex to control the acces to myQueue to prevent data races
mutex consoleMutex;//mutex to control the access to cout to avoid concurrent writes from multiple threads
mutex resultMutex;//mutex to control the access to result queue to prevent data races


//thread function
void threadFunc(DynQueue<District>& myQueue, District inpDistrict, int threadCount) {
	vector<LowestDistricts> lowDistrictVector(3); // vector to hold the three lowest districts
	//initializing vector with arbitrary high scores to ensure that any valid district's score will be lower
	lowDistrictVector[0] = LowestDistricts(District(), 101.5);
	lowDistrictVector[1] = LowestDistricts(District(), 100.3);
	lowDistrictVector[2] = LowestDistricts(District(), 102.1);
	int thTime = 0;//counter for how many times a thread processed a district
	
	while (!myQueue.isEmpty()) {//until queue is empty

		thTime++;
		District district;
		queueMutex.lock();// lock the queue to ensure only this thread can access or modify it
		if (!myQueue.isEmpty()) {//check if queue is empty before dequeuing to avoid dequeueing on an empty queue due to context switch happening
			myQueue >> district;//dequeue a district
		}
		else {
			queueMutex.unlock();
			break;
		}
		queueMutex.unlock();
		double score = dissimilarityScore(district, inpDistrict);//get the districts dissimilarity score
		int maxIndex = findIndexOfMax(lowDistrictVector);//find the max score's index in the low district vector

		if (score < lowDistrictVector[maxIndex].dissimScore) {//compare the highest score in vector and the districts score
			lowDistrictVector[maxIndex] = LowestDistricts(district, score);//update the vector
		}

		if (thTime % 1000 == 0) {// every 1000 items processed 

			consoleMutex.lock();// lock console access to avoid simultaneous output
			cout << "Thread with id " << this_thread::get_id() << " is processing its " << thTime << "th item." << endl;//print the progress
			consoleMutex.unlock();
			
		}
	}
	consoleMutex.lock();// lock console access to avoid simultaneous output
	cout << "Thread with id " << this_thread::get_id() << " is exiting." << endl;//print to show this thread exited
	consoleMutex.unlock();

	resultMutex.lock();// lock access to result queue to avoid concurrent modifications
	for (int i = 0; i < 3; i++) {
		resultQueue << lowDistrictVector[i];//enqueue the found districts and their dissimmilarity score to the result queue
	}
	resultMutex.unlock();
}

vector<LowestDistricts> findIndexOfLowestThree(int threadCount) {//function to find the three district with lowest dissimilarity scores
	vector<LowestDistricts> compareVector(3);//create a vector to hold the districts and their scores
	//initializing vector with arbitrary high scores to ensure that any valid district's score will be lower
	compareVector[0] = LowestDistricts(District(), 100.5);
	compareVector[1] = LowestDistricts(District(), 100.2);
	compareVector[2] = LowestDistricts(District(), 100.1);
	for (int i = 0; i < 3 * threadCount; i++) {//compare all of the districts thread's foundings
		LowestDistricts lowD;
		resultQueue >> lowD;//dequeue from the result vector
		int highIndex = findIndexOfMax(compareVector);
		if (lowD.dissimScore < compareVector[highIndex].dissimScore) {//compare the highest score in vector and the districts score
			compareVector[highIndex] = lowD;
		}
	}
	return compareVector;//return the vector that holds the three district that have the lowest scores
}

int findIndexOfLow(vector<LowestDistricts> ld) {//function to find the lowest dissimilarity score 
	int lowIndex = 0;
	if (ld[0].dissimScore < ld[1].dissimScore && ld[0].dissimScore < ld[2].dissimScore) {//compare each score with the other two score
		lowIndex = 0;
	}
	else if (ld[1].dissimScore < ld[0].dissimScore && ld[1].dissimScore < ld[2].dissimScore) {
		lowIndex = 1;
	}
	else if (ld[2].dissimScore < ld[1].dissimScore && ld[2].dissimScore < ld[0].dissimScore) {
		lowIndex = 2;
	}
	return lowIndex;
}

int main() {
	fstream dataFile;
	dataFile.open("data.tsv");//open the data.tvs file(file that has the input)
	//declare all headers of the data that file holds
	int numOfThreads, houseAge;
	double  numOfRooms, numOfBedrooms, latitude, longitude;

	if (!dataFile.fail()) {//if file succesfully opens
		string header;
		getline(dataFile, header);//read the header line to discard it
		cout << "20640 districts have been loaded." << endl;
		cout << "How many threads?: ";
		cin >> numOfThreads;//get the number of threads
		cout << endl;
		cout << "What is the age of the house?: ";
		cin >> houseAge;//get the age of the house input
		cout << "How many rooms does the house have?: ";
		cin >> numOfRooms;//get the number of rooms of the house input
		cout << "How many bedrooms does the house have?: ";
		cin >> numOfBedrooms;//get the number of bedrooms of the house input
		cout << "What is the latitude of the house?: ";
		cin >> latitude;//get the latitute of the house input
		cout << "What is the longitude of the house?: ";
		cin >> longitude;//get the longitude of the house input
		cout << endl;

		District inputDistrict(0, houseAge, 0, numOfRooms, numOfBedrooms, latitude, longitude);//create a district with the given input
		DynQueue<District> districtQueue;//create the queue to hold the districts in the file

		while (!dataFile.eof()) {//until file ends
			string districtData;
			getline(dataFile, districtData);//read a line from the file
			istringstream is(districtData);//turn the line into an input string stream to get the data from the line
			int id, medAge, medPrice;
			double avgRoom, avgBedroom, lat, lon;
			is >> id >> medAge >> avgRoom >> avgBedroom >> lat >> lon>> medPrice;//turn the data in the line into appropriate type
			District district(id, medAge, medPrice, avgRoom, avgBedroom, lat, lon);//create a district with the line's data
			districtQueue << district;//enqueue the district
		}

		vector<thread> threadVec;//vector to hold the created threads
		for (int i = 0; i < numOfThreads; i++) {
			threadVec.push_back(thread(&threadFunc, ref(districtQueue), inputDistrict, numOfThreads));//create the wanted number of threads
		}

		for (int i = numOfThreads - 1; i > -1; i--) {//make every thread join
			threadVec[i].join();
		}

		vector<LowestDistricts> last3DistrictVector = findIndexOfLowestThree(numOfThreads);//find the three district with the lowest score
		cout << endl << "Closest three districts in terms of features have the following median prices:" << endl << endl;//print the founded district
		for (int i = 0; i < 3; i++) {
			int index=findIndexOfLow(last3DistrictVector);//find the lowest district with the dissimilarity score to print in a increasing order

			cout << "Closest District " <<i+1<< endl;
			cout << "--------" << endl;
			cout << "Id: " << last3DistrictVector[index].lowDistrict.id <<endl;
			cout << "Median Price : " << last3DistrictVector[index].lowDistrict.medianPrice << endl;
			cout << "Calculated Dissimilarity Metric : " << last3DistrictVector[index].dissimScore << endl;
			cout << endl;
			last3DistrictVector[index].dissimScore = 10;//change the dissimilarity score of the printed district to ensure that district not printing again
		}
		//calculate the average median price of the three districts 
		int avgMed = (last3DistrictVector[0].lowDistrict.medianPrice + last3DistrictVector[1].lowDistrict.medianPrice + last3DistrictVector[2].lowDistrict.medianPrice) / 3;

		cout << "Average median price of three closest districts: " << avgMed<<endl;//print the average median of the three districts
	}
	return 0;
}
