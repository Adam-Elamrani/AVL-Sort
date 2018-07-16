/*  Adam Elamrani - ad963765
    COP 3502 - McAlpin
    HW 3 - Summer 2017

    I Adam Elamrani (ad963765) afﬁrm that this program is entirely my own
    work and that I have neither developed my code together with any another
    person, nor copied any code from any other person, nor permitted my code
    to be copied or otherwise used by any other person, nor have I copied,
    modiﬁed, or otherwise used program code that I have found in any external
    source, including but not limited to, online sources. I acknowledge that
    any violation of the above terms will be treated as academic dishonesty.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Updated airPdata struct to new specifications.
typedef struct airPdata
{
	char *LocID; //Airport’s ‘‘Short Name’’, ie MCO 
	char *fieldName; //Airport Name 
	char *city; //Associated City 
	float longitude; //Longitude 
	float latitude; //Latitude 
} airPdata;

// Node struct to be used in the AVL tree
struct node
{
	float key;
    airPdata *airport;
    struct node *left;
    struct node *right;
    int height;
};

// Function prototypes
void printData(airPdata *airport);
void destroyAirport (airPdata *airport);
airPdata *createAirport ();
float sexag2decimal (char *degreeString);
void sortByLocID (airPdata *airport);
void sortByLatitude (airPdata *airports);
int height(struct node *N);
int max(int a, int b);
struct node* newNode(float key, airPdata *airport);
struct node *rightRotate(struct node *y);
struct node *leftRotate(struct node *x);
int getBalance(struct node *N);
struct node* insert(struct node* node, float key, airPdata *airport);
void inOrder(struct node *root);
float concat(float a, float b);
float LocID2Float(char *LocID);


int main (int argc, char *argv[])
{
	// variable declarations and file I/O
	const char *fileName = argv[1];
	char *sortParam = argv[2];
	FILE *inputFile = NULL;
	inputFile = fopen(fileName, "r");
	char line[1000];
	struct node *root = NULL;

	// guard condition to ensure a sort parameter is passed at runtime
	if (argc <= 2)
	{
		printf("\nhw3Sort ERROR: valid sort parameters are a or n\n");
		return(-1);
	}

	// guard condition to ensure the csv file exists and can be read from
	if (inputFile == NULL)
	{
		printf("\nhw3Sort ERROR: File \"%s\" not found.\n\n", fileName);
		return(-1);
	}

	// guard condition to ensure proper sort parameters are passed at runtime
	if (tolower(sortParam[0]) != 'a' && tolower(sortParam[0]) != 'n')
	{
		printf("\nhw3Sort ERROR: valid sort parameters are a or n\n");
		return(-1);
	}

	printf("code,name,city,lat,lon\n");

	// Parsing the CSV file line by line 
	while (fgets(line, 999, inputFile))
	{
		// Variable declarations
		int lineLength = 0, commaCount = 0, i, k = 0;
		lineLength = strlen(line);
		char tempstring[1000] = " ";
		

		// Properly allocates memory for the struct pointer and its members
		airPdata *airport = createAirport();

		// Iterates through each line, extracts and stores necessary data 
		for (i = 0; i < lineLength - 1; i++)
		{
			if (line[i] == ',')
			{
				if (commaCount == 0)
				{
					memset(tempstring, '\0', sizeof(tempstring));
					k = 0;
				}
				if (commaCount == 1)
				{
					strcpy(airport->LocID, tempstring);
					memset(tempstring, '\0', sizeof(tempstring));
					k = 0;
				}
				if (commaCount == 2)
				{
					strcpy(airport->fieldName, tempstring);
					memset(tempstring, '\0', sizeof(tempstring));
					k = 0;
				}
				if (commaCount == 3)
				{
					strcpy(airport->city, tempstring);
					memset(tempstring, '\0', sizeof(tempstring));
					k = 0;
				}	
				if (commaCount == 4)
				{
					memset(tempstring, '\0', sizeof(tempstring));
					k = 0;
				}
				if (commaCount == 5)
				{
					memset(tempstring, '\0', sizeof(tempstring));
					k = 0;
				}
				if (commaCount == 6)
				{
					memset(tempstring, '\0', sizeof(tempstring));
					k = 0;
				}
				if (commaCount == 7)
				{
					memset(tempstring, '\0', sizeof(tempstring));
					k = 0;
				}
				if (commaCount == 8)
				{
					airport->latitude = sexag2decimal(tempstring);
					memset(tempstring, '\0', sizeof(tempstring));
					k = 0;
				}
				if (commaCount == 9)
				{
					airport->longitude = sexag2decimal(tempstring);
					memset(tempstring, '\0', sizeof(tempstring));
					k = 0;
				}

				commaCount++;
			}
			else
			{
				tempstring[k] = line[i];
				k++;
			}
		}
		
		// tests each individual airport, filters out the ones that dont meet
		// the specified criteria, then inserts the valid ones into the AVL
		// tree using a numerical representation of the airPdata struct
		// member "char *LocID" as the key. 
		if (sortParam[0] == 'a')
		{
			sortByLocID(airport);
			if (airport->LocID != NULL)
			{
				root = insert(root, LocID2Float(airport->LocID), airport);
			}
		}

		// tests each individual airport, filters out the ones that dont meet
		// the specified criteria, then inserts the valid ones into the AVL
		// tree using the airPdata struct member "float latitude" as the key.
		if (sortParam[0] == 'n')
		{
			sortByLatitude(airport);
			if (airport->LocID != NULL && airport->latitude != 0.0000)
			{
				root = insert(root, airport->latitude, airport);
			}
		}
	
	}

	// performs an inOrder traversal of the AVL tree to output
	// the airport data sorted alphabetically by their LocID. 
	if (sortParam[0] == 'a')
	{
		inOrder(root);
	}

	// performs an inOrder traversal of the AVL tree to output the airport
	// data sorted in ascending order by their decimal degree latitude values
	// to represent moving from South to North as specified. 
	if (sortParam[0] == 'n')
	{
		inOrder(root);
	}

	fclose(inputFile);
	return 0;
}

// prints all the members of the airPdata struct passed
void printData(airPdata *airport)
{
	if (airport != NULL)
	{
		 	printf("%s,", airport->LocID);
			printf("%s,", airport->fieldName);
			printf("%s,", airport->city);
			printf("%.4f,", airport->latitude);
			printf("%.4f", airport->longitude);
			printf("\n");
	}

	return;
}

// Allocates memory for a new airPdata struct and all its members, initializes
// float values, and returns a pointer to the newly created airPdata struct.
airPdata *createAirport ()
{
	airPdata *airport = malloc(sizeof(airPdata));
	airport->LocID = (char*)malloc(51 * sizeof(char));
	airport->fieldName = (char*)malloc(51 * sizeof(char));
	airport->city = (char*)malloc(51 * sizeof(char));
	airport->longitude = 0.0;
	airport->latitude = 0.0;

	return airport;
}


// Frees all the memory previously allocated in the createAirport()
// function, and sets all pointers back to null. 
void destroyAirport (airPdata *airport)
{
	free(airport->LocID);
	airport->LocID = NULL;
	free(airport->fieldName);
	airport->fieldName = NULL;
	free(airport->city);
	airport->city = NULL;
	free(airport);
	airport = NULL;
}

// Converts the string of characters passed into smaller integer values
// that represent degrees, minutes, seconds, and milliarcseconds. Then tests
// each of these to ensure they are within the acceptable range, then performs
// the necessary base conversions to return an accurate float degree value. 
float sexag2decimal (char *degreeString)
{
	int DD, MM, SS, MAS;
	int params = 5;
	char D;
	float degreeFloat = 0.0;
	float SSMAS = 0.0;

	if (degreeString == NULL)
	{
		return 0.0;
	}

	// parsing the character string and testing for the required data
	if (sscanf(degreeString, "%d-%d-%d.%d%c", &DD, &MM, &SS, &MAS, &D) != params)
	{
		return 0.0;
	}

	// ensuring the data is within acceptable ranges
	if (DD < 0 || DD > 180 || MM < 0 || MM > 59 || SS < 0 || SS > 59 || MAS < 0
		|| MAS > 9999)
	{
		return 0.0;
	}

	if (toupper(D) != 'N' && toupper(D) != 'E' && toupper(D) != 'S' && 
		toupper(D) != 'W')
	{
		return 0.0;
	}

	SSMAS = (float)MAS;
	
	while (SSMAS > 1.0)
	{
		SSMAS *= 0.1;
	}
	
	SSMAS = (float)SS + (float)(SSMAS);

	// formula applied to cast individual int values to floats and convert
	// to decimal degrees. 
	degreeFloat = ((float)(DD) + (((float)MM)/(((float)60))) + ((SSMAS)/((float)3600)));

	if (toupper(D) == 'S' || toupper(D) == 'W')
	{
		degreeFloat = -degreeFloat;
	}

	return degreeFloat;
}

// Tests the airPdata struct to ensure the LocID meets specified criteria,
// if any failures occur the struct is destroyed, memory is freed, and the
// node will not be inserted into the AVL tree. 
void sortByLocID (airPdata *airport)
{
	int flag = 0;

	if (airport != NULL)
	{
		int i, nameLen = 0;
		nameLen = strlen(airport->LocID);

		if ((nameLen != 3 && nameLen != 4))
		{
			flag = 1;
		}

		else 
		{
			for (i = 0; i < nameLen; i++)
			{
				if (isdigit(airport->LocID[i]))
				{
					flag = 1;
				}
			}
		}
	}
	if (flag == 1)
	{
		destroyAirport(airport);
	}
}

// Tests the airPdata struct to ensure the LocID meets specified criteria,
// if any failures occur the struct is destroyed, memory is freed, and the
// node will not be inserted into the AVL tree.
void sortByLatitude (airPdata *airport)
{
	int flag = 0;

	if (airport != NULL)
	{
		int i, nameLen = 0;
		nameLen = strlen(airport->LocID);

		if ((nameLen != 3 && nameLen != 4))
		{
			flag = 1;
		}

		else 
		{
			for (i = 0; i < nameLen; i++)
			{
				if (isdigit(airport->LocID[i]))
				{
					flag = 1;
				}
			}

		}
	}
	if (flag == 1)
	{
		destroyAirport(airport);
	}
	
}

// A utility function to get height of the tree
int height(struct node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

// A utility function to get maximum of two integers
int max(int a, int b)
{
    return (a > b)? a : b;
}

// Helper function that allocates a new node with the given key and
// NULL left and right pointers.
struct node* newNode(float key, airPdata *airport)
{
    struct node* node = (struct node*) malloc(sizeof(struct node));
    node->airport = airport;
    node->key   = key;
    node->left   = NULL;
    node->right  = NULL;
    node->height = 1;
    return(node);
}

// A utility function to right rotate subtree rooted with y
// See the diagram given above. 
struct node *rightRotate(struct node *y)
{
    struct node *x = y->left;
    struct node *T2 = x->right;
 
    x->right = y;
    y->left = T2;
 
    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;
 
    return x;
}

// A utility function to left rotate subtree rooted with x
// See the diagram given above.
struct node *leftRotate(struct node *x)
{
    struct node *y = x->right;
    struct node *T2 = y->left;

    y->left = x;
    x->right = T2;
 
    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;
 
    return y;
}

// Get Balance factor of node N
int getBalance(struct node *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

// Inserts a node into the AVL tree, calculates heights, uses heights to
// calculate balance factor and performs necessary rotations based on 
// balance factor, returns a struct node pointer to the newly inserted node. 
struct node* insert(struct node* node, float key, airPdata *airport)
{
    if (node == NULL)
        return(newNode(key, airport));
 
    if (key < node->key)
        node->left  = insert(node->left, key, airport);
    else
        node->right = insert(node->right, key, airport);

    node->height = max(height(node->left), height(node->right)) + 1;
 
    int balance = getBalance(node);

    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    if (balance > 1 && key > node->left->key)
    {
        node->left =  leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// A utility function to print inOrder traversal of the tree to stdout.
void inOrder(struct node *root)
{
    if(root != NULL)
    {
      inOrder(root->left);
      printData(root->airport);
      inOrder(root->right);
    }
}

// A utility function to concatenate two int values on either side of a decimal
float concat(float a, float b)
{
	float pow = 10.0;
    
    while(b >= pow)
    {
    	pow *= 10.0;
    }
    return (float)(a * pow + b);        
}

// A custom function to convert the character string passed in to an accurately
// represented float value that can later be used as the key in the AVL tree.
float LocID2Float(char *LocID)
{
	if (LocID == NULL)
	{
		return 0.0;
	}

	float letter[4] = {0.0};
	int i, length = 0;
	length = strlen(LocID);
	float converted = 0.0;

	for (i = 0; i < length; i ++)
	{
		letter[i] = LocID[i] - 0.0;
		converted = concat(converted, letter[i]);
	}

	return converted;
}