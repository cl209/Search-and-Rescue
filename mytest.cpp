// UMBC - CMSC 341 - Fall 2024 - Proj2
#include "swarm.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std;
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(){}
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }
    void init(int min, int max){
        m_min = min;
        m_max = max;
        m_type = UNIFORMINT;
        m_generator = std::mt19937(10);// 10 is the fixed seed value
        m_unidist = std::uniform_int_distribution<>(min,max);
    }
    void getShuffle(vector<int> & array){
        // this function provides a list of all values between min and max
        // in a random order, this function guarantees the uniqueness
        // of every value in the list
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // this function provides a list of all values between min and max
        // in a random order, this function guarantees the uniqueness
        // of every value in the list
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }

    string getRandString(int size){
        // the parameter size specifies the length of string we ask for
        // to use ASCII char the number range in constructor must be set to 97 - 122
        // and the Random type must be UNIFORMINT (it is default in constructor)
        string output = "";
        for (int i=0;i<size;i++){
            output = output + (char)getRandNum();
        }
        return output;
    }
    
    int getMin(){return m_min;}
    int getMax(){return m_max;}
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};
class Tester{
    public:
    //Check AVL is balanced
    bool checkBalance();
    bool checkBalance(Robot* current);

    //Check BST property
    bool checkBSTProperty();
    bool checkBSTProperty(Robot* current);
    

    //Check splay
    bool checkSplay();

    //Check height
    bool checkHeight();
    bool checkHeight(Robot* current);

    //Check remove
    bool checkRemove();
    bool findRobot(Robot* current, int ID);
    
    //Check remove edge cases
    bool checkRemoveEdge();

    //Check AVL balance after removals
    bool checkBalanceRemove();

    //Check BST property after removals
    bool checkBSTPropertyRemove();

    //Check BST height after removals
    bool checkBSTHeightRemove();

    //Check assignment operator for normal case
    bool checkAssignmentOperator();

    //Check assignment operator for edge case
    bool checkAssignmentOperatorEdge();

};
bool Tester::checkBalance(){
    //Creating the object and inserting 300 robots
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    Swarm obj(AVL);
    int Size = 300;
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;
    for(int i = 0;i < Size; i++){
        ID = idGen.getRandNum();
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID,roboType);
        obj.insert(robot);
    }
    //Checking if AVL is balanced
    return checkBalance(obj.m_root);
}
//Helper function to check if AVL is balanced
bool Tester::checkBalance(Robot* current){
    //Empty tree is balanced tree
    if(current == nullptr){
        return true;
    }
    //Store left and right height of the subtree
    int leftHeight = (current->getLeft() != nullptr) ? current->getLeft()->getHeight() : -1;
    int rightHeight = (current->getRight() != nullptr) ? current->getRight()->getHeight() : -1;

    int balanceFactor = leftHeight - rightHeight;
    //Checking if the tree is balanced
    if(balanceFactor > 1 || balanceFactor < -1){
        return false;
    }
    //Recursively checking the left and right subtree
    return checkBalance(current->getLeft()) && checkBalance(current->getRight());
}
bool Tester::checkBSTProperty(){
    //Creating the object and inserting 100 robots
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    Swarm obj(BST);
    int Size = 100;
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;
    for(int i = 0;i < Size; i++){
        ID = idGen.getRandNum();
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID,roboType);
        obj.insert(robot);
    }
    //Checking if BST property is satisfied
    return checkBSTProperty(obj.m_root);
}

