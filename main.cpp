#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>

using namespace std;

void input_file(); //functions explained later on (when they are defined)
void output_file();
void process_commands(string &input_file, string &output_file);
void dissect_command(char *command);
ofstream *outFile;

template <class T>
class SimpleList // abstract base class for stacks and queues
{
public:
    SimpleList(){
        head = new Node;
        tail = new Node; // shadow node
    }
    string name;
    virtual void push(T) = 0; //virtual functions for pushing and popping
    virtual T pop() = 0;
    bool is_empty() //function to check if list is empty
    {
        return this -> head->next == NULL;
    };

protected:
    void push_front(T new_data); //specific pushing and popping functions
    T pop_front();
    void push_end(T new_data);

private:
    class Node
    {
    public:
        T data;
        Node *next = NULL; //pointer to next node
    };
    Node* head = NULL;
    Node* tail = NULL; //initialization
};

template <class T>
SimpleList<T>* search_list(list<SimpleList <T>*> &lsl, string list_name); //function declaration (needed to be declared after SimpleList)

template <class T>
class Stack : public SimpleList <T> //derived class: stack
{
public:
    Stack() {
    }

    Stack (string input_name)
    :SimpleList<T>()
    {
        this->name=input_name;
    }
    void push(T elem)
    {
        this->push_front(elem);
    }
    T pop()
    {
       return this->pop_front();
    }

};

template <class T>
class Queue : public SimpleList <T>//derived class: queue
{
public:
    Queue() {
    }

    Queue (string input_name)
    :SimpleList<T>()
    {
        this->name=input_name;
    }
    void push(T elem)
    {
        this->push_end(elem);
    }
    T pop()
    {
        return this->pop_front();
    }
};

int main() {

    string input_file, output_file;
    cout << "Enter name of input file: ";
    cin >> input_file;
    cout << "Enter name of output file: ";
    cin >> output_file;

    process_commands(input_file, output_file);

    return 0;
}


template <class T>
void SimpleList<T>::push_front(T new_data) // push for stack
{
    Node* new_node = new Node();
    new_node->data = new_data;
    new_node->next = head->next;
    head->next = new_node;
}

template <class T>
T SimpleList<T>::pop_front() //popping for both queue and stack
{
    if (head->next == NULL)
    {
        *outFile << "ERROR: This list is empty!" <<endl;
        return T();
    }
    T temp = head->next->data;
    head->next= head->next->next;

    return temp;
}

template <class T>
void SimpleList<T>::push_end(T new_data) //pushing for queue
{
    Node* new_node = new Node();
    new_node->data = new_data;

    if (tail->next != NULL) {
        tail->next->next = new_node;
    }

    tail->next = new_node;

    if (head->next == NULL)
    {
        head->next = tail->next;
    }

}

