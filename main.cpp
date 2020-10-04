#include <iostream>
#include <math.h>
#include <time.h>

using namespace std;
struct pointNode// to traverse the points of a user
{
    int x,y;//create x and y points
    struct pointNode* next;//used to get to the "next" of each node
};


struct Person
{
    int** genome; // stores the seats and rows
    int fitnessScore;
    pointNode* node; // for the fitness

};
pointNode* GetPoint(Person individual, int seats, int rows, int seatDistance, int rowDistance)
{
    pointNode* node;
    pointNode* head;// keep track of first node
    int test = 0;
    node = nullptr;// intialize it as nothing
    for (int i =0 ; i < rows; i++)
    {
        for (int j = 0; j < seats; j ++)
        {
            if ( individual.genome[i][j] == 1)//someone is sitting here
            {
                if (node == nullptr)// for first person, create new node
                {
                    test++;// this is to test if its getting the right points . DELETE IT LATER
                    //  cout<<"in if statement of nullptr"<<endl;
                    node = new pointNode;// create new node
                    node->x=i*rowDistance;// take x coordinate from i
                    node->y=j*seatDistance;// take y coordinate from j
                    head=node;// get first node
                    //   cout<<"after reassigning head"<<endl;

                }
                else
                {
                    test++;
                    pointNode *newNode =  new pointNode;
                    newNode->x = i*rowDistance;// store x variable of where the person is
                    newNode->y = j*seatDistance;// store y coordinate of where person is defined i as the rows and j as seats
                    if ( node == head)// if the node and head are the same thing (first thing in linkedlist)
                    {
                        head->next=newNode; // store the next of head as the new node data
                    }
                    node->next=newNode;
                    node=newNode; // say the node is now newnode

                }
            }
        }
    }
    node->next= nullptr;//explciitly tell it  to be a null ptr to work in the fitness function after
    return head;
}
int fitness(Person individual, pointNode* head, int minDist)
{
    double distance;
    int notSafe=1;
    int score=0;
    pointNode* node= head;// need two nodes in order to traverse the array
    pointNode* node2= head;//inner node used to traverse array
    while ( node != nullptr)
    {
        score=score+1;// scoring for existing
        while ( node2 != nullptr) { //iterate thru columns and rows, and find people via this.
            distance = sqrt(pow((node->x - node2->x), 2) + pow((node->y - node2->y), 2));// print distance
            if (distance < minDist &&distance != 0)// the !=0 prevents the distance formula to get a 0 and causes it to  overcount
            {
                score = score - 2;// penalty
                notSafe = 0; // fail
                break;
            }
            node2 = node2->next;
        }
        // cout<<"inside outer loop (loop1 ):"<<counter<<endl;
        node2=head;//reset node2 to head
        node=node->next;
    }
    //cout<<"counter "<<counter<<endl;
    if ( notSafe==1 )// didnt fail, only added once
    {
        cout<<"in not safe"<<endl;
        score=score+10;// 10 pt bonus
    }
    return score;
}
int RandomInRange(int min, int max)
{
    int randomRange= rand()%(max+1-min)+min; // grabs max, adds 1 and subtracts min and grabs min again.
    return randomRange;
}

int** createArray(int seats, int rows)// this will create a 2d array of a persons genome
{
    int** dynamicArray; //intialize the array, which is the "genome"
    dynamicArray= new int*[rows];
    for (int i = 0; i < rows; i++)
    {
        dynamicArray[i]=new int[seats];
        for (int k =0; k<seats; k++)
        {
            dynamicArray[i][k]=RandomInRange(0,1);//randomize the initial genome, which is a 2d array
        }
    }
    return dynamicArray;
}
Person createPerson(int seats, int rows, int rowDistance, int seatDistance)
{
    Person individual;
    individual.genome=createArray(seats,rows);
    individual.node= GetPoint(individual,seats,rows,rowDistance,seatDistance);// grab the persons point
    individual.fitnessScore=fitness(individual,individual.node,72);// 72 inches = 6 feet per specifications
    return individual;
}
Person nextGeneration(int seats, int rows, Person mother, Person father, int rowDistance, int seatDistance)
{
    Person newPerson;
    newPerson.genome=new int*[rows];
    for (int i = 0; i < rows; i++)
    {
        newPerson.genome[i]=new int[seats];
        for (int k =0; k<seats; k++)
        {
            int genePass = RandomInRange(0,100);//randomizes the genes
            if(genePass < 45 ) // 45% chance is mother
            {
                newPerson.genome[i][k] = mother.genome[i][k];//access values of mother using 2d array, and takes where mother is sitting and gives it to kid
            }
            else if ( genePass < 90 ) // other 45% chance is father
            {
                newPerson.genome[i][k]=father.genome[i][k];
            }
            else// remaining 10 % which means mutation
            {
                newPerson.genome[i][k]= RandomInRange(0,1); // randomizes new persons genes
            }
        }
    }
    newPerson.node= GetPoint(newPerson,seats,rows,rowDistance,seatDistance);// grab the persons point
    newPerson.fitnessScore=fitness(newPerson,newPerson.node,72);// 72 inches = 6 feet per specifications

    return newPerson;// return the new person
}



void printGenome(Person individual, int seats, int rows)
{
    for (int i = 0 ; i < rows; i++)
    {
        for ( int j = 0; j < seats; j ++)
        {
            cout << individual.genome[i][j];// print all the values of the genome
        }
        cout<<endl;
    }
    cout<<"fitness: "<<individual.fitnessScore<<endl;//print fitness
    cout<<"----------"<<endl;// make border
}

