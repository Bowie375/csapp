#include<iostream>
#include <math.h>
#include <map>
#include <string>
using namespace std;

map<pair<char,int>,char> transitTable={{{'0',0},'2'},{{'0',1},'1'},
                                    {{'1',0},'4'},{{'1',1},'5'},
                                    {{'2',0},'3'},{{'2',1},'6'},
                                    {{'3',0},'1'},{{'3',1},'5'},
                                    {{'4',0},'6'},{{'4',1},'5'},
                                    {{'5',0},'5'},{{'5',1},'0'},
                                    {{'6',0},'0'},{{'6',1},'2'}};

string test="0123456";

bool is_valid(int seq,int len){
    int tmp=0;
    int strlen=test.length();
    string arr=test;

    for(int i=len-1;i>=0;--i){
        for(int j=0;j<strlen;++j){
            arr[j]=transitTable[{arr[j],(seq>>i)&1}];
        }
    }
    
    tmp=arr[0];
    for(int i=1;i<strlen;++i){
        if(arr[i]!=tmp){
            return false;
        }
    }

    return true;
}

int main(){
    bool flag=0;
    for(int i=0;i<(1<<13);++i){
        for(int j=1;j<=13;++j){
            if(is_valid(i,j)){
                flag=1;
                for(int k=j-1;k>=0;--k){
                    cout<<((i>>k)&1);
                }
                return 0;
            }
        }
    }
    if(!flag){
        cout<<"no answer!"<<endl;
    }
    return 0;
}
