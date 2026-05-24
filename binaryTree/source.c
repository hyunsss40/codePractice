#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
	int data;
	struct TreeNode* left, * right;
}TreeNode;

int max = 0, minor = 99999;
void Finding(TreeNode* root) {
	if (root == NULL)return;
	if (root->data < minor)
		minor = root->data;
	if (root->data > max)
		max = root->data;
	Finding(root->left);
	Finding(root->right);

}

int main() {
	TreeNode* n1, * n2, * n3;
	n1 = (TreeNode*)malloc(sizeof(TreeNode));
	n2 = (TreeNode*)malloc(sizeof(TreeNode));
	n3 = (TreeNode*)malloc(sizeof(TreeNode));

	n1->data = 10;
	n1->left = n2;
	n1->right = n3;

	n2->data = 20;
	n2->left = NULL;
	n2->right = NULL;

	n3->data = 30;
	n3->left = NULL;
	n3->right = NULL;

	Finding(n1);
	printf("최소값: %d\n", minor);
	printf("최대값: %d\n", max);

	free(n1); free(n2); free(n3);
}