// UMBC - CMSC 341 - Fall 2024 - Proj2
#include "swarm.h"
Swarm::Swarm(){
    //Create emoty object
    m_root = nullptr;
    m_type = NONE;

}
Swarm::Swarm(TREETYPE type){//Incomplete
    //Create empty root
    m_root = nullptr;
    //Make type
    m_type = type;
}
Swarm::~Swarm(){
    clear();
    m_root = nullptr;
}
//Helper function for deletion
void Swarm::clear(Robot* root){
    if (root != nullptr){
        //Recursively delete the left child
        clear(root->m_left);
        //Recursively delete the right child
        clear(root->m_right);
        //Delete the current node
        delete root;
    }
}
void Swarm::clear(){
    //If not null, it has nodes
    if (m_root != nullptr){
        //Recursively delete
        clear(m_root);
    }
    //Initialize to null
    m_root = nullptr;

    return;
}

void Swarm::insert(const Robot& robot){
    //Check that it is within the range
    if (robot.getID() < MINID || robot.getID() > MAXID){
        return;
    }
    //If null, create a new tree
    if (m_root == nullptr){
        m_root = new Robot(robot.m_id, robot.m_type, robot.m_state);
    } else {
        //Use helper function to insert new node
        insert(robot, m_root);
    }
}
void Swarm::insert(const Robot& robot, Robot*& current){
    //Base Case: If where the current is null, create new node
    if (current == nullptr){
        current = new Robot(robot.m_id, robot.m_type, robot.m_state);
    } else if(current->getID() == robot.getID()){
        //Do nothing
        return;
    //Recursive to implement the new node
    } else if (robot.getID() > current->getID()){ // More than
        //insert to the right of current
        insert(robot, current->m_right); 
    //Recursive to implement the new node
    } else if (robot.getID() < current->getID()){ // Less than
        //insert to the left of current
        insert(robot, current->m_left);
    }
    //Update the height base on type
    //AVL
    if (m_type == AVL){
        current = balance(current);
    //SPLAY
    } else if (m_type == SPLAY){
        current = splay(current, robot.getID());
    }
    updateHeight(current);
}
void Swarm::updateHeight(Robot* current){
    //Check if current is null
    if (current == nullptr){
        return;
    }
    //Case 0: no child
    if (current->getRight() == nullptr && current->getLeft() == nullptr){
        current->setHeight(0);
    }
    //Case 1: 1 child
    //Right child exist
    else if (current->getRight() != nullptr && current->getLeft() == nullptr){
        current->setHeight(1 + current->getRight()->getHeight());
    }
    //Left child exist
    else if (current->getLeft() != nullptr && current->getRight() == nullptr){
        current->setHeight(1 + current->getLeft()->getHeight());
    }
    //Case 2: 2 children
    else if (current->getRight() != nullptr && current->getLeft() != nullptr){
        //Get the heights of left and right children
        int leftHeight = current->getLeft()->getHeight();
        int rightHeight = current->getRight()->getHeight();
        //Set the height based on the maximum of the two
        current->setHeight(1 + max(leftHeight, rightHeight));
    }
}

Robot* Swarm::balance(Robot*& current){
    //Store the height of the left and right child
    int childDifference = balanceFactor(current);
     //Left heavy
    if(childDifference > 1){
        //If the left child has more robots
        if(balanceFactor(current->getLeft()) >= 0){
            //Rotating to the right
            current = ll_Rotate(current);
        }
        //If the right child has more robots
        else{
            //Right-then-left rotation
            current = lr_Rotate(current);
        }
    }
    //Right heavy
    else if(childDifference < -1){
        //If the right child has more robots
        if(balanceFactor(current->getRight()) <= 0){
            //Rotating to the left
            current = rr_Rotate(current);
        }
        //If the left child has more robots
        else{
            //Right then left rotation
            current = rl_Rotate(current);
        }
    }
    return current;
}
int Swarm::balanceFactor(Robot* robot){
    //Default values
    int leftHeight = -1;
    int rightHeight = -1;

    //Check if left exist
    if (robot->getLeft() != nullptr){
        leftHeight = robot->getLeft()->getHeight();
    }
    //Check if right exist
    if (robot->getRight() != nullptr){
        rightHeight = robot->getRight()->getHeight();
    }
    //Return the difference
    return (leftHeight - rightHeight);
}
Robot* Swarm::ll_Rotate(Robot*& current) {
    // Save the left child
    Robot* temp = current->getLeft();
    if (temp == nullptr) {
        return current;
    }
    //Save the right child of current
    Robot* nextTemp = temp->getRight();
    //Set current left to nextTemp (Null or some node)
    current->setLeft(nextTemp);
    // Set temp's right to current
    temp->setRight(current);
    // Update heights after modifications
    updateHeight(current);
    updateHeight(temp);
    
    return temp; // New root of subtree
}