void sortArray(Person array[],int first, int last) {
    int i, j, pivot;
    Person temp;
    if (first < last)//base case for recursion
    {
        pivot = first;//initialize pivot
        i = first;//initialize i as far leftmost
        j = last;//initialize j as far rightmost
        while (i < j)//if leftmost<rightmost
        {
            while (array[i].fitnessScore <= array[pivot].fitnessScore &&
                   i < last)//this compares the values and makes sure it doesnt go out of bounds
                i++;//increment i
            while (array[j].fitnessScore >
                   array[pivot].fitnessScore)//compare values and make sure it goes out of bounds again
                j--;//decrement j
            if (i < j) {//if want to swap variables
                temp = array[i];//store in temp
                array[i] = array[j];//index of i is now index of j
                array[j] = temp;//j is now i
            }
        }

        temp = array[pivot];//store the pivot
        array[pivot] = array[j];//put pivot in the value of j
        array[j] = temp;//store the index of j in temp
        sortArray(array, first, j - 1);//recursive call go to go from leftwards-> rightwards
        sortArray(array, j + 1, last);//go rightwards->leftwards
    }
}
int main()
{
    srand((unsigned)(time(0)));//used to randomize the "seed" of random functions
      int rowCount=0;
      int seatCount=0;
      int seatDistance=0;
      int rowDistance=0;
      int population=0;
      std::cout <<"How many rows?" << std::endl;
      cin>>rowCount;
      std::cout<<"How many seats?"<<endl;
      cin>>seatCount;
      cout<<"Distance between seats in inches?"<<endl;
      cin>>seatDistance;
      cout<<"Distance between rows in inches?"<<endl;
      cin>>rowDistance;
      cout<<"How many people do you want in your population?"<<endl;
      cin>>population;

    int userInput=4;
    int generation=0;
    Person individual;

    Person populationArray[population];

    for (int i = 0 ; i < population; i++)
    {

        populationArray[i]=createPerson(seatCount,rowCount,rowDistance,seatDistance);// create a array of people

    }
    sortArray(populationArray,0,population-1);// sort the fitness
    for (int i = 0 ; i < population; i++)
    {
        printGenome(populationArray[i],seatCount,rowCount);// print the genome
    }
    cout<< "/////////////////////////////////////"<<endl;
    cout<<"Would you like to continue? press 1 if yes and 0 if no"<<endl;
    cin>>userInput;
    if ((userInput != 1) && (userInput !=0))//user puts a number thats illegal
    {
        while (userInput != 1)
        {
            cout<<"Try again. press 1 if yes and 0 if no"<<endl;
            cin>>userInput;
            if (userInput == 0) // ask user wants to leave
                    {
                            return 0; // end program
                    }
        }
    }
    int indexOfMotherGene = 0;
    int indexOfFatherGene=0;
    if (userInput==1)
    {
        Person newGenerationArray[population];
        int survivingCount= (population-(population*.05));//take the top 5% of the best

        int topMateCount=population*.5;//make them have kids with the top 50% of the population

        while (userInput==1)//prompt the user to keep going
        {
            for (int i = 0 ; i < survivingCount; i++)// this is to make the top 5% to mate, it takes random indexes from the "good" parents randomly and makes kids from it
            {
                indexOfFatherGene=RandomInRange(topMateCount,population-1);//needs an offset of -1 for the randominrange cause 0 is counted
                indexOfMotherGene=RandomInRange(topMateCount,population-1);// put this in the while loop so it makes new families
                while ( indexOfFatherGene == indexOfMotherGene)//prevents having same mother and father gene (so its just a clone)
                {
                    indexOfFatherGene=RandomInRange(topMateCount,population-1);
                    indexOfMotherGene=RandomInRange(topMateCount,population-1);// put this in the while loop so it makes new families
                }
                newGenerationArray[i] = nextGeneration(seatCount, rowCount, populationArray[indexOfMotherGene],
                                                       populationArray[indexOfFatherGene],rowDistance,seatDistance);//create the new generation
            }

            for (int i = survivingCount; i < population; i++)
            {
                newGenerationArray[i]=populationArray[i];//store old array data in new data to swap
            }
            for (int i = 0; i < population; i++)//rewrite new  array over old array
            {
                populationArray[i]=newGenerationArray[i];
            }
            sortArray(populationArray,0,population-1);
            for ( int i = 0; i < population; i++)
            {
                printGenome(populationArray[i],seatCount,rowCount);//print the "genome", aka the seating
            }
            generation++;//increment amount of generations
            cout<<"Do you want to run a next generation? Type 1 for yes and 0 for no"<<endl; //prompt the user to ask for next generation
            cin>>userInput;//store input
            if ((userInput != 1) && (userInput !=0))//user puts a number thats illegal
            {
                while (userInput != 1)
                {
                    cout<<"Try again. press 1 if yes and 0 if no"<<endl;
                    cin>>userInput;
                    if (userInput == 0) // ask user wants to leave
                    {
                        cout<<"Generations created : "<<generation<<endl;//show user amount of generations created
                        return 0; // end program
                    }
                }
            }
            cout<<"Generations created : "<<generation<<endl;//show user amount of generations created
        }
    }


    return 0;
}
