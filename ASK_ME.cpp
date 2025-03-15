#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <map>
#include <utility>
#include <fstream>
#include <exception>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <limits>

using namespace std;

// helper function to parse the string to bool
bool toInt(string s)
{
    return s == "1";
}

class User
{
private:
    int id;
    string name;
    string email;
    string password;
    string user_name;
    bool allow_anonymous_questions;
    vector<int> questions_received_ids;
    vector<int> questions_sent_ids;
    map<int, vector<int>> questions_id_thread_ids; // for thread questions

public:
    User() = default;
    User(string name, string email, string password, bool allow_anonymous_questions, string user_name) : name(name), email(email),
                                                                                                         password(password), allow_anonymous_questions(allow_anonymous_questions), user_name(user_name) {}

    int getID() const
    {
        return id;
    }
    void setID(int id)
    {
        this->id = id;
    }
    const string &getName() const
    {
        return name;
    }
    void setName(const string &name)
    {
        this->name = name;
    }
    const string &getEmail() const
    {
        return email;
    }
    void setEmail(const string &email)
    {
        this->email = email;
    }
    bool getAnonymousStatus() const
    {
        return allow_anonymous_questions;
    }
    void setAnonymousStatus(bool allow_anonymous_questions)
    {
        this->allow_anonymous_questions = allow_anonymous_questions;
    }
    void setUserName(const string &user_name)
    {
        this->user_name = user_name;
    }
    const string &getUserName() const
    {
        return user_name;
    }
    void setPassword(const string &password)
    {
        this->password = password;
    }
    const string &getPassword() const
    {
        return password;
    }

    const vector<int> &getQuestionsSentIds() const
    {
        return questions_sent_ids;
    }

    void setQuestionsSentIds(const vector<int> &questions_sent_ids)
    {
        this->questions_sent_ids = questions_sent_ids;
    }

    const vector<int> &getQuestionsReceivedIds() const
    {
        return questions_received_ids;
    }
    void setQuestionsReceivedIds(const vector<int> &questions_received_ids)
    {
        this->questions_received_ids = questions_received_ids;
    }
    void readUser()
    {
        string str;
        cout << "Enter user name (no spaces): ";
        cin >> str;
        setUserName(str);
        cout << "Enter password: ";
        cin >> str;
        setPassword(str);
        cout << "Enter name: ";
        cin >> str;
        setName(str);
        cout << "Enter email: ";
        cin >> str;
        setEmail(str);
        cout << "Allow anonymous questions? (0 or 1): ";
        cin >> str;
        setAnonymousStatus(toInt(str));
    }

    void printUser()
    {
        cout << "ID: " << getID() << "\tName: " << getName() << "\n";
    }
};

class Question
{
private:
    int id{};
    string question{};
    string answer{};
    int user_id{};
    bool anonymous{};

public:
    Question() = default;
    Question(int id) : id(id) {}

    void setQuestion(const string &question)
    {
        this->question = question;
    }

    const string &getQuestion() const
    {
        return question;
    }

    void setID(int id)
    {
        this->id = id;
    }

    int getID() const
    {
        return id;
    }

    bool getAnonymousStatus() const
    {
        return anonymous;
    }

    void setAnonymousStatus(bool anonymous)
    {
        this->anonymous = anonymous;
    }

    const string &getAnswer() const
    {
        return answer;
    }

    void setAnswer(const string &answer)
    {
        this->answer = answer;
    }

    int getUserID() const
    {
        return user_id;
    }
    void setUserID(int user_id)
    {
        this->user_id = user_id;
    }
    void printQuestion()
    {
        cout << "Question id (" << id << ") ";
        if (!anonymous)
            cout << "from user (" << user_id << "):";
        cout << "\nQuestion: " << question << "\n";
        cout << "Answer: " << (answer == "" ? "Not Answered Yet" : answer) << "\n\n";
    }
};