Robot* Swarm::rr_Rotate(Robot*& current) {
    //Save the right child
    Robot* temp = current->getRight();
    if (temp == nullptr){
        return current;
    }
    //Save the left child of current
    Robot* nextTemp = temp->getLeft();
    //Set current right to nextTemp (Null or some node)
    current->setRight(nextTemp);
    //Set temp's left to current
    temp->setLeft(current);
    //Update height
    updateHeight(current);
    updateHeight(temp);
    return temp; // New root of subtree
}

Robot* Swarm::lr_Rotate(Robot*& current){
    Robot* temp = current->getLeft();
    //Rotate the left child to the left
    current->setLeft(rr_Rotate(temp));
    //Rotate the current to the right
    return ll_Rotate(current);
}
Robot* Swarm::rl_Rotate(Robot*& current){
    Robot* temp = current->getRight();
    //Rotate the right child to the right
    current->setRight(ll_Rotate(temp));
    //Rotate the current to the left   
    return rr_Rotate(current);
}

Robot* Swarm::splay(Robot*& root, int id) {
    if (root == nullptr) {
        return root; //Base case
    }

    //Left side heavy
    if (id < root->getID()) {
        //Zig-Zig 
        if (root->getLeft() != nullptr && id < root->getLeft()->getID()) {
            //Recursively splay for the left-left grandchild
            root->getLeft()->setLeft(splay(root->m_left->m_left, id));
            //Perform right rotation for root
            root = ll_Rotate(root);
        }
        // Zig-Zag
        else if (root->getLeft() != nullptr && id > root->getLeft()->getID()) {
            //Recursively splay for the left-right grandchild
            root->getLeft()->setRight(splay(root->m_left->m_right, id));
            //Perform left rotation on the left child
            if (root->getLeft()->getRight() != nullptr) {
                root->setLeft(rr_Rotate(root->m_left));
            }
        }

        //Return root after the rotations
        return (root->getLeft() == nullptr) ? root : ll_Rotate(root);
    }
    //Right side heavy
    else if (id > root->getID()) {
        //Zig-Zig
        if (root->getRight() != nullptr && id > root->getRight()->getID()) {
            //Recursively splay for the right-right grandchild
            root->getRight()->setRight(splay(root->m_right->m_right, id));
            //Perform left rotation for root
            root = rr_Rotate(root);
        }
        //Zig-Zag
        else if (root->getRight() != nullptr && id < root->getRight()->getID()) {
            // Recursively splay for the right-left grandchild
            root->getRight()->setLeft(splay(root->m_right->m_left, id));
            // Perform right rotation on the right child
            if (root->getRight()->getLeft() != nullptr) {
                root->setRight(ll_Rotate(root->m_right));
            }
        }

        //Return root after the rotations
        return (root->getRight() == nullptr) ? root : rr_Rotate(root);
    }

    //ID found (base case)
    return root;
}


void Swarm::remove(int id) {
    //If the tree is empty, do nothing
    if (m_root == nullptr) {
        return;
    }
    
    //Use helper function
    if (m_type != SPLAY || m_type != NONE) {
        m_root = remove(id, m_root);
    }
    //Check if null
    if (m_root == nullptr) {
        return; //Return
    }
    
    //Update height
    updateHeight(m_root);
    
    // Balance the tree if AVL
    if (m_type == AVL) {
        m_root = balance(m_root); //Perform AVL balancing
    }
    
    //Update the height again after balancing
    updateHeight(m_root);
}