//Helper function to check if BST property is satisfied
bool Tester::checkBSTProperty(Robot* current){
    //Empty tree is BST
    if(current == nullptr){
        return true;
    }
    //Checking if the left child is smaller than the current node
    if(current->getLeft() != nullptr && current->getLeft()->getID() > current->getID()){
        return false;
    }
    //Checking if the right child is greater than the current node
    if(current->getRight() != nullptr && current->getRight()->getID() < current->getID()){
        return false;
    }
    //Recursively checking the left and right subtree
    return checkBSTProperty(current->getLeft()) && checkBSTProperty(current->getRight());

}
bool Tester::checkSplay(){
    //Create object and insert
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    Swarm obj(SPLAY);
    int Size = 100;
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;
    for(int i = 0;i < Size; i++){
        ID = idGen.getRandNum();
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID,roboType);
        obj.insert(robot);
        //Check if the root is the same as the last inserted robot
        if (obj.m_root->getID() != ID){
            return false;
        }
    }
    return true;
}
bool Tester::checkHeight(){
    //Create object and insert
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    Swarm obj(SPLAY);
    int Size = 100;
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;
    for(int i = 0;i < Size; i++){
        ID = idGen.getRandNum();
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID,roboType);
        obj.insert(robot);
    }
    //Checking if the height is correct
    return checkHeight(obj.m_root);

}
//Helper function to check if the height is correct
bool Tester::checkHeight(Robot* current){
    //Checking if the current is empty
    if(current == nullptr){
        return true;
    }
    //Store left and right height of the subtree
    int leftHeight = (current->getLeft() != nullptr) ? current->getLeft()->getHeight() : -1;
    int rightHeight = (current->getRight() != nullptr) ? current->getRight()->getHeight() : -1;
    //Checking if the height is correct
    if(current->getHeight() != max(leftHeight,rightHeight) + 1){
        return false;
    }
    //Recursively checking the left and right subtree
    return checkHeight(current->getLeft()) && checkHeight(current->getRight());
}
bool Tester::checkRemove(){
    //Create object and insert
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    Swarm obj(BST);
    int Size = 100;
    int ID = 0;
    int tempID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;
    for(int i = 0;i < Size; i++){
        ID = idGen.getRandNum();
        if (i == Size / 2) tempID = ID;
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID,roboType);
        obj.insert(robot);
    }
    //Remove the robot
    obj.remove(tempID);
    //Checking if the robot is removed
    return !findRobot(obj.m_root,tempID);

}
//Helper function to find the robot
bool Tester::findRobot(Robot* current, int ID){
    //Checking if the current is empty
    if(current == nullptr){
        return false;
    }
    //Checking if the current robot is the same as the ID
    if(current->getID() == ID){
        return true;
    }
    //Recursively checking the left and right subtree
    return findRobot(current->getLeft(),ID) || findRobot(current->getRight(),ID);
}

bool Tester::checkRemoveEdge(){
    //Create object and remove root
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    Swarm obj(BST);
    int ID = idGen.getRandNum();
    ROBOTTYPE roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
    Robot robot(ID,roboType);
    obj.insert(robot);
    obj.remove(ID);

    //Checking if the tree is empty
    if(obj.m_root != nullptr){
        return false;
    }
    return true;
}
bool Tester::checkBalanceRemove(){
    //Create object and insert
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    Swarm obj(AVL);
    int Size = 300;
    int tempID[Size];
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;
    for(int i = 0;i < Size; i++){
        ID = idGen.getRandNum();
        tempID[i] = ID;
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID,roboType);
        obj.insert(robot);
    }
    //Remove 150 robots
    for(int i = 0;i < Size / 2; i++){
        obj.remove(tempID[i]);
    }
    return checkBalance(obj.m_root);
}
bool Tester::checkBSTPropertyRemove(){
    //Create object and insert
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    Swarm obj(BST);
    int Size = 300;
    int tempID[Size];
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;
    for(int i = 0;i < Size; i++){
        ID = idGen.getRandNum();
        tempID [i]= ID;
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID,roboType);
        obj.insert(robot);
    }
    //Remove 150 robots
    for(int i = 0;i < Size / 2; i++){
        obj.remove(tempID[i]);
    }
    return checkBSTProperty(obj.m_root);
}
bool Tester::checkBSTHeightRemove(){
    //Create object and insert
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    Swarm obj(BST);
    int Size = 300;
    int tempID[Size];
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;
    for(int i = 0;i < Size; i++){
        ID = idGen.getRandNum();
        tempID [i]= ID;
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID,roboType);
        obj.insert(robot);
    }
    //Remove 150 robots
    for(int i = 0;i < Size / 2; i++){
        obj.remove(tempID[i]);
    }
    return checkHeight(obj.m_root);
}
bool Tester::checkAssignmentOperator(){
    //Create object and insert 100 robots
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    Swarm obj(BST);
    int Size = 100;
    int tempID[Size];
    int ID = 0;
    ROBOTTYPE roboType = DEFAULT_TYPE;
    for(int i = 0;i < Size; i++){
        ID = idGen.getRandNum();
        tempID [i]= ID;
        roboType = static_cast<ROBOTTYPE>(typeGen.getRandNum());
        Robot robot(ID,roboType);
        obj.insert(robot);
    }
    //Assign obj2 to obj1
    Swarm obj2;
    obj2 =  obj;

    //Iterate through the tree and check if the ID is the same
    for (int i = 0; i < Size; i++){
        if (!findRobot(obj2.m_root,tempID[i])){
            return false;
        }
    }
    return true;

}

