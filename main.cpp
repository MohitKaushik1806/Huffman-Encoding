#include<bits/stdc++.h>
#include<iostream>
using namespace std;


int ispowerof2(int i){
	if (i==1)
        return 0;
	if (i==0)
        return 1;
	while (i>2)
	{
		if (i%2!=0)
		return 0;
		i=i/2;
	}
	return 1;
}

int fn(int l){
	if (l==1||l==0) 
        return 0;
	return 2*fn(l-1)+1;
}

int isequal(const char a[], const char b[], int length){
	int i=0;
	while (i<length)
	{
		if(b[i]!=a[i])
            return 0;
		i++;
	}
	if (a[i]!='\0')
        return 0;
	return 1;
}

class node{
public:
    char symbol;
    int freq;
    char *code;
    node *left;
    node *right;
};

class BinaryTree{
private:
  node *root;  

public:

    BinaryTree(){
        root=NULL;
    }
    
    void print(){
        if(root==NULL)
            return;
        cout<<root->symbol<<"\t"<<root->freq<<endl;
        if(root->symbol!='\0')
            return;
            
        BinaryTree l,r;
        //linked the left and right of the root to the new made binaryTree for recursion
        l.root=root->left;
        r.root=root->right;
        l.print();
        r.print();
    }
    
    void assign_code(int i){
        if(root==NULL)
            return;
            
        //check if leaf then print
        if(root->symbol!='\0'){
            root->code[i]='\0'; //to end that char array
            cout<<root->symbol<<"\t"<<root->code<<endl;
            return;
        }
        
        BinaryTree l,r;
        //link the root's left and right child to the new binary tree
        l.root=root->left;
        r.root=root->right;
        
        //now declare code char array in that l,r tree
        l.root->code=new char[i+1];
        r.root->code=new char[i+1];
        
        //looping to get the information from above tree
        for(int k=0;k<i;k++){
            l.root->code[k]=root->code[k];
            r.root->code[k]=root->code[k];
        }
        
        //by default left-'0' and right-'1'
        l.root->code[i]='0';
        r.root->code[i]='1';
        i++;
        
        l.assign_code(i);
        r.assign_code(i);
    }
    
    void encode(string e){
        if(root==NULL)
            return;
        int i=0;
        cout<<"Encoded code for the File is\n";
        while(1)
        {
            if(int(e.size())==i)
            {
                cout<<endl; 
                return;
            }
            print_code(e[i]);			
            i++;		
        }
    }

    void print_code(char c)
    {
        int flag=0;
        
        //if leaf
        if (root->symbol!='\0')
        { 
            if (c==root->symbol) 
            {
                flag=1;
                //code is present in this root for char 'c', which has been assigned to it in assign_code function
                cout<<root->code;
            }
            return ; 
        }		
        BinaryTree l,r; 
        l.root=root->left;
        if (flag==0)
            l.print_code(c);			
        
        r.root=root->right;
        if (flag==0)
            r.print_code(c);
    }
    
    //decoding functions
    void decode(string cd,int size){
        if (root==NULL)
            return;
        int i=0;
        int length=0;
        int f;
        char *s;
        cout<<endl<<"Decoded string for the input code '"<<cd<<"' is\n";
        while (i<size)
        {
            f=0;		
            
            //from i to end string is assigned in s
            s=&cd[i];
            //cout<<s<<endl;
            while (f==0)
            {
                length++;
                print_symbol(s,f,length);			
            }							
            i=i+length;
            length=0;		
        }
        cout<<endl;
    }
    
    void print_symbol(char cd[], int &f, int length){   
        if (root->symbol!='\0'){ 
            if (isequal(root->code, cd, length)){
                f=1;
                cout<<root->symbol;
            }
            return; 
        }
       
        BinaryTree l,r; 
        l.root=root->left;
        if (f!=1)
        l.print_symbol(cd,f,length);			
        r.root=root->right;
        if (f!=1)
        r.print_symbol(cd,f,length);		
}
    
    friend class Huffman;
    friend class minheap;
};

class minheap{
private:
    BinaryTree *a;
    //int n;  //total number of symbols
public:

    //will take input in this constructor and construct our binary tree 
    //which later will be converted into minheap
    minheap(){
        ifstream input("Address of thr file");
        char data;
        vector<int> v(256,0);
        while(!input.eof()){
            input.get(data);
            //input>>data;
            v[data]++;
            
        }
        input.close();
        int n=0;
        for(int i=0;i<256;i++){
            if(v[i]>0)
                n++;
        }
        
        a=new BinaryTree[n+1];
        a[0].root=new node;
        a[0].root->freq=n;
        
        cout<<"File has "<<n<<" characters"<<endl;
        cout<<"Symbol"<<"\t"<<"frequency"<<endl;
        int j=1;
        for(int i=1;i<=256;i++){
            if(v[i-1]>0){
                a[j].root=new node;
                a[j].root->symbol=char(i-1);
                cout<<a[j].root->symbol<<"\t";
                a[j].root->freq=v[i-1];
                cout<<a[j].root->freq<<endl;
                
                //at this time all nodes are leaf nodes
                a[j].root->code=NULL;
                a[j].root->left=NULL;
                a[j].root->right=NULL;
                j++;
            }
        }

        cout<<endl<<"Tree after taking input-"<<endl;
        print();
        
        int i=int(n/2);
        while(i>0){
            heapify(i);
            i--;
        }
        
        cout<<"After heapification"<<endl;
        print();
    }
    
