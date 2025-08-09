//////////////////////////////////////////////////////////////////////////////////

/* CE1007/CZ1007 Data Structures
Lab Test: Section F - Binary Search Trees Questions
Purpose: Implementing the required functions for Question 5
		 Implementing 'remove node' operation for BST*/
//////////////////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////////

typedef struct _bstnode {
	int item;
	struct _bstnode* left;
	struct _bstnode* right;
} BSTNode;   // You should not change the definition of BSTNode

typedef struct _stackNode {
	BSTNode* data;
	struct _stackNode* next;
}StackNode; // You should not change the definition of StackNode

typedef struct _stack
{
	StackNode* top;
}Stack; // You should not change the definition of Stack

///////////////////////// function prototypes ////////////////////////////////////

// You should not change the prototypes of these functions
void postOrderIterativeS2(BSTNode* root);

void insertBSTNode(BSTNode** node, int value);

void push(Stack* stack, BSTNode* node);
BSTNode* pop(Stack* s);
BSTNode* peek(Stack* s);
int isEmpty(Stack* s);
void removeAll(BSTNode** node);
BSTNode* removeNodeFromTree(BSTNode* root, int value);

//////////////////////여기에 뭐 넣었어요////////////////////////////////
BSTNode* removeNodeFromTree2(BSTNode* root, int value);
BSTNode* BST_Search1(BSTNode* root, int value);
BSTNode* BST_Search2(BSTNode* root, int value);
BSTNode* BST_Replace(BSTNode* root, BSTNode* u, BSTNode* v);
BSTNode* BST_Delete(BSTNode* root, BSTNode* deleteNode);

///////////////////////////// main() /////////////////////////////////////////////

