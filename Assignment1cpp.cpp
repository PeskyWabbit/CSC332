#include "pch.h"
#include "Assignment1cpp.h"
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <stdio.h>
#include <string>
#include <fstream>
#include <iomanip>
#include <windows.h>
#include <vector>

using namespace std;

vector<int> sortedArray(int arr[100]);
void getStats(string filename1, string filename2);
void compareAndWriteToTxt();

int values[100][2];

int main() {
	// open file to write data to
	ofstream outData;
	outData.open("Brute_Force_Results.csv", ios::app);

	// headers
	outData << "Number 1, Number 2, GCD, Time Spent(NanoSeconds)" << endl;

	// nested loop to fill each value of array with random number
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 2; j++) {
			values[i][j] = rand();
		}
	} 

	// Loop to iteratre through all array elements and calculate the GCD while tracking time
	for (int i = 0; i < 100; i++) {
		int num1 = values[i][0];
		int num2 = values[i][1];
		typedef std::chrono::high_resolution_clock Time;
		typedef std::chrono::nanoseconds ns;
		typedef std::chrono::duration<float> fsec;

		// get start time
		auto t0 = Time::now();
		int gcd = calculateGcd(num1, num2);
		// get end time
		auto t1 = Time::now();

		// getting execution duration
		fsec fs = t1 - t0;
		ns d = std::chrono::duration_cast<ns>(fs);

		cout << "BRUTE FORCE - GCD of " << num1 << " and " << num2 << ": " << gcd << " found in: " << d.count() << endl;

		// write to excel
		outData << num1 << ", " << num2 << ", " << gcd << ", " << d.count() << endl;
	}

	outData.close();
	getStats("Brute_Force_Results.csv", "Brute_Force_Stats.csv");


	outData.open("Original_Euclid_Results.csv");

	// headers
	outData << "Number 1, Number 2, GCD, Time Spent(NanoSeconds)" << endl;

	// Loop through all those same values but run euclids alg on them instead of brute force
	for (int i = 0; i < 100; i++) {
		int num1 = values[i][0];
		int num2 = values[i][1];
		typedef std::chrono::high_resolution_clock Time;
		typedef std::chrono::nanoseconds ns;
		typedef std::chrono::duration<float> fsec;

		// get start time
		auto t0 = Time::now();
		int gcd = euclids1(num1, num2);
		// get end time
		auto t1 = Time::now();

		// getting execution duration
		fsec fs = t1 - t0;
		ns d = std::chrono::duration_cast<ns>(fs);

		cout << "EUCLIDS - GCD of " << num1 << " and " << num2 << ": " << gcd << " found in: " << d.count() << endl;

		// write to excel
		outData << num1 << ", " << num2 << ", " << gcd << ", " << d.count() << endl;
	}
	outData.close();
	getStats("Original_Euclid_Results.csv", "Original_Euclid_Stats.csv");


	outData.open("Improved_Euclid_Results.csv");

	// headers
	outData << "Number 1, Number 2, GCD, Time Spent(NanoSeconds)" << endl;

	// Loop through all those same values but run improved euclids alg on them instead of original euclids
	for (int i = 0; i < 100; i++) {
		int num1 = values[i][0];
		int num2 = values[i][1];
		typedef std::chrono::high_resolution_clock Time;
		typedef std::chrono::nanoseconds ns;
		typedef std::chrono::duration<float> fsec;

		if (num1 < num2) {
			int temp = num1;
			num1 = num2;
			num2 = temp;
		}

		// get start time
		auto t0 = Time::now();
		int gcd = euclids2(num1, num2);
		// get end time
		auto t1 = Time::now();

		// getting execution duration
		fsec fs = t1 - t0;
		ns d = std::chrono::duration_cast<ns>(fs);

		cout << "EUCLIDS 2 - GCD of " << num1 << " and " << num2 << ": " << gcd << " found in: " << d.count() << endl;

		// write to excel
		outData << num1 << ", " << num2 << ", " << gcd << ", " << d.count() << endl;
	}
	outData.close();
	getStats("Improved_Euclid_Results.csv", "Improved_Euclid_Stats.csv");

	compareAndWriteToTxt();
}