    void heapify(int i){
        while(true){
            //if left child doesn't exists then return 
            if(2*i > a[0].root->freq)
                return;
            
            //means only left chid exists
            if(2*i+1> a[0].root->freq){
                //and check if that child satisfy minHeap property if not swap them 
                if(a[i].root->freq >= a[2*i].root->freq)
                    swap(a[i],a[2*i]);
                return;
            }
            
            //if we are here means both left and right child exists
            int m=min(a[2*i].root->freq,a[2*i+1].root->freq);
            
            //if parent is smaller than both its children then minHeap property is satisfied
            //root<left && root<right
            if(a[i].root->freq <= m)
                return;
                
            if(a[2*i].root->freq > a[2*i+1].root->freq){
                //satisfying minHeap prop
                swap(a[i],a[2*i+1]);
                i=2*i+1;
            }
            else{
                swap(a[i],a[2*i]);
                i=2*i;
            }
        }
    }
    
    BinaryTree remove(){
        BinaryTree ans=a[1];
        a[1]=a[a[0].root->freq]; //a[1] pe last wala aa jayega aur last wala htt jayega coz 
                           //freq-- krdi toh usko reach hi nhi kr paunga mein 
        a[0].root->freq--;
        if(a[0].root->freq!=1)
            heapify(1); 
            
        return ans;
    }
    
    void insert(BinaryTree b){
        a[0].root->freq++;
        
        //inserting in last position
        a[a[0].root->freq]=b;
        
        //normal heapify it
        int i=int(a[0].root->freq/2);
        while(i>0){
            heapify(i);
            i--;
        }
    }
    
    void print(){
        cout<<"minHeap with there symbol frequency-"<<endl;
        if(a[0].root->freq==0){
            cout<<endl;
            system("pause");
            return;
        }
        int level=1;
        while( a[0].root->freq >= pow(2,level) )// 2^n-1 is the max. no. of nodes in a complete tree of n levels
            level++;
        if(level==1)
        {
            cout<<a[1].root->freq<<"\n";
            system ("pause");
            return;
        }	
        for (int i=1; i<=a[0].root->freq; i++)
        {
            if (ispowerof2(i))
            {
                cout<<"\n"; 
                level--;
            }
            for (int k=1; k<=fn(level); k++)
                cout<<" ";
            
            cout<<a[i].root->freq<<" "; 
            
            for (int k=1; k<=fn(level); k++)
                cout<<" ";
        }
        cout<<endl;
        system ("pause");
    }
    
    friend class Huffman;
};


class Huffman{
private:
        BinaryTree HuffmanTree; //A Huffman Tree (a minimum weighted external path length tree) 
        //with symbols as external nodes.
public:
        Huffman(){
            
            //minheap constructor will be called and heap will be formed
            minheap heap;
            
            // Huffman Tree is build from bottom to top.
            // The symbols with lowest frequency are at the bottom of the tree 
            // that leads to longer codes for lower frequency symbols and hence
            // shorter codes for higher frequency symbol giving OPTIMAL codelength.
            while (heap.a[0].root->freq>1)
            {
                // The first two trees with minimum frequency are taken
                BinaryTree l=heap.remove();
                cout<<"\nAfter removing "<<l.root->freq<<endl;
                heap.print();
                BinaryTree r=heap.remove();
                cout<<"\nAfter removing "<<r.root->freq<<endl;
                heap.print();
                
                // a new tree is constructed taking the above trees as left and right sub-trees 
                // with the frequency of root node as the sum of frequencies of left and right child.
                HuffmanTree.root=new node;
                HuffmanTree.root->symbol='\0';
                HuffmanTree.root->freq=l.root->freq + r.root->freq;
                
                //making link between root and its subtrees
                
                /*  root  freq=l.root->freq +r.root->freq
                    /  \
                   /    \
                 left   right */
                 
                HuffmanTree.root->left=l.root;
                HuffmanTree.root->right=r.root;
                
                // then it is inserted in the Tree and Tree is heapified again.
                // Deletion and Insertion at an intermediate step is facilitated in heap-sort.
                heap.insert(HuffmanTree);
                cout<<"\nAfter inserting "<<l.root->freq<<"+"<<r.root->freq<<"= "<<HuffmanTree.root->freq<<endl;
                heap.print();	
            }
            cout<<endl<<"Process is completed and the Huffman Tree is ready"<<endl;
            system("pause");
            
            //At the end only one node will be left in our heap, that will contain our HuffmanTree
            //because all the connections have been built above 
            HuffmanTree=heap.a[1];
            delete []heap.a;
            //'a' was declared dynamically so delete it so as to avoid any memory leak.
            
            cout<<"Huffman tree traversal"<<endl;
            HuffmanTree.print();
            
            system ("pause");
            cout<<"\nThe symbols with their codes are as follows\n";
            HuffmanTree.assign_code(0);
            system ("pause");
            
            //encoding
            ifstream input("C:/Users/HIMANSHI KAUSHIK/Desktop/read.txt");
            char data;
            string input_for_encoding="";
            while(!input.eof()){
                input.get(data);
                input_for_encoding+=data;
            }
            input.close();
            HuffmanTree.encode(input_for_encoding);
            system ("pause");
            
            //decoding
            string decode_string;
            cout<<"Enter the code to be decoded: ";
            cin>>decode_string;
            HuffmanTree.decode(decode_string,int(decode_string.size()));
            
            return;
        }
};

int main()
{
    Huffman c;
    system("pause");
    return 0;    
}