void process_commands(string &input_file, string &output_file) //parse input files and process commands
{
    list<SimpleList<int> *> listSLi;
    list<SimpleList<double> *> listSLd;
    list<SimpleList<string> *> listSLs;

    ifstream ifs(input_file);
    ofstream ofs(output_file);
    outFile = &ofs;

    if(ifs.is_open())
    {
        string command,c1,c2,c3;

        while(getline(ifs,command))
        {
            *outFile << "PROCESSING COMMAND: " << command << endl;
            stringstream ss;
            ss.str(command);

            ss>>c1; //dissecting command line into 3 parts
            ss>>c2;
            ss>>c3;

            if(c1 == "create")
            {
                if (c2[0] == 'i') //2nd loop: nested if else statements to test for 'i' vs 'd' vs 's'
                {
                    SimpleList<int>* l = search_list(listSLi, c2);
                    if (l==nullptr) // if file does not exist
                    {
                        if (c3=="stack")
                        {
                            SimpleList<int> *sl = new Stack<int> (c2);
                            listSLi.push_back(sl);
                        }
                        else // c3=queue
                        {
                            SimpleList<int> *sl = new Queue<int> (c2);
                            listSLi.push_back(sl);
                        }
                    }
                    else
                    {
                        *outFile << "ERROR: This name already exists!" << endl;
                        continue;
                    }
                }
                else if (c2[0] == 'd')
                {
                    SimpleList<double>* l = search_list(listSLd, c2);
                    if (l==nullptr)
                    {
                        if (c3=="stack")
                        {
                            SimpleList<double> *sl = new Stack<double> (c2);
                            listSLd.push_back(sl);
                        }
                        else //c3=queue
                        {
                            SimpleList<double> *sl = new Queue<double> (c2);
                            listSLd.push_back(sl);
                        }
                    }
                    else
                    {
                        *outFile << "ERROR: This name already exists!" << endl;
                        continue;
                    }
                }
                else //c2[0]= s
                {
                    SimpleList<string>* l = search_list(listSLs, c2);
                    if (l==nullptr)
                    {
                        if (c3=="stack")
                        {
                            SimpleList<string> *sl = new Stack<string> (c2);
                            listSLs.push_back(sl);
                        }
                        else// c3="queue"
                        {
                            SimpleList<string> *sl = new Queue<string> (c2);
                            listSLs.push_back(sl);
                        }
                    }
                    else
                    {
                        *outFile << "ERROR: This name already exists!"<< endl;
                        continue;
                    }
                }
            }
            else if (c1 == "push")
            {
                if (c2[0] == 'i')
                {
                    SimpleList<int>* l = search_list(listSLi, c2);
                    if (l==nullptr)
                    {
                        *outFile << "ERROR: This name does not exist!" <<endl;
                        continue;
                    }
                    l->push(stoi(c3)); //convert string to int type
                }
                else if (c2[0] == 'd')
                {
                    SimpleList<double>* l = search_list(listSLd, c2);
                    if (l==nullptr)
                    {
                        *outFile << "ERROR: This name does not exist!" <<endl;
                        continue;
                    }
                    l->push(stod(c3)); //convert string to double
                    continue;
                }
                else // c2[0] has to be 's'
                {
                    SimpleList<string>* l = search_list(listSLs, c2);
                    if (l==nullptr)
                    {
                        *outFile << "ERROR: This name does not exist!" <<endl;
                        continue;
                    }
                    l->push(c3);
                }
            }
            else //has to be "pop"
            {
                if (c2[0] == 'i')
                {
                    SimpleList<int>* l = search_list(listSLi, c2);
                    if (l==nullptr)
                    {
                        *outFile << "ERROR: This name does not exist!" <<endl;
                        continue;
                    }
                    if (l->is_empty())
                    {
                        *outFile << "ERROR: This list is empty!" << endl;
                        continue;
                    }
                    *outFile << "Value popped: " <<l->pop()<<endl;
                }
                else if (c2[0] == 'd')
                {
                    SimpleList<double>* l = search_list(listSLd, c2);
                    if (l==nullptr)
                    {
                        *outFile << "ERROR: This name does not exist!" <<endl;
                        continue;
                    }
                    if (l->is_empty())
                    {
                        *outFile << "ERROR: This list is empty!" << endl;
                        continue;
                    }
                    *outFile << "Value popped: " <<l->pop()<<endl;
                }
                else // has to be 's'
                {
                    SimpleList<string>* l = search_list(listSLs, c2);
                    if (l==nullptr)
                    {
                        *outFile << "ERROR: This name does not exist!" <<endl;
                        continue;
                    }
                    if (l->is_empty())
                    {
                        *outFile << "ERROR: This list is empty!" << endl;
                        continue;
                    }
                    *outFile << "Value popped: " <<l->pop() << endl;
                }
            }


        }
    }
    else
    {
        cout << "File cannot be opened";
    }

    ifs.close();
    ofs.close();
}

template <class T>
SimpleList<T>* search_list(list<SimpleList <T>*> &lsl, string list_name)
{
    for(auto sl: lsl)
    {
        if (sl->name == list_name)
        {
            return sl; //list exists already
        }
    }
    return nullptr; // list does not exist
}