int main()
{
	int c, i;
	c = 1;

	//Initialize the Binary Search Tree as an empty Binary Search Tree
	BSTNode* root;
	root = NULL;

	printf("1: Insert an integer into the binary search tree;\n");
	printf("2: Print the post-order traversal of the binary search tree;\n");
	////////////////////여기에 넣음///////////////////////
	printf("3: Remove an integer from the binary search tree;\n");
	printf("4: Remove an integer from the binary search tree_2;\n");
	///////////////////////////////////////////////////////
	printf("0: Quit;\n");


	while (c != 0)
	{
		printf("Please input your choice(1/2/3/0): ");
		scanf("%d", &c);

		switch (c)
		{
		case 1:
			printf("Input an integer that you want to insert into the Binary Search Tree: ");
			scanf("%d", &i);
			insertBSTNode(&root, i);
			break;
		case 2:
			printf("The resulting post-order traversal of the binary search tree is: ");
			postOrderIterativeS2(root); // You need to code this function
			printf("\n");
			break;
		/////////////////////////여기에 넣음//////////////////////////
		case 3:
			printf("Input an integer you want to remove from the Binary Search Tree: ");
			scanf("%d", &i);
			printf("\n");
			root = removeNodeFromTree(root, i); // 삭제 후 반드시 root 갱신
			printf("The post-order traversal of the binary search tree after deleting %d is: ", i);
			postOrderIterativeS2(root);
			printf("\n");
			break;
		case 4:
			printf("Input an integer you want to remove from the Binary Search Tree_2: ");
			scanf("%d", &i);
			printf("\n");
			root = removeNodeFromTree2(root, i); // 삭제 후 반드시 root 갱신
			printf("The post-order traversal of the binary search tree after deleting %d is: ", i);
			postOrderIterativeS2(root);
			printf("\n");
			break;
		//////////////////////////////////////////////////////////////
		case 0:
			removeAll(&root);
			break;
		default:
			printf("Choice unknown;\n");
			break;
		}

	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////

void postOrderIterativeS2(BSTNode* root)
{
	// two stacks을 이용해서 후위순회를 해봐라
	// 푸쉬 팝만 쓰고

	if (root == NULL) return;

	Stack* tempStack1 = malloc(sizeof(Stack));
	Stack* tempStack2 = malloc(sizeof(Stack));
	tempStack1->top = NULL;
	tempStack2->top = NULL;

	push(tempStack1, root);
	while (!isEmpty(tempStack1))
	{
		BSTNode* now = pop(tempStack1);

		// 이러면 tempStack2에 root, rifht, left 순으로 들어감 -> 팝하면 후위순회 순으로
		// tempStack1은 방문 예정인 노드를 DFS 느낌으로 찾고
		// tempStack2는 방문 완료된 노드를 기록함 -> 후위순회 역순으로 스택에 누적함
		push(tempStack2, now);
		if (now->left != NULL)
			push(tempStack1, now->left);
		if (now->right != NULL)
			push(tempStack1, now->right);

	}
	while (!isEmpty(tempStack2))
	{
		BSTNode* now = pop(tempStack2);
		printf("%d ", now->item);
	}
}

// 메모리적으로 removeNodeFromTree이게 훨씬 좋음
// removeNodeFromTree_2는 중복해서 탐색하잖아 


/* Given a binary search tree and a key, this function
   deletes the key and returns the new root. Make recursive function. */
BSTNode* removeNodeFromTree(BSTNode* root, int value)
{
	if (root == NULL) return NULL; 

	// 삭제할 값이 현재 노드 값보다 작으면 왼쪽 서브트리에서 삭제
	if (value < root->item) 
	{
		
		root->left = removeNodeFromTree(root->left, value);
	}
	// 삭제할 값이 현재 노드 값보다 크면 오른쪽 서브트리에서 삭제
	else if (value > root->item) 
	{
		
		root->right = removeNodeFromTree(root->right, value);
	}
	// 현재 노드가 삭제 대상인 경우
	else 
	{
		// case 1: 자식이 없거나, 오른쪽 자식만 있는 경우
		if (root->left == NULL) 
		{
			BSTNode* temp = root->right;  // 오른쪽 자식 포인터 저장
			free(root);                   // 현재 노드 메모리 해제
			return temp;                  // 오른쪽 자식(또는 NULL)을 반환
		}
		// case 2: 왼쪽 자식만 있는 경우
		else if (root->right == NULL) 
		{
			BSTNode* temp = root->left;   // 왼쪽 자식 포인터 저장
			free(root);                   // 현재 노드 메모리 해제
			return temp;                  // 왼쪽 자식 반환
		}

		// case 3: 자식이 모두 있을 때
		// 현재 노드를 대체할 in-order successor(next) 찾기
		BSTNode* nextParent = root;       // next 부모 노드
		BSTNode* next = root->right;      // 오른쪽 서브트리 시작
		while (next->left != NULL)
		{      
			nextParent = next;			  // 가장 왼쪽(최소값)까지 이동
			next = next->left;
		}
		root->item = next->item;          // next 값을 현재 노드에 복사

		// next를 트리에서 삭제
		if (nextParent->left == next)
		{
			// next가 nextParent의 왼쪽 자식이면
			nextParent->left = removeNodeFromTree(nextParent->left, next->item);
		}
		else 
		{
			// next가 nextParent의 오른쪽 자식이면
			nextParent->right = removeNodeFromTree(nextParent->right, next->item);
		}
	}
	return root; 
}


BSTNode* removeNodeFromTree2(BSTNode* root, int value)
{
	// value값의 노드를 삭제하고, 새로운 트리 루트 포인터를 반환해라.
	// 1. BST에서 재귀적으로 특정 노드를 찾고, -> BST_Search2
	// 2. 그 노드에 연관된 부분들을 복구해주자 -> BST_Delete
	// 3. 아 이게 아닌가봐 씨

	if (root == NULL) return NULL;

	// 일단 찾고
	BSTNode* deleteNode = BST_Search2(root, value);
	if (deleteNode == NULL) return root;

	// 걜 삭제해
	root = BST_Delete(root, deleteNode);
	return root;

}

// 이건 반복문으로
BSTNode* BST_Search1(BSTNode* root, int value)
{
	if (root == NULL) return NULL;

	BSTNode* now = root;

	while (now && value != now->item)
	{
		if(now->item > value)
		{
			now = now->left;
		}
		else
		{
			now = now->right;
		}
	}

	return now;
	
}

// 이게 재귀적으로
BSTNode* BST_Search2(BSTNode* root, int value)
{
	if (root == NULL) return NULL;

	BSTNode* now = root;

	if(now == NULL || now->item == value)
	{
		return now;
	}

	if (now->item > value)
	{
		return BST_Search2(now->left, value);
	}
	else
	{
		return BST_Search2(now->right, value);
	}
}

BSTNode* BST_Delete(BSTNode* root, BSTNode* deleteNode)
{
	if (deleteNode == NULL) return root;

	// deleteNode가 자식없음
	if (deleteNode->left == NULL && deleteNode->right == NULL) 
	{
		root = BST_Replace(root, deleteNode, NULL);
	}

	// 자식이 하나만 있음
	else if (deleteNode->left == NULL)
	{
		root = BST_Replace(root, deleteNode, deleteNode->right);
	}
	else if (deleteNode->right == NULL)
	{
		root = BST_Replace(root, deleteNode, deleteNode->left);
	}
	// 자식이 두개 있음
	else
	{
		// deleNode의 오른쪽 서브트리에서 가장 작은 값을 찾을거임(삭제할 노드의 바로 다음(next)노드)
		BSTNode* nextParent = deleteNode; // next의 부모, 초기값은 일단 deleNode로
		BSTNode* next = deleteNode->right;	// 오른쪽 서브트리의 시작

		// 오른쪽 서브트리의 가장 왼쪽에 있는게(가장 작은 값) 내 다음 next겠지
		while (next->left) 
		{
			nextParent = next;
			next = next->left;
		}
		// next의 값을 deleteNode에 복사해줌! 즉, deleteNode를 next로 대체하고 끝단의 next를 삭제할거임
		deleteNode->item = next->item;

		// next를 실제로 삭제함
		if (nextParent->left == next)
		{
			nextParent->left = BST_Delete(nextParent->left, next);
		}	
		else
		{
			nextParent->right = BST_Delete(nextParent->right, next);
		}	
	}

	return root;
}

// 두 연결된 노드를 서로 교체하는 함수가 필요해
BSTNode* BST_Replace(BSTNode* root, BSTNode* u, BSTNode* v)
{
	// u: 삭제할 노드
	// v: 교체할 노드 (NULL도 가능)
	// u를 v로 바꿀거임

	if(root == NULL || u == NULL) return root;

	// 루트 삭제하게?
	if (root == u) 
	{
		free(u);
		return v; // 그럼 v가 걍 루트 됨
	}

	BSTNode* parent = root;
	BSTNode* now = root;

	while (now != NULL && now != u) 
	{
		parent = now;				// 현재 노드를 부모로 저장
		if (u->item < now->item)	
			now = now->left;		// u값이 지금 노드보다 작으면 왼쪽 찾고
		else
			now = now->right;		// u값이 지금 노드보다 크거나 같으면 오른쪽 찾고
	}

	if (now == NULL) return root;	// 트리에 u가 없어 

	// 부모의 자식 포인터 중에서 u였던 노드를 v로 바꿔줌
	if (parent->left == u)		
	{
		parent->left = v;
	}
	else if (parent->right == u)
	{
		parent->right = v;
	}

	free(u);
	return root;
}

///////////////////////////////////////////////////////////////////////////////

void insertBSTNode(BSTNode** node, int value) {
	if (*node == NULL)
	{
		*node = malloc(sizeof(BSTNode));

		if (*node != NULL) {
			(*node)->item = value;
			(*node)->left = NULL;
			(*node)->right = NULL;
		}
	}
	else
	{
		if (value < (*node)->item)
		{
			insertBSTNode(&((*node)->left), value);
		}
		else if (value > (*node)->item)
		{
			insertBSTNode(&((*node)->right), value);
		}
		else
			return;
	}
}

//////////////////////////////////////////////////////////////////////////////////

void push(Stack* stack, BSTNode* node)
{
	StackNode* temp;

	temp = malloc(sizeof(StackNode));

	if (temp == NULL)
		return;
	temp->data = node;

	if (stack->top == NULL)
	{
		stack->top = temp;
		temp->next = NULL;
	}
	else
	{
		temp->next = stack->top;
		stack->top = temp;
	}
}


BSTNode* pop(Stack* s)
{
	StackNode* temp, * t;
	BSTNode* ptr;
	ptr = NULL;

	t = s->top;
	if (t != NULL)
	{
		temp = t->next;
		ptr = t->data;

		s->top = temp;
		free(t);
		t = NULL;
	}

	return ptr;
}

BSTNode* peek(Stack* s)
{
	StackNode* temp;
	temp = s->top;
	if (temp != NULL)
		return temp->data;
	else
		return NULL;
}

int isEmpty(Stack* s)
{
	if (s->top == NULL)
		return 1;
	else
		return 0;
}


void removeAll(BSTNode** node)
{
	if (*node != NULL)
	{
		removeAll(&((*node)->left));
		removeAll(&((*node)->right));
		free(*node);
		*node = NULL;
	}
}
