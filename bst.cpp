#include <iostream>
using namespace std;

struct node{

	int val;
	node* left;
	node* right;

};

class BST{
	int* sorted;
	node* root;

	void insert_helper(node* temp,int val){
		if (temp == NULL){
			temp = new node;
			temp->val = val;
			temp->right = NULL;
			temp->left = NULL;
		}else if (temp->val > val){
			if(temp->left == NULL){
				temp->left = new node;
				temp->left->val = val;
				temp->left->left = NULL;
				temp->left->right = NULL;
			}else {
				insert_helper(temp->left,val);
			}
		}else if (temp->val <= val){
			if(temp->right == NULL){
				temp->right = new node;
				temp->right->val = val;
				temp->right->left = NULL;
				temp->right->right = NULL;
			}else {
				insert_helper(temp->right,val);
			}
		}
	}

	void print_in_order_helper(node* temp){
		if (temp == NULL){
			return;
		}else {
			print_in_order_helper(temp->left);
			cout<<temp->val<<endl;
			print_in_order_helper(temp->right);
		}
	}
	
	public:
		char* sorted_tree(){
			//do in order traversal here and insert
		}

		void insert(int val){
			if(root){
				insert_helper(root,val);
			}else {
				root = new node;
				root->val = val;
				root->left = NULL;
				root->right = NULL;
			}
		}

		void print_in_order(){
			print_in_order_helper(this->root);
		}

		
};

int main(){
	BST *bst = new BST();
	// bst->insert(2);
	// bst->insert(4);
	// bst->insert(1);
	for (int i=0;i<20;i++){
		bst->insert(rand() + i);
	}
	cout<<"inserted tree"<<endl;
	bst->print_in_order();
}