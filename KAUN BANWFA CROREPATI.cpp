#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

struct Question {
    string question;
    vector<string> options;
    char correctAnswer;
    int difficulty;
};

vector<Question> questions = {
    {"What is the capital of India?", {"A. Mumbai", "B. Delhi", "C. Kolkata", "D. Chennai"}, 'B', 1},
    {"Which planet is known as the Red Planet?", {"A. Venus", "B. Mars", "C. Jupiter", "D. Saturn"}, 'B', 1},
    {"Who wrote the Indian National Anthem?", {"A. Tagore", "B. Gandhi", "C. Nehru", "D. Patel"}, 'A', 2},
    {"What is the currency of Japan?", {"A. Yuan", "B. Won", "C. Yen", "D. Dollar"}, 'C', 2},
    {"Which is the largest mammal?", {"A. Elephant", "B. Blue Whale", "C. Giraffe", "D. Rhino"}, 'B', 3}
};

vector<int> prizeMoney = {
    1000, 2000, 5000, 10000, 20000, 40000, 80000, 160000, 320000, 640000,
    1250000, 2500000, 5000000, 10000000, 70000000
};

void displayPrizeChart(int currentLevel) {
    cout << "\nCurrent Prize Money: ₹" << prizeMoney[currentLevel] << endl;
    if(currentLevel > 0) cout << "Safe Haven: ₹" << prizeMoney[currentLevel-1] << endl;
}

bool useFiftyFifty(Question &q) {
    vector<string> newOptions;
    newOptions.push_back(q.options[q.correctAnswer - 'A']);
    
    vector<int> wrongOptions;
    for(int i = 0; i < 4; i++) {
        if(i != (q.correctAnswer - 'A')) wrongOptions.push_back(i);
    }
    
    random_shuffle(wrongOptions.begin(), wrongOptions.end());
    newOptions.push_back(q.options[wrongOptions[0]]);
    
    sort(newOptions.begin(), newOptions.end());
    
    cout << "\nOptions after 50-50:\n";
    for(auto &opt : newOptions) cout << opt << endl;
    return true;
}

bool usePhoneAFriend(Question &q) {
    cout << "\nFriend says: I think the answer might be " << q.correctAnswer << endl;
    return true;
}

int main() {
    srand(time(0));
    int currentQuestion = 0;
    int totalQuestions = questions.size();
    bool fiftyUsed = false;
    bool phoneUsed = false;

    cout << "Welcome to Kaun Banega Crorepati!\n";
    
    while(currentQuestion < totalQuestions) {
        Question q = questions[currentQuestion];
        cout << "\nQuestion " << (currentQuestion+1) << ": " << q.question << endl;
        for(string option : q.options) cout << option << endl;

        // Display lifelines
        cout << "\nLifelines: ";
        if(!fiftyUsed) cout << "1. 50-50 ";
        if(!phoneUsed) cout << "2. Phone a Friend";
        cout << endl;

        cout << "Enter your answer (A-D) or 9 to quit, 0 for lifelines: ";
        char choice;
        cin >> choice;

        if(choice == '9') {
            cout << "\nYou chose to quit! You win ₹" << prizeMoney[currentQuestion-1] << endl;
            break;
        }
        
        if(choice == '0') {
            if(fiftyUsed && phoneUsed) {
                cout << "No lifelines remaining!\n";
                continue;
            }
            
            cout << "Choose lifeline: ";
            int lifeline;
            cin >> lifeline;
            
            if(lifeline == 1 && !fiftyUsed) {
                fiftyUsed = useFiftyFifty(q);
            }
            else if(lifeline == 2 && !phoneUsed) {
                phoneUsed = usePhoneAFriend(q);
            }
            else {
                cout << "Invalid lifeline choice!\n";
            }
            continue;
        }

        if(toupper(choice) == q.correctAnswer) {
            cout << "\nCorrect Answer!\n";
            displayPrizeChart(currentQuestion);
            currentQuestion++;
            
            if(currentQuestion == totalQuestions) {
                cout << "\nCONGRATULATIONS! You've won ₹7 Crore!\n";
            }
        }
        else {
            cout << "\nWrong Answer! Game Over!\n";
            if(currentQuestion > 0) {
                cout << "You win ₹" << prizeMoney[currentQuestion-1] << endl;
            }
            else {
                cout << "You win nothing!\n";
            }
            break;
        }
    }

    return 0;
}