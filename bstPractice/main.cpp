#include <iostream>
using namespace std; 

struct Node{
    int data; 
    Node* left; 
    Node* right; 
}; 

Node* createNode(int data){
    Node* newNode = new Node();
    newNode->data = data; 
    newNode->left = newNode->right = nullptr;
    return newNode;

}

void preorderDFS(Node* root){
    if (root == nullptr) return;

    cout<< root->data <<endl;
    preorderDFS(root->left);
    preorderDFS(root->right);
}

int main(){
    //LVL 1
    Node* root = createNode(1);
    //LVL 2
    root->left = createNode(2);
    root->right = createNode(3);
    //LVL 3
    root->left->left = createNode(4);
    root->right->right = createNode(5);
    root->right->left = createNode(7);
    //LVL 4
    // Ensure intermediate nodes exist before assigning their children
    if (root->left->right == nullptr) {
        root->left->right = createNode(-1); // placeholder value
    }
    root->left->right->left = createNode(9);

    if (root->right->right->left == nullptr) {
        // root->right->right exists (assigned above), so safe to assign its left child
        // but check anyway
    }
    root->right->right->left = createNode(15);

    preorderDFS(root);

    return 0;
};