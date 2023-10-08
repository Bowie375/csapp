#include<iostream>
#include <math.h>
#include <map>
#include <string>
using namespace std;

/*this table is the definition of the fsm*/
map<pair<char,int>,char> transitTable={{{'0',0},'2'},{{'0',1},'1'},
                                    {{'1',0},'4'},{{'1',1},'5'},
                                    {{'2',0},'3'},{{'2',1},'6'},
                                    {{'3',0},'1'},{{'3',1},'5'},
                                    {{'4',0},'6'},{{'4',1},'5'},
                                    {{'5',0},'5'},{{'5',1},'0'},
                                    {{'6',0},'0'},{{'6',1},'2'}};

string init_points="0123456"; //the initial values we work on. They should give the same output for the fsm.

/*this function is used to test if the 
* last 'len' bits of the sequence 'seq'
* satisfies our need.
*/
bool is_valid(int seq,int len){
    int tmp=0;
    int arrlen=init_points.length();
    string arr=init_points;

    /*apply fsm to the sequence and the init_points*/
    for(int i=len-1;i>=0;--i){
        for(int j=0;j<arrlen;++j){
            arr[j]=transitTable[{arr[j],(seq>>i)&1}];
        }
    }
    
    /*check if all the outputs are the same*/
    tmp=arr[0];
    for(int i=1;i<arrlen;++i){
        if(arr[i]!=tmp){
            return false;
        }
    }

    return true;
}

int main(){
    bool found=0;

    /*we use the bit-sequence of 'i' to represent 
    * our answer sequence and 'j' to represent the
    * length of the sequence here.
    */
    for(int j=1;j=13;++j){
        for(int i=0;i<(1<<j);++i){
            if(is_valid(i,j)){
                found=1;

                /*output the sequence according to bit order*/
                for(int k=j-1;k>=0;--k){
                    cout<<((i>>k)&1);
                }
                return 0;
            }
        }
    }

    if(!found){
        cout<<"no answer!"<<endl;
    }
    return 0;
}