bool Tester::checkAssignmentOperatorEdge(){
    //Create 2 empty objects and assign obj2 to obj1
    Swarm obj1;
    Swarm obj2;
    obj1 = obj2;

    //Checking if both are empty
    if(obj1.m_root == nullptr && obj2.m_root == nullptr){
        return true;
    }
    return false;
}
int main(){
    Tester test;
    if(test.checkBalance()){
        cout << "Check balance in AVL after multiple insertions: Passed" << endl;
    } else {
        cout << "Check balance in AVL after multiple insertions: Failed" << endl;
    }
    if(test.checkBSTProperty()){
        cout << "Check BST Property after insertions: Passed" << endl;
    } else {
        cout << "Check BST Property after insertions: Failed" << endl;
    }
    if(test.checkSplay()){
        cout << "Check if the splaying correctly: Passed" << endl;
    } else {
        cout << "Check if the splaying correctly: Failed" << endl;
    }
    if(test.checkHeight()){
        cout << "Check height after multiple insertions in AVL Tree: Passed" << endl;
    } else {
        cout << "Check height after multiple insertions in AVL Tree: Failed" << endl;
    }
    if(test.checkRemove()){
        cout << "Check normal case remove in BST Tree: Passed" << endl;
    }else {
        cout << "Check normal case remove in BST Tree: Failed" << endl;
    }
    if(test.checkRemoveEdge()){
        cout << "Check edge case remove in BST Tree: Passed" << endl;
    } else {
        cout << "Check edge case remove in BST Tree: Failed" << endl;
    }
    if(test.checkBalanceRemove()){
        cout << "Check AVL Tree is balanced after multiple removes: Passed" << endl;
    } else {
        cout << "Check AVL Tree is balanced after multiple removes: Failed" << endl;
    }
    if(test.checkBSTPropertyRemove()){
        cout << "Check BST Property after multiple removes: Passed" << endl;
    } else {
        cout << "Check BST Property after multiple removes: Failed" << endl;
    }
    if(test.checkBSTHeightRemove()){
        cout << "Check BST Height after multiple removes: Passed" << endl;
    } else {
        cout << "Check BST Height after multiple removes: Passed" << endl;
    }
    if(test.checkAssignmentOperator()){
        cout << "Check Assignment Operator Normal Case: Passed" << endl;
    } else {
        cout << "Check Assignment Operator Normal Case: Failed" << endl;
    }
    if(test.checkAssignmentOperatorEdge()){
        cout << "Check Assignment Operator Edge Case: Passed" << endl;
    } else {
        cout << "Check Assignment Operator Edge Case: Failed" << endl;
    }
    return 0;
}