void getStats(string filename1, string filename2){
	string num1 = "";
	string num2 = "";
	string gcd = "";
	string time = "";
	int times[100];
	int i = 0;

	ifstream inData;
	inData.open(filename1);

	// iterate over the header row
	getline(inData, num1, ',');
	getline(inData, num2, ',');
	getline(inData, gcd, ',');
	getline(inData, time, '\n');

	// get the rest of the lines values, but only keep the time value
	for(int i=0; i < 100; i++){
		getline(inData, num1, ',');
		getline(inData, num2, ',');
		getline(inData, gcd, ',');
		getline(inData, time, '\n');
		times[i] = std::stoi(time);
	}
	inData.close();

	// find min, max
	ofstream outData;
	outData.open(filename2);
	int max = 0;
	int min = 99999;
	int total = 0;
	double average = 0.0;

	for (int i : times) {
		if (i > max) {
			max = i;
		}
		else if (i < min) {
			min = i;
		}
		total += i;
	}

	// find avg
	average = total / 100;
	outData << "Statistics, Milliseconds" << endl;
	outData << "Maximum time, " << max << endl;
	outData << "Minimum time, " << min << endl;
	outData << "Average time, " << average << endl;

	// sort array and find median
	vector<int> sortedTimes = sortedArray(times);
	outData << "Median time, " << sortedTimes.at(49) << endl;
	outData.close();
}

// brute force gcd
int calculateGcd(int num1, int num2) {
	// every integer is divisible by so this is default
	int gcd = 1;

	// iterate through all integers and update gcd when both are evenly divided by that int. keeps highest value in gcd var
	if (num1 > num2) {
		for (int i=1; i<num2; i++) {
			if (num1 % i == 0 && num2 % i == 0) {
				gcd = i;
			}
		}
	}
	else {
		for (int i = 1; i < num1; i++) {
			if (num1 % i == 0 && num2 % i == 0) {
				gcd = i;
			}
		}
	}
	return gcd;
}


// bubble sort because I'm lazy
vector<int> sortedArray(int arr[100]) {
	vector<int> timeVec;
	int temp = 0;
	for (int j = 0; j < 99; j++) {
		for (int i = 0; i < 99; i++) {
			if (arr[i] > arr[i + 1]) {
				int temp = arr[i];
				int int2 = arr[i + 1];
				arr[i] = int2;
				arr[i + 1] = temp;
			}
		}
	}

	// add values to vector to return and print so I can see what's going on
	for (int i = 0; i < 100; i++) {
		timeVec.push_back(arr[i]);
	}
	return timeVec;
}

// original version
int euclids1(int a, int b) {
	int gcd = 0;
	int q = 0;
	int r = 1;

	if (a == 0 || b == 0)
		return gcd;

	else {
		while (r != 0){
			q = (a / b);
			r = a - (q * b);
			a = b;
			b = r;
		}
		gcd = a;
	}
	return gcd;
}

// improved version
int euclids2(int a, int b) {
	if(a == 0 || b == 0) {
		return 1;
	}

	int r = 1;
	while (r != 0) {
		r = a - b;
		if (r > b) {
			r = r - b;
			if (r > b) {
				r = r - b;
				if (r > b) {
					r = a - (b * (a / b));
				}
			}
		}
		a = b;
		b = r;
	}
	return a;
}