class QuestionsManager
{
private:
    map<int, Question> question;
    map<int, vector<Question>> questions_threads;
    static int question_id;

public:
    QuestionsManager() = default;

    void printAllQuestions()
    {
        auto it = question.begin();
        while (it != question.end())
        {
            printQuestionFull(it->first);
            it++;
        }
    }
    Question &searchQuestion(int id)
    {
        return question[id];
    }

    int addQuestionToDatabase(int anoynmous, int user_id, int id = -1)
    {
        Question q;
        int temp = question_id;
        q.setID(question_id);
        cout << "Enter Question Text: ";
        string Q;
        cin.ignore();
        getline(cin, Q);
        q.setQuestion(Q);
        q.setAnonymousStatus(anoynmous);
        q.setUserID(user_id);
        if (id != -1)
            questions_threads[id].push_back(q);
        question[question_id++] = q;
        return temp;
    }

    void printQuestionFull(int id)
    {
        question[id].printQuestion();
        for (int i = 0; i < int(questions_threads[id].size()); i++)
        {
            cout << "Thread: ";
            questions_threads[id][i].printQuestion();
        }
    }

    void delete_question_from_databse(int id)
    {
        question.erase(question.find(id));
    }
};

int QuestionsManager::question_id = 1;

class usersManager
{
private:
    vector<User> users;
    static int id;
    QuestionsManager question_manager;

public:
    usersManager() = default;
    void Login_signup()
    {
        cout << "1: Login\n";
        cout << "2: Signup\n";
        cout << "Enter 1 or 2: ";
        int num;
        cin >> num;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input please try again\n";
        }
        else if (num != 1 && num != 2)
            cout << "invalid number, to Log in Enter 1, to Sign up Enter 2\n";
        else if (num == 1)
            doLogin();
        else
            doSignUp();
        Login_signup();
    }

    void doLogin()
    {
        string user_name;
        string password;
        cout << "Enter user name & password: ";
        cin >> user_name >> password;
        for (auto &user : users)
        {
            if (user.getUserName() == user_name && user.getPassword() == password)
            {
                userMenu(user);
                return;
            }
        }
        cout << "User not found\n";
        return;
    }

    void userMenu(User &user)
    {
        cout << "1: Print Questions To Me\n";
        cout << "2: Print Questions From Me\n";
        cout << "3: Answer Question\n";
        cout << "4: Delete Question\n";
        cout << "5: Ask Question\n";
        cout << "6: List system User\n";
        cout << "7: Feed\n";
        cout << "8: Logout\n";
        int num;
        cout << "Enter a number in range 1 - 8: ";
        cin >> num;
        if (num < 1 || num > 8)
            cout << "ERROR: Invalid number ... try again\n\n";
        else if (num == 1)
            printQuestionsToMe(user);
        else if (num == 2)
            printQuestionsFromMe(user);
        else if (num == 3)
            answerQuestion(user);
        else if (num == 4)
            deleteQuestion(user);
        else if (num == 5)
            askQuestion(user);
        else if (num == 6)
            listSystemUsers();
        else if (num == 7)
            printFeed();
        else if (num == 8)
            return;
        userMenu(user);
    }

    void printFeed()
    {
        question_manager.printAllQuestions();
    }

    void listSystemUsers()
    {
        for (auto &user : users)
            user.printUser();
    }

    void askQuestion(User &user)
    {
        cout << "Enter user ID or -1 to cancel: ";
        int user_id;
        cin >> user_id;
        if (user_id == -1)
            return;
        User *receiver = nullptr;
        for (auto &it : users)
        {
            if (it.getID() == user_id)
            {
                receiver = &it;
                break;
            }
        }
        if (!receiver)
        {
            cout << "User Not found\n";
            return;
        }
        else
        {
            bool anonymous = receiver->getAnonymousStatus();
            if (!anonymous)
                cout << "Note: Anonymous questions aren't allowed for this user\n";
            bool flag = false;
            if (anonymous)
            {
                cout << "Choose if you want to ask anonymous Question (0 or 1)\n";
                cin >> flag;
            }
            cout << "For thread question: Enter question ID or -1 for new question: ";
            int num;
            cin >> num;
            int temp_id;
            if (num == -1)
                temp_id = question_manager.addQuestionToDatabase(flag, user.getID());
            else
                temp_id = question_manager.addQuestionToDatabase(flag, user.getID(), num);
            vector<int> temp = user.getQuestionsSentIds();
            temp.push_back(temp_id);
            user.setQuestionsSentIds(temp);
            temp = receiver->getQuestionsReceivedIds();
            temp.push_back(temp_id);
            receiver->setQuestionsReceivedIds(temp);
        }
    }

    void deleteQuestion(User &user)
    {
        cout << "Enter Question ID or -1 to cancel: ";
        int curr_id;
        cin >> curr_id;
        if (curr_id == -1)
            return;
        vector<int> questions_received_ids = user.getQuestionsReceivedIds();
        if (find(questions_received_ids.begin(), questions_received_ids.end(), curr_id) == questions_received_ids.end())
        {
            cout << "This question id is not found for this user\n";
            return;
        }
        Question &q = question_manager.searchQuestion(curr_id);
        int user_sender_id = q.getUserID();
        User *u = nullptr;
        for (auto &tmp : users) {
            if (tmp.getID() == user_sender_id)
                u = &tmp;
        }
        // delete question from database and synchronizing for sender and receiver
        vector<int> questions_sent_by_sender = u->getQuestionsSentIds();
        questions_sent_by_sender.erase(find(questions_sent_by_sender.begin(), questions_sent_by_sender.end(), curr_id));
        u->setQuestionsSentIds(questions_sent_by_sender);
        auto id_to_remove = find(questions_received_ids.begin(), questions_received_ids.end(), curr_id);
        questions_received_ids.erase(id_to_remove);
        user.setQuestionsReceivedIds(questions_received_ids);
        question_manager.delete_question_from_databse(curr_id);
    }

    void answerQuestion(User &user)
    {
        cout << "Enter Question id or -1 to Cancel: ";
        int id;
        cin >> id;
        if (id == -1)
            return;
        vector<int> user_questions_ids = user.getQuestionsReceivedIds();
        auto it = find(user_questions_ids.begin(), user_questions_ids.end(), id);
        if (it == user_questions_ids.end())
        {
            cout << "The question ID not found ... please try again\n";
            return;
        }
        else
        {
            Question &q = question_manager.searchQuestion(id);
            string ans;
            q.printQuestion();
            if (q.getAnswer() != "")
                cout << "Warning: Already answered. Answer will be updated\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, ans);
            q.setAnswer(ans);
        }
    }

    void printQuestionsFromMe(User &user)
    {
        vector<int> questions = user.getQuestionsSentIds();
        for (auto &question_id : questions)
            question_manager.printQuestionFull(question_id);
    }

    void printQuestionsToMe(User &user)
    {
        vector<int> questions = user.getQuestionsReceivedIds();
        for (auto &question_id : questions)
            question_manager.printQuestionFull(question_id);
    }

    void doSignUp()
    {
        User user_;
        user_.readUser();
        user_.setID(usersManager::id++);
        users.push_back(user_);
    }
};

int usersManager::id = 1;

class AskMeSystem
{
private:
    usersManager user_manager;

public:
    AskMeSystem() = default;
    void run()
    {
        user_manager.Login_signup();
    }
};

// dummy class to test while i am coding
class WhiteBoxTester
{
private:
    User user;

public:
    WhiteBoxTester() = default;
    void testToInt()
    {
        string s = "1";
        assert(toInt(s) == true);
        s = "0";
        assert(toInt(s) == false);
    }
};

int main()
{
    AskMeSystem().run();
    return 0;
}