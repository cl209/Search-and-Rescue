// UMBC - CMSC 341 - Fall 2024 - Proj2
#ifndef SWARM_H
#define SWARM_H
#include <iostream>
using namespace std;
class Grader;//this class is for grading purposes, no need to do anything
class Tester;//this is your tester class, you add your test functions in this class
class Swarm;
enum STATE {ALIVE, DEAD};   // possible states for a robot
enum ROBOTTYPE {BIRD, DRONE, REPTILE, SUB, QUADRUPED};
enum TREETYPE {NONE, BST, AVL, SPLAY};
const int MINID = 10000;    // min robot ID
const int MAXID = 99999;    // max robot ID
#define DEFAULT_HEIGHT 0
#define DEFAULT_ID 0
#define DEFAULT_TYPE DRONE
#define DEFAULT_STATE ALIVE

class Robot{
    public:
    friend class Swarm;
    friend class Grader;
    friend class Tester;
    Robot(int id, ROBOTTYPE type = DEFAULT_TYPE, STATE state = DEFAULT_STATE)
        :m_id(id),m_type(type), m_state(state) {
            m_left = nullptr;
            m_right = nullptr;
            m_height = DEFAULT_HEIGHT;
        }
    Robot(){
        m_id = DEFAULT_ID;
        m_type = DEFAULT_TYPE;
        m_state = DEFAULT_STATE;
        m_left = nullptr;
        m_right = nullptr;
        m_height = DEFAULT_HEIGHT;
    }
    int getID() const {return m_id;}
    STATE getState() const {return m_state;}
    string getStateStr() const {
        string text = "";
        switch (m_state)
        {
        case DEAD:text = "DEAD";break;
        case ALIVE:text = "ALIVE";break;
        default:text = "UNKOWN";break;
        }
        return text;
    }
    ROBOTTYPE getType() const {return m_type;}
    string getTypeStr() const {
        string text = "";
        switch (m_type)
        {
        case BIRD:text = "BIRD";break;
        case DRONE:text = "DRONE";break;
        case REPTILE:text = "REPTILE";break;
        case SUB:text = "SUB";break;
        case QUADRUPED:text = "QUADRUPED";break;
        default:text = "UNKNOWN";break;
        }
        return text
        ;
    }
    int getHeight() const {return m_height;}
    Robot* getLeft() const {return m_left;}
    Robot* getRight() const {return m_right;}
    void setID(const int id){m_id=id;}
    void setState(STATE state){m_state=state;}
    void setType(ROBOTTYPE type){m_type=type;}
    void setHeight(int height){m_height=height;}
    void setLeft(Robot* left){m_left=left;}
    void setRight(Robot* right){m_right=right;}
    private:
    int m_id;
    ROBOTTYPE m_type;
    STATE m_state;
    Robot* m_left;  //the pointer to the left child in the BST
    Robot* m_right; //the pointer to the right child in the BST
    int m_height;   //the height of this node in the BST
};
class Swarm{
    public:
    friend class Grader;
    friend class Tester;
    Swarm();
    Swarm(TREETYPE type);
    ~Swarm();
    const Swarm & operator=(const Swarm & rhs);
    void clear();
    TREETYPE getType() const;
    void setType(TREETYPE type);
    void insert(const Robot& robot);
    void remove(int id);
    void dumpTree() const;
    private:
    Robot* m_root;  // the root of the BST
    TREETYPE m_type;// the type of tree

    // ***************************************************
    // Any private helper functions must be delared here!
    // ***************************************************
    void clear(Robot* root);
    void insert(const Robot& robot, Robot*& current);
    void updateHeight(Robot* current);
    Robot* remove(int id, Robot* current);
    Robot* successor(Robot* current);
    int balanceFactor(Robot* current);
    Robot* balance(Robot*& current);
    Robot* copy(Robot* current);

    //Rotations
    Robot* ll_Rotate(Robot*& current);
    Robot* rr_Rotate(Robot*& current);
    Robot* lr_Rotate(Robot*& current);
    Robot* rl_Rotate(Robot*& current);

    //Splay helper function
    Robot* splay(Robot*& root, int id);


    void dump(Robot* aBot) const;//helper for recursive traversal
};
#endif