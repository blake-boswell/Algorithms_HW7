#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <algorithm>

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
        std::ostringstream s;
        s << "(" << value << ":: " << start << " - " << end << ")";
        std::string activity(s.str());
        return activity;
    }
};

void writeOutput(int maxValue, vector<int> activityList, string outFile) {
    ofstream file(outFile.c_str());
    if (file.is_open()) {
        file << maxValue << "\n";
        for (int i = 0; i < activityList.size(); i++) {
            file << activityList[i];
            if (i < activityList.size() - 1) {
                file << " ";
            }
        }
        file.close();
    }
}

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
    int current = actNum;
    actNum--;
    while (actNum >= 0) {
        if (activities[actNum].end <= activities[current].start) {
            return actNum;
        } 
        actNum--;
    }
    return -1;
}

/**
 * Using DP array maxValues[0..actNum] find the max value activity of the vector up to activity i
 * Strategy: Check for the max value we can get from activities 0..i as the max value with current activity
 * Check for the max value we can get from activities 0..i-1 as the max value without the current activity
 * maxValues[i] will represent the max value we can get with the interval activity 0..i
 * idList vector holds the sequence of id numbers to get maxValues[i]
 * 
 * Complexity: O(n^2)
 * Calls fn: findNextBest w/ complexity O(n) n times 
 */
int findMaxValueDP(Activity activities[], int numActivities, string outFile) {

    if (numActivities < 1) {
        vector<int> emptyList(0);
        writeOutput(0, emptyList, outFile);
        return -1;
    }
    
    // Need the extra value because our base case takes up a slot
    int maxValSize = numActivities + 1;
    int maxValues[numActivities];
    vector< vector<int> > idList(numActivities);
    // Set base case
    maxValues[0] = activities[0].value;
    idList[0].push_back(activities[0].id);
    // Find the max value from 0..i as the with current value (including this new activity)
    // Find the max value from 0..i-1 as the without current value (if we were not to include the current activity)
    // Set maxValues[i] to max of the two values
    for (int i = 1; i < numActivities; i++) {
        int valueWithCurrent = activities[i].value;
        vector<int> idListWithCurrent;

        idListWithCurrent.push_back(activities[i].id);
        int indexOfNextBestJob = findNextBest(activities, i);

        if (indexOfNextBestJob != -1) {
            valueWithCurrent += maxValues[indexOfNextBestJob];
            for (int j = 0; j < idList[indexOfNextBestJob].size(); j++) {
                idListWithCurrent.push_back(idList[indexOfNextBestJob][j]);
            }
        }

        int valueWithoutCurrent = maxValues[i - 1];
        idList[i] = idList[i - 1];
        if (valueWithCurrent > valueWithoutCurrent) {
            idList[i] = idListWithCurrent;
        }
        maxValues[i] = max(valueWithCurrent, valueWithoutCurrent);
    }
    
    vector<int> activityList(idList[numActivities - 1]);
    // sort the sequence in increasing order
    sort(activityList.begin(), activityList.end());

    writeOutput(maxValues[numActivities - 1], activityList, outFile);

    return maxValues[numActivities - 1];
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

int getShavedSize(Activity activities[], int numActivities, int interval) {
    int newSize = numActivities;
    for (int i = numActivities; i >= 0; i--) {
        if (activities[i].end > interval) {
            newSize--;
        }
    }
    return newSize;
}

int main(int argc, char** argv) {
    // Init values
    Activity* activities;
    fstream file;
    string filename = "input.txt";
    string outFile = "output.txt";
    if (argc > 1) {
        filename = argv[1];
    }
    if (argc > 2) {
        outFile = argv[2];
    }
    file.open(filename.c_str());
    int numActivities, intervalEnd;
    // Read input
    if (file.is_open()) {
        cout << "Output written to " << outFile << endl;
        file >> numActivities >> intervalEnd;
        activities = new Activity[numActivities];
        int id, start, end, value;
        for (int i = 0; i < numActivities; i++) {
            file >> id >> start >> end >> value;
            activities[i].set(id, start, end, value);
        }
        file.close();

        // Sort by end time
        sort(activities, activities + numActivities);

        // Shave off activities outside the interval
        int newSize = getShavedSize(activities, numActivities, intervalEnd);

        findMaxValueDP(activities, newSize, outFile);

    } else {
        cout << "Could not find file " << filename << ". Please enter the name of an input file as a parameter." << endl;
    }

    return 0;
}