// get relevant data needed to create Conclusions.txt. 
// Then create Conclusions.txt, do the math, and add in the required sentences comparing values.
void compareAndWriteToTxt() {
	ifstream bruteForce;
	ifstream euclid1;
	ifstream euclid2;
	bruteForce.open("Brute_Force_Results.csv");

	vector<int> bfTimes;
	vector<int> euclid1Times;
	vector<int> euclid2Times;
	string num1, num2, gcd, time;

	// iterate over the header row
	getline(bruteForce, num1, ',');
	getline(bruteForce, num2, ',');
	getline(bruteForce, gcd, ',');
	getline(bruteForce, time, '\n');

	// get the rest of the lines values, but only keep the time value
	for (int i = 0; i < 100; i++) {
		getline(bruteForce, num1, ',');
		getline(bruteForce, num2, ',');
		getline(bruteForce, gcd, ',');
		getline(bruteForce, time, '\n');
		bfTimes.push_back(std::stoi(time));
	}
	bruteForce.close();

	euclid1.open("Original_Euclid_Results.csv");
	// iterate over the header row
	getline(euclid1, num1, ',');
	getline(euclid1, num2, ',');
	getline(euclid1, gcd, ',');
	getline(euclid1, time, '\n');

	// get the rest of the lines values, but only keep the time value
	for (int i = 0; i < 100; i++) {
		getline(euclid1, num1, ',');
		getline(euclid1, num2, ',');
		getline(euclid1, gcd, ',');
		getline(euclid1, time, '\n');
		euclid1Times.push_back(std::stoi(time));
	}

	euclid2.open("Improved_Euclid_Results.csv");
	// iterate over the header row
	getline(euclid2, num1, ',');
	getline(euclid2, num2, ',');
	getline(euclid2, gcd, ',');
	getline(euclid2, time, '\n');

	// get the rest of the lines values, but only keep the time value
	for (int i = 0; i < 100; i++) {
		getline(euclid2, num1, ',');
		getline(euclid2, num2, ',');
		getline(euclid2, gcd, ',');
		getline(euclid2, time, '\n');
		euclid2Times.push_back(std::stoi(time));
	}

	euclid2.close();

	// print statements to see whats going on
	for (int i : bfTimes) {
		cout << "BF: " << i << endl;
	}
	for (int i : euclid1Times) {
		cout << "EC1: " << i << endl;
	}
	for (int i : euclid2Times) {
		cout << "EC2: " << i << endl;
	}

	ofstream outData;
	outData.open("Conclusions.txt");

	// comparing all cases asked for in assignment sheet
	vector<int> diff1;
	vector<int> diff2;
	vector<int> diff3;
	int total1 = 0;
	int total2 = 0;
	int total3 = 0;
	int count1 = 0;
	int count2 = 0;
	int count3 = 0;

	//iterate through each set of times and keep those that are matching the performance standards
	for (int i = 0; i < 100; i++) {
		if (bfTimes.at(i) > euclid1Times.at(i)) {
			diff1.push_back(euclid1Times.at(i) - bfTimes.at(i));
			total1 = total1 + (bfTimes.at(i) - euclid1Times.at(i));
			count1++;
		}

		if (bfTimes.at(i) > euclid2Times.at(i)) {
			diff2.push_back(euclid1Times.at(i) - bfTimes.at(i));
			total2 = total2 + (bfTimes.at(i) - euclid2Times.at(i));
			count2++;
		}

		if (euclid1Times.at(i) > euclid2Times.at(i)) {
			diff3.push_back(euclid1Times.at(i) - bfTimes.at(i));
			total3 = total3 + (euclid1Times.at(i) - euclid2Times.at(i));
			count3++;
		}

	}

	// calc averages improvements
	int avg1 = total1 / count1;
	int avg2 = total2 / count2;
	int avg3 = total3 / count3;

	outData << "Out of 100 pairs of integers, the original version of Euclid outperformed brute-force in " << count1 << " pairs; and the average saved time for these " << count1 << " pairs of integers was " << avg1 << " nanoseconds." << endl << endl;
	outData << "Out of 100 pairs of integers, the improved version of Euclid outperformed brute-force in " << count2 << " pairs; and the average saved time for these " << count2 << " pairs of integers was " << avg2 << " nanoseconds." << endl << endl;
	outData << "Out of 100 pairs of integers, the improved version of Euclid outperformed the original version of Euclid in " << count3 << " pairs; and the average saved time for these " << count3 << " pairs of integers was " << avg3 << " nanoseconds." << endl << endl;
	outData.close();
}