Robot* Swarm::remove(int id, Robot* current){
    //Base case: if current is null, return nullptr
    if (current == nullptr){
        return nullptr;
    }
    //Recursive calls to find the node to remove
    if (id > current->getID()){
        current->setRight(remove(id, current->getRight()));
    } else if (id < current->getID()){
        current->setLeft(remove(id, current->getLeft()));
    } else { 
        //ID is found, handle three cases
        
        //Case 1: No child (leaf node)
        if (current->getRight() == nullptr && current->getLeft() == nullptr){
            delete current;
            current = nullptr;
        }
        //Case 2: One child (left or right)
        else if (current->getRight() == nullptr || current->getLeft() == nullptr) {
            Robot* temp = current;
            //If right child is not null, promote
            if (current->getRight() != nullptr)
                current = current->getRight();
            //If left child is not null, promote
            else
                current = current->getLeft();
            delete temp;//Delete the node
        }
        //Case 3: Two children
        else {
            Robot* temp = successor(current->getRight()); // Find in-order successor
            //Copy data to current node
            current->setID(temp->getID());
            current->setType(temp->getType());
            current->setState(temp->getState());
            //Remove the successor node
            current->setRight(remove(temp->getID(), current->getRight()));
        }
    }
    //If current is not null
    if(current != nullptr){
        //Update the height of the current node
        updateHeight(current);
        //Balance the tree if AVL
        current = balance(current);
        //Updating the height after balancing
        updateHeight(current);
    }
    return current;
}
Robot* Swarm::successor(Robot* current){
    //Base case: if current is null, return nullptr
    if (current == nullptr){
        return nullptr;
    }
    //Recursive call to find the minimum value
    if (current->getLeft() != nullptr){
        return successor(current->getLeft());
    }
    return current;
}
const Swarm & Swarm::operator=(const Swarm & rhs){
    //If tree is not same
    if(&rhs != this){
        //Clear the tree
        if(m_root != nullptr) clear();
        
        //Copy the tree
        if(rhs.m_root != nullptr) 
            m_root = copy(rhs.m_root);
    }
    return *this;
}
Robot* Swarm::copy(Robot* rhs) {
    Robot* current = nullptr;
    Robot* left = nullptr;
    Robot* right = nullptr;

    //Making a new node if it is a leaf
    if(rhs->getRight() == nullptr && rhs->getLeft() == nullptr){
        current = new Robot(rhs->getID(), rhs->getType(), rhs->getState());
        current->setHeight(rhs->getHeight());
        current->setRight(right);
        current->setLeft(left);
        return current;
    }

    //Copy left
    if(rhs->getLeft() != nullptr){
        left = copy(rhs->getLeft());
    }

    //Copy right
    if(rhs->getRight() != nullptr){
        right = copy(rhs->getRight());
    }

    //Creating a new node
    current = new Robot(rhs->getID(), rhs->getType(), rhs->getState());
    current->setHeight(rhs->getHeight());
    current->setRight(right);
    current->setLeft(left);
    return current;
}

TREETYPE Swarm::getType() const{
    return m_type;
}
void Swarm::setType(TREETYPE type) {
    //If the type is the same, do nothing
    if (m_type == type) {
        return;
    }
    //If the new type is AVL
    if (type == AVL) {
        //Balance the tree
        if (m_type == BST || m_type == SPLAY) {
            balance(m_root);
        }
    }
    //Set new type
    m_type = type;
    //Clear if the type is AVL and the current type is BST or SPLAY
    if (m_type == AVL && (type == SPLAY || type == BST)) {
        clear();
    }
}

void Swarm::dumpTree() const 
{
    dump(m_root);
}
void Swarm::dump(Robot* aBot) const{
    if (aBot != nullptr){
        cout << "(";
        dump(aBot->m_left);//first visit the left child
        cout << aBot->m_id << ":" << aBot->m_height;//second visit the node itself
        dump(aBot->m_right);//third visit the right child
        cout << ")";
    }
}