#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

struct Activity {
    int id;
    int start;
    int end;
    int value;
    Activity() {
        this->id = -1;
        this->start = -1;
        this->end = -1;
        this->value = -1;
    }
    Activity(int id, int start, int end, int value) {
        this->id = id;
        this->start = start;
        this->end = end;
        this->value = value;
    }

    void set(int id, int start, int end, int value) {
        this->id = id;
        this->start = start;
        this->end = end;
        this->value = value;
    }

    bool operator < (const Activity& b) const {
        return (end < b.end);
    }
    string to_string() {
        string activity = "("  + std::to_string(value) + 
            ":: " + std::to_string(start) + " - " + std::to_string(end) + ")";
        return activity;
    }
};

/**
 * Find the latest job before the current job that doesn't have conflicting times
 * with the current job
 * 
 * @return: the index of the activity
 * 
 * Complexity: O(n)
 * 
 */
int findNextBest(Activity activities[], int actNum) {
    cout << "\t\tFinding best value job for " << activities[actNum].to_string() << endl;
    int current = actNum;
    actNum--;
    while (actNum >= 0) {
        if (activities[actNum].end <= activities[current].start) {
            cout << "\t\tFound best value: " << activities[actNum].to_string() << endl;
            return actNum;
        } 
        actNum--;
    }
    cout << "\t\tCouldn't find another." << endl;
    return -1;
}

/**
 * Using DP array maxValues[0..actNum] find the max value activity of the vector up to activity i
 * Strategy: Check for the max value we can get from activities 0..i as the max value with current activity
 * Check for the max value we can get from activities 0..i-1 as the max value without the current activity
 * maxValues[i] will represent the max value we can get with the interval activity 0..i
 * 
 * Complexity: O(n^2)
 * Calls fn: findNextBest w/ complexity O(n) n times 
 */
int findMaxValueDP(Activity activities[], int numActivities) {
    
    // Need the extra value because our base case takes up a slot
    int maxValSize = numActivities + 1;
    int maxValues[maxValSize];
    string idList[maxValSize];
    // Set base case
    maxValues[0] = activities[0].value;
    idList[0] = to_string(activities[0].id);
    // Find the max value from 0..i as the with current value (including this new activity)
    // Find the max value from 0..i-1 as the without current value (if we were not to include the current activity)
    // Set maxValues[i] to max of the two values
    for (int i = 1; i < maxValSize; i++) {
        int valueWithCurrent = activities[i].value;
        string idListWithCurrent = to_string(activities[i].id);
        int indexOfNextBestJob = findNextBest(activities, i);
        if (indexOfNextBestJob != -1) {
            valueWithCurrent += maxValues[indexOfNextBestJob];
            idListWithCurrent += " " + idList[indexOfNextBestJob];
        }

        int valueWithoutCurrent = maxValues[i - 1];
        cout << "Value with: " << valueWithCurrent << endl;
        cout << "Value w/out: " << valueWithoutCurrent << endl;
        idList[i] = idList[i - 1];
        if (valueWithCurrent > valueWithoutCurrent) {
            idList[i] = idListWithCurrent;
        }
        maxValues[i] = max(valueWithCurrent, valueWithoutCurrent);
    }
    cout << "DP matrix" << endl;
    for (int i = 0; i < maxValSize; i++) {
        cout << maxValues[i] << " ";
    }
    cout << endl;

    cout << "ID LIST\n";
    for (int i = 0; i < numActivities; i++) {
        cout << idList[i] << endl;
    }

    string sequence = idList[maxValSize - 1];
    cout << "\nSequence\n" << sequence << endl;

    // for (int i = 0; i < sequence.size(); i++) {

    // }

    return maxValues[maxValSize - 1];
}

/**
 * Recursively find the max value activity of the vector that doesn't conflict
 * 
 * Worst case: If findNextBest returns the index of the value before, we will calculate it 2 times
 * If this happens all n times we get O(n*2^n), which is horrible...
 * 
 * Solution: store in array to access it in theta(1) after it is already computed
 * 
 * Complexity: O(n*2^n) -> O(n^2) using findMaxValueDP ^^
 */
int findMaxValue(Activity activities[], int actNum) {
    // base case: there are no jobs that occur before current
    if (actNum == 0) {
        return activities[actNum].value;
    }

    int valueWithCurrent = activities[actNum].value;
    int indexOfNextBestJob = findNextBest(activities, actNum);
    if (indexOfNextBestJob != -1) {
        valueWithCurrent += findMaxValue(activities, indexOfNextBestJob);
    }
    
    int valueWithoutCurrent = findMaxValue(activities, actNum - 1);
    cout << "[Value w/out Current]: " << valueWithoutCurrent << endl;
    cout << "\t[Value w/ Current]: " << valueWithCurrent << endl;
    return max(valueWithCurrent, valueWithCurrent);
}

void printActivityChart(Activity activities[], int numActivities) {
    cout << "ID";
    for (int i = 0; i < numActivities; i++) {
        cout << "|" << activities[i].id << "\t";
    }
    cout << endl;

    cout << "S ";
    for (int i = 0; i < numActivities; i++) {
        cout << "|" << activities[i].start << "\t";
    }
    cout << endl;

    cout << "E ";
    for (int i = 0; i < numActivities; i++) {
        cout << "|" << activities[i].end << "\t";
    }
    cout << endl;

    cout << "V ";
    for (int i = 0; i < numActivities; i++) {
        cout << "|" << activities[i].value << "\t";
    }
    cout << endl;
}

int main(int argc, char** argv) {
    // Init values
    Activity* activities;
    fstream file;
    file.open("input1.txt", ios::in);
    int numActivities, intervalEnd;
    if (file.is_open()) {
        file >> numActivities >> intervalEnd;
        activities = new Activity[numActivities];
        int id, start, end, value;
        for (int i = 0; i < numActivities; i++) {
            file >> id >> start >> end >> value;
            activities[i].set(id, start, end, value);
        }
        file.close();
    }

    printActivityChart(activities, numActivities);

    // Sort by end time
    sort(activities, activities + numActivities);
    cout << "\nSORTED\n";

    printActivityChart(activities, numActivities);

    int maxValueDP = findMaxValueDP(activities, numActivities - 1);
    cout << "MAX DP: " << maxValueDP << endl;

    

    return 0;
}