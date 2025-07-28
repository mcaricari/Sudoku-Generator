#include <iostream>
#include "SudokuSolver.h"

using namespace std;

static void printMatrix(std::vector<std::vector<Node>>& matrix)
{
	for (auto& row : matrix)
	{
		for (Node& n : row)
		{
			if (n.initialized)
				cout << '1';
			else
				cout << '0';
		}
		cout << endl;
	}
}

Node::Node(int row, int col, int value) :
	row{ row },
	col{ col },
	value{ value },
	initialized{ true } {
}

Node::Node()
{
}

void SudokuSolver::initMatrix()
{
	for (int i = 0; i < size; i++)			//fila
	{
		for (int j = 0; j < size; j++)		//columna
		{
			for (int k = 0; k < size; k++)	//valores posibles por celda
			{
				int row = (i * colOffset + j * size + k);

				Node* rowNode, * colNode, * cellNode, * boxNode;

				matrix[row][rowOffset + (i + size + k)] = Node(i, j, k);
				matrix[row][colOffset + (j + size + k)] = Node(i, j, k);
				matrix[row][cellOffset + (i + size + j)] = Node(i, j, k);
				matrix[row][boxOffset + ((i / boxPerRow + j / boxPerCol * boxPerCol) * size + k)] = Node(i, j, k);


				rowNode = &matrix[row][rowOffset + (i + size + k)];
				colNode = &matrix[row][colOffset + (j + size + k)];
				cellNode = &matrix[row][cellOffset + (i + size + j)];
				boxNode = &matrix[row][boxOffset + ((i / boxPerRow + j / boxPerCol * boxPerCol) * size + k)];

				rowNode->right = colNode;
				rowNode->left = boxNode;
				colNode->left = rowNode;
				colNode->right = cellNode;
				cellNode->left = colNode;
				cellNode->right = boxNode;
				boxNode->left = cellNode;
				boxNode->right = rowNode;
			}
		}
	}

	printMatrix(matrix);
}

bool SudokuSolver::linkHeadersAndColumns()
{;
	int j = 0;
	for (Node& h : headers)
	{
		Node* nextNodeToLink = &h;
		for (int i = 0; i < maxRows; i++)
		{
			if (matrix[i][j].initialized)
			{
				matrix[i][j].top = nextNodeToLink;
				nextNodeToLink->bottom = &matrix[i][j];
				matrix[i][j].bottom = &h;
				h.top = &matrix[i][j];
				matrix[i][j].colHeader = &h;
				nextNodeToLink = &matrix[i][j];
			}
		}

		if (h.bottom == &h)
			cout << "La columna no tiene filas. Col: " << j << endl;

		if (!addColumn(&h))
		{
			cout << "Error al añadir columna" << endl;
			return false;
		}

		j++;
	}

	return true;
}

bool SudokuSolver::addColumn(Node* newNode)
{
	bool ret = false;

	if (newNode->header)
		ret = addColumn(newNode, &root);

	return ret;
}

bool SudokuSolver::addColumn(Node* newNode, Node* rightNode) //inserta un header, o sea, una columna entera
{
	if (rightNode->right == &root && rightNode != newNode)
	{
		rightNode->right->left = newNode;
		newNode->right = rightNode->right;
		newNode->left = rightNode;
		rightNode->right = newNode;

		return true;
	}
	else if (rightNode == newNode)
		return false;
	else
		return addColumn(newNode, rightNode->right);
}

size_t SudokuSolver::calculateMaxSz()
{
	return size_t();
}

SudokuSolver::SudokuSolver(std::vector<std::vector<char>>& board, const int size, const int boxPerCol, const int boxPerRow) :
	maxRows{ static_cast<int>(pow(static_cast<double>(size), 3)) },
	maxCols{ static_cast<int>(pow(static_cast<double>(size), 2)) * 4 },
	size{ size },
	boxPerCol{ boxPerCol },
	boxPerRow{ boxPerRow },
	rowOffset{ 0 },
	colOffset{ static_cast<int>(pow(static_cast<double>(size), 2)) },
	cellOffset{ colOffset + colOffset },
	boxOffset{ cellOffset + colOffset }
{
	root = Node();
	root.bottom = &root;
	root.top = &root;
	root.left = &root;
	root.right = &root;

	matrix.assign(maxRows, vector<Node>(maxCols, Node()));
	headers.assign(maxCols, Node());

	for (Node& h : headers)
	{
		h.top = &h;
		h.bottom = &h;
		h.left = &h;
		h.right = &h;
		h.colHeader = &h;
		h.header = true;
	}

	initMatrix();
	if (linkHeadersAndColumns())
	{

	}